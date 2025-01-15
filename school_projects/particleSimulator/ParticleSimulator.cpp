#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "ParticleSimulator.h"
#include "ParticleSystem.h"


ParticleSimulator::ParticleSimulator(const std::string& name, BaseSystem* target) : BaseSimulator(name), m_object(NULL), prev_time(0.0)
{
	/*m_object(target),*/
}

ParticleSimulator::~ParticleSimulator()
{
}


std::vector<int> ParticleSimulator::get_ptrl_offset(int k)
{
	std::vector<int> offS_vect;
	int begin_one = k * (7 + sim_num_prtls);
	for (int i = 0; i < 7; i++)
	{
		offS_vect.push_back(begin_one + i);
	}
	return offS_vect;
}

/*
* Initializes the indices of 
*/
void ParticleSimulator::init_links()
{
	if (links_set == 0)
	{
		links_set = 1;
		for (int i = 0; i < sim_num_prtls; i++)
		{
			/*ptlr_i's neignbours. lk_offset always points to self. If i = 2 and lk_offset = x
			* then x is the offset of particle 2.
			* Note, self neighbouring is not allowed. No springs pointing to self when the springs are drawen.
			*/
			int lk_offset = (i * (7 + sim_num_prtls)) + 7;
			// 2 ptrls p0_offset = 7, p1_offset = 8 for i = 0
			for (int j = 0; j < sim_num_prtls; j++)
			{
				prtls_pos_vel[lk_offset + j] = -1;
			}
		}
	}
}

//int ParticleSimulator::get_num_particles()
//{
//	int end_pos = 1;
//	// approximate number of particles. 2 praticles for each spring. 3 * Nsp since no direct access;
//	int last_pos = (14 * num_of_sp) + 1;
//	int count = 0;
//	while (end_pos > 0)
//	{
//		//random value used in getState() in ParticleSystem. Can break program, at least visually. 
//		if (prtls_pos_vel[count] == 1010.012345)
//		{
//			// even if last_pos >> true number of prtls count = true number of prtls due to this "if".
//			end_pos = 0;
//		}
//		else
//		{
//			count += 1;
//			if (count > last_pos)
//			{
//				end_pos = 0;
//				count -= 1;
//			}
//		}
//	}
//	count = count / 7;
//	return count;
//}


void ParticleSimulator::euler_method(std::vector<int> indices, Vector frc, double mass_i, double euler_t, int prtl_i)
{
	//[px, py, pz, vx, vy, vz, m]
	//[ 0, 1 , 2 , 3 , 4 , 5 , 6]


	// [px, py, pz]
	int pos_ind[3];
	pos_ind[0] = indices[0];
	pos_ind[1] = indices[1];
	pos_ind[2] = indices[2];

	// [vx, vy, vz]
	int vel_ind[3];
	vel_ind[0] = indices[3];
	vel_ind[1] = indices[4];
	vel_ind[2] = indices[5];

	double prv_py, cur_vy;
	prv_py = prtls_pos_vel[pos_ind[1]];
	cur_vy = prtls_pos_vel[vel_ind[1]];

	double ptl_m_fr = 1.0 / mass_i;
	double frc_coeff = dlt_t * ptl_m_fr;
	Vector frc_s;
	VecCopy(frc_s, frc);
	// store new position first before updating the velocity.
	// store position at time: t + dlt_t
	for (int i = 0; i < 3; i++)
	{
		double nw_upd = dlt_t * prtls_pos_vel[vel_ind[i]];
		prtls_pos_vel[pos_ind[i]] += nw_upd;
	}
	// store new velocity at time: t + dlt_t
	for (int i = 0; i < 3; i++)
	{
		// frc_coeff = dlt / m_i
		double nvalue = frc_coeff * frc_s[i];
		prtls_pos_vel[vel_ind[i]] += nvalue;
	}

	// For each particle, check if it falls through the ground
	double grdCls_pnlty = ground_collision_forces(indices, cur_vy);
	// grdCls_pnlty is either: pos_ind[1](didn't fall through) or the value the dot product of Penalty_Method_force and Normal_vector_ground.
	if (grdCls_pnlty == 0.0)
	{
		prtls_pos_vel[vel_ind[1]] = grdCls_pnlty;
		prtls_pos_vel[pos_ind[1]] = grdCls_pnlty;
	}
	else
	{
		if (grdCls_pnlty < 0.0) // less than zero only when particle did not fall through the ground.
		{
			prtls_pos_vel[pos_ind[1]] += 0.0;
		}
		else
		{
			// at time t + dlt_t, the particle falls through the ground. Immitate an unrealistic bounce.
			double end_t = thisTime;
			prtls_pos_vel[pos_ind[1]] += grdCls_pnlty;
			if (prtls_pos_vel[pos_ind[1]] < 0.0)
			{
				prtls_pos_vel[pos_ind[1]] = grdCls_pnlty;
			}
			/*if (bounce == 0)
			{
				animTcl::OutputMessage("Unrealistic bounce!");
				bounce = 1;
			}*/
		}
	}

	if (thisTime > 5.2)
	{
		double checkT = 0.0;
	}

	if (cur_vy == 0.0 && (one_sim > 0))
	{
		prtls_pos_vel[vel_ind[1]] = cur_vy;
		prtls_pos_vel[pos_ind[1]] = prv_py;
	}
	// checks if any of its neighbours has a velocity of 0 in the y direction
	double self_vy = set_self_yvel_zero(prtl_i);
	if (self_vy == 0 && (one_sim > 0))
	{
		prtls_pos_vel[vel_ind[1]] = self_vy;
	}
}

void ParticleSimulator::symplectic_euler(std::vector<int> indices, Vector frc, double mass_i, double euler_t, int prtl_i)
{
	//[px, py, pz, vx, vy, vz, m]
	//[ 0, 1 , 2 , 3 , 4 , 5 , 6]

	// [px, py, pz]
	int pos_ind[3];
	pos_ind[0] = indices[0];
	pos_ind[1] = indices[1];
	pos_ind[2] = indices[2];

	// [vx, vy, vz]
	int vel_ind[3];
	vel_ind[0] = indices[3];
	vel_ind[1] = indices[4];
	vel_ind[2] = indices[5];

	double prv_py, cur_vy;
	prv_py = prtls_pos_vel[pos_ind[1]];
	cur_vy = prtls_pos_vel[vel_ind[1]];
	/*this_prv_vy = prtls_pos_vel[vel_ind[1]];*/

	double ptl_m_fr = 1.0 / mass_i;
	Vector frc_s;
	VecCopy(frc_s, frc);

	VecScale(frc_s, ptl_m_fr);
	VecScale(frc_s, dlt_t);
	if (thisTime > 230.0)
	{
		double pyCheck = 0.0;
	}
	// store velocities first.
	for (int i = 0; i < 3; i++)
	{
		// frc_s was calculated using the previous position and velocity. So the update is for v_i(t + dlt_t) with frc_s(x_i(t), v_i(t))
		if ((i == 1) && (prtls_pos_vel[vel_ind[i]] > 0.0))
		{
			double vyCheck = 0.0;
		}
		prtls_pos_vel[vel_ind[i]] += frc_s[i];
		if ((prtls_pos_vel[vel_ind[0]] != 0) || (prtls_pos_vel[vel_ind[2]] != 0))
		{
			double moved = 0.0;
		}
	}

	// store position at time: t + dlt_t
	for (int i = 0; i < 3; i++)
	{
		double new_vel = prtls_pos_vel[vel_ind[i]];
		double new_pos = dlt_t * new_vel;
		// old_pos + new_pos 
		prtls_pos_vel[pos_ind[i]] += new_pos;
	}

	// For each particle, check if it falls through the ground
	double grdCls_pnlty = ground_collision_forces(indices, cur_vy);
	// grdCls_pnlty is either: pos_ind[1](didn't fall through) or the value the dot product of Penalty_Method_force and Normal_vector_ground.
	if (grdCls_pnlty == 0.0)
	{
		prtls_pos_vel[vel_ind[1]] = grdCls_pnlty;
		prtls_pos_vel[pos_ind[1]] = grdCls_pnlty;
	}
	else
	{
		if (grdCls_pnlty < 0.0) // less than zero only when particle did not fall through the ground.
		{
			prtls_pos_vel[pos_ind[1]] += 0.0;
		}
		else
		{
			// at time t + dlt_t, the particle falls through the ground. Immitate an unrealistic bounce.
			double end_t = thisTime;
			prtls_pos_vel[pos_ind[1]] += grdCls_pnlty;
			if (prtls_pos_vel[pos_ind[1]] < 0.0)
			{
				prtls_pos_vel[pos_ind[1]] = grdCls_pnlty;
			}
			/*if (bounce == 0)
			{
				animTcl::OutputMessage("Unrealistic bounce!");
				bounce = 1;
			}*/
		}
	}

	if (thisTime > 5.2)
	{
		double checkT = 0.0;
	}

	if (cur_vy == 0.0 && (one_sim > 0))
	{
		prtls_pos_vel[vel_ind[1]] = cur_vy;
		prtls_pos_vel[pos_ind[1]] = prv_py;
	}
	// checks if any of its neighbours has a velocity of 0 in the y direction
	double self_vy = set_self_yvel_zero(prtl_i);
	if (self_vy == 0 && (one_sim > 0))
	{ 
		prtls_pos_vel[vel_ind[1]] = self_vy;
	}

}

void ParticleSimulator::verlet_method(std::vector<int> indices, Vector frc, double mass_i, double euler_t, int k)
{
	//[px, py, pz, vx, vy, vz, m]
	//[ 0, 1 , 2 , 3 , 4 , 5 , 6]


	// [px, py, pz]
	int pos_ind[3];
	pos_ind[0] = indices[0];
	pos_ind[1] = indices[1];
	pos_ind[2] = indices[2];

	// [vx, vy, vz]
	int vel_ind[3];
	vel_ind[0] = indices[3];
	vel_ind[1] = indices[4];
	vel_ind[2] = indices[5];

	// set particle k previous position

	double cur_py;
	double cur_vy;

	Vector use_prev_pos;
	// use_prev_pos = x(t - dlt). if t1 use_prev_pos = x(t0)
	setVector(use_prev_pos, previous_pos[k][0], previous_pos[k][1], previous_pos[k][2]);

	cur_py = prtls_pos_vel[indices[1]];
	cur_vy = prtls_pos_vel[indices[4]];

	double prtl_inv_m = 1.0 / mass_i;
	Vector frc_s;
	/*set frc_s to forces acting on particle k, drag + gravity + spring force + damper force */
	VecCopy(frc_s, frc);

	/* store position at time: t + dlt_t */
	for (int i = 0; i < 3; i++)
	{
		double dlt_sqrd = dlt_t * dlt_t;
		// frc_s[i] * 1/m_i
		double acc = frc_s[i] * prtl_inv_m;
		double coeff = acc * dlt_sqrd;
		double nPos = 0.0;
		nPos = (2 * prtls_pos_vel[pos_ind[i]]) - use_prev_pos[i] + coeff;
		previous_pos[k][i] = prtls_pos_vel[pos_ind[i]];
		prtls_pos_vel[pos_ind[i]] = nPos;
	}

	double r = 1.0 / (2.0 * dlt_t);
	// r = r * 0.5;
	// set new velocity.
	for (int i = 0; i < 3; i++)
	{
		double nVel = 0.0;
		nVel = prtls_pos_vel[pos_ind[i]] - use_prev_pos[i];
		// r = 1/ (2*dlt)
		prtls_pos_vel[vel_ind[i]] = r * nVel;
	}

	// For each particle, check if it falls through the ground
	double grdCls_pnlty = ground_collision_forces(indices, cur_vy);
	// grdCls_pnlty is either: pos_ind[1](didn't fall through) or the value the dot product of Penalty_Method_force and Normal_vector_ground.
	if (grdCls_pnlty == 0.0)
	{
		prtls_pos_vel[vel_ind[1]] = grdCls_pnlty;
		prtls_pos_vel[pos_ind[1]] = grdCls_pnlty;
	}
	else
	{
		if (grdCls_pnlty < 0.0) // less than zero only when particle did not fall through the ground.
		{
			prtls_pos_vel[pos_ind[1]] += 0.0;
		}
		else
		{
			// at time t + dlt_t, the particle falls through the ground. Immitate an unrealistic bounce.
			double end_t = thisTime;
			prtls_pos_vel[pos_ind[1]] += grdCls_pnlty;
			if (prtls_pos_vel[pos_ind[1]] < 0.0)
			{
				prtls_pos_vel[pos_ind[1]] = grdCls_pnlty;
			}
			/*if (bounce == 0)
			{
				animTcl::OutputMessage("Unrealistic bounce!");
				bounce = 1;
			}*/
		}
	}

	if (thisTime > 5.2)
	{
		double checkT = 0.0;
	}

	if (cur_vy == 0.0 && (one_sim > 0))
	{
		prtls_pos_vel[vel_ind[1]] = cur_vy;
		prtls_pos_vel[pos_ind[1]] = cur_py;
	}
	// checks if any of its neighbours has a velocity of 0 in the y direction
	double self_vy = set_self_yvel_zero(k);
	if (self_vy == 0 && (one_sim > 0))
	{
		prtls_pos_vel[vel_ind[1]] = self_vy;
	}

}


double ParticleSimulator::set_self_yvel_zero(int prtl_i)
{
	std::vector<double> nei_vel{ -1.0, -1.0, -1.0 };
	double self_vy = -1.0;
	int lk_offset = prtl_i * (7 + sim_num_prtls) + 7;
	int self_pos = 0;
	for (int j = 0; j < sim_num_prtls; j++)
	{
		if (j == 0) 
		{
			self_pos = prtl_i * (7 + sim_num_prtls);
		}
		if (prtls_pos_vel[lk_offset + j] >= 0) // this offset has a neighbour
		{
			// (int) prtls_pos_vel[lk_offset + j] = num/name of particle 0,1..
			int p_name = (int)(prtls_pos_vel[lk_offset + j]);
			int nei_pos = p_name * (7 + sim_num_prtls);
			if (prtls_pos_vel[nei_pos + 4] == 0.0) // y-axis velocity
			{
				self_vy = 0.05 * prtls_pos_vel[self_pos + 4];
				
				if (self_vy < 0.001)
				{
					self_vy = 0.0;
				}
				if ((self_vy < 0) && (self_vy > (- 1.0 * 0.001)))
				{
					self_vy = 0.0;
				}
				if (thisTime > 89.5)
				{
					double check_vy = 0.0;
				}
				return self_vy;
			}
		}
	}
	return self_vy;
}

double ParticleSimulator::ground_collision_forces(std::vector<int> indices, double prev_vy)
{
	Vector ptl_now;
	Vector vel_now;
	double pnlty_y;


	double px, py, pz, vx, vy, vz;
	px = prtls_pos_vel[indices[0]];
	py = prtls_pos_vel[indices[1]];
	pz = prtls_pos_vel[indices[2]];
	vx = prtls_pos_vel[indices[3]];
	vy = prtls_pos_vel[indices[4]];
	vz = prtls_pos_vel[indices[5]];

	double t_vy = prev_vy;

	if (t_vy < 0.0)
	{
		t_vy = -1.0 * t_vy;
	}

	setVector(ptl_now, px, py, pz);
	setVector(vel_now, vx, vy, vz);

	setVector(point_on_xz, 2.0, 0.0, 3.0);
	setVector(xz_plane_norm, 0.0, 1.0, 0.0);
	Vector check_pn;
	Vector lt;
	Vector rgt_side;
	Vector fn;
	zeroVector(lt);
	zeroVector(rgt_side);
	zeroVector(fn);

	zeroVector(check_pn);
	VecSubtract(check_pn, ptl_now, point_on_xz);
	VecCopy(lt, check_pn);
	// (p_pos - pt_on_plane)dot(N_of_plane)
	double pntr_cond = VecDotProd(check_pn, xz_plane_norm);

	if (thisTime > 30.3)
	{
		double check_pn = 0.0;
	}

	if (pntr_cond < 0.01)
	{
		/* Left side of penalty force equation*/
		double lt_coef = -1.0 * ground_ks;
		// -k_s((p_pos - pt_on_plane)dot(N_of_plane))
		lt_coef = lt_coef * pntr_cond;
		Vector lft_side;
		VecCopy(lft_side, xz_plane_norm);
		// (-k_s(x.._i - P)dot(N..))N..
		VecScale(lft_side, lt_coef);

		/* Right side of penalty force equation */
		double rht_coeff = -1.0 * ground_kd;
		// v.._i<dot>(N)
		double n = VecDotProd(vel_now, xz_plane_norm);
		rht_coeff = rht_coeff * n;
		VecCopy(rgt_side, xz_plane_norm);
		// (-k_d * (v.._i * N..))*N.., rgt_side = N
		VecScale(rgt_side, rht_coeff);
		// -ks((x_i-p)dot(N))*N + -kd((v_i)dot(N))N
		VecAdd(fn, lft_side, rgt_side);
		pnlty_y = VecDotProd(fn, xz_plane_norm);

		double t_pnlty = pnlty_y;

		if (t_pnlty < 0.0) { t_pnlty = -1.0 * t_pnlty; }
		
		if (thisTime > 30.15)
		{
			double check_y = 0.0;
		}

		if (pnlty_y > (0.2 * t_vy))
		{
			/*double pCheck = 0.0;*/
			
			if (t_vy < 1)
			{
				pnlty_y = 0.5 * t_vy;
			}
			else
			{
				pnlty_y = 0.2 * t_vy;
			}
			if (pnlty_y < t_pnlty)
			{
				if (thisTime > 40.0) 
				{ 
					double cur_t = 0.0;
				}
				
				pnlty_y = 0.0;
			}
		}
		

		return pnlty_y;
	}
	else
	{
		// particle did not fall through the ground. -1.0 indicate no change in py.
		return -1.0;
	}

}


int ParticleSimulator::step(double time)
{
	int i_idx, j_idx;
	int num_ptrls;

	if (m_object)
	{
		//prtls_pos_vel = [px, py, pz, vx, vy, vz] for all particles
		m_object->getState(prtls_pos_vel);
		if (reset_sim == 1)
		{
			reset_sim = 0;
		}
		// True number of particles not size of prtls_pos_vel
		num_ptrls = sim_num_prtls;

		/*initialize prev positions once for verlet integration*/
		if (negative_time) // negative_time set to zero after the euler step in verlet
		{
			previous_pos = new Vector[num_ptrls];
			for (int i = 0; i < num_ptrls; i++)
			{
				zeroVector(previous_pos[i]);
			}
			/*NOTE: must do this ->, negative_time = 0; , in verlet once we have used euler to calculate for x(t + dt) at t = 0*/
		}
		/*summation of the spring and damping forces on a particle*/
		summed_forces = new Vector[num_ptrls];
		for (int i = 0; i < num_ptrls; i++)
		{
			zeroVector(summed_forces[i]);
		}
		std::vector<int> offSet;

		// Used for resting the sim
		if (init_done == 0)
		{
			init_done = 1;
			int ptrl_indices = 7 * num_ptrls;
			for (int i = 0; i < ptrl_indices; i++)
			{
				double pos_vel = prtls_pos_vel[i];
				init_pos_vel.push_back(pos_vel);
			}
			// used for adding spring links to the prtls_pos_vel channel.
			init_links();
		}

		// calculate the forces spring k  puts on each particle.
		for (int k = 0; k < springs_count; k++)
		{
			Vector ptrl_i_pos;
			Vector prtl_j_pos;
			Vector prtl_i_vel;
			Vector ptl_j_vel;
			Vector dst_ij;
			Vector vel_ij;
			Vector dm_frc_ij;
			Vector dm_frc_ji;
			Vector sp_frc_ij;
			Vector sp_frc_ji;
			Vector drag_frc_i;
			Vector drag_frc_j;
			double dst_len;

			i_idx = spr_i_and_j[k][0]; // ptl i of spring k
			j_idx = spr_i_and_j[k][1]; // ptl j of sprink k
			// prtls_pos_vel[0] = p0 p0 p0 v0 v0 v0 m_0 w0 w1. w_i = value/name of particle 0,1,..
			if (links_offset_init == 0)
			{
				if (k == springs_count - 1)
				{
					links_offset_init = 1;
				}
				// 7 = px py pz vx vy vz m_ptrl
				int linkOffSet = (i_idx * (7 + num_ptrls)) + (j_idx + 7);
				if (prtls_pos_vel[linkOffSet] == -1)
				{
					prtls_pos_vel[linkOffSet] = j_idx;
				}
			}
			
			if (thisTime > 6.7)
			{
				double checkTime = 0.0;
			}
			std::vector<double> sp_properties = sp_ks_kd_rstLen[k];

			/*get and set POSITION of particle i*/

			offSet = get_ptrl_offset(i_idx);
			setVector(ptrl_i_pos, prtls_pos_vel[offSet[0]], prtls_pos_vel[offSet[1]], prtls_pos_vel[offSet[2]]);
			/*get and set VELOCITY of particle i*/
			setVector(prtl_i_vel, prtls_pos_vel[offSet[3]], prtls_pos_vel[offSet[4]], prtls_pos_vel[offSet[5]]);
			/*drag force on particle i*/
			VecCopy(drag_frc_i, prtl_i_vel);
			VecScale(drag_frc_i, sim_drag_const);

			/*get and set POSITION of particle j*/
			offSet = get_ptrl_offset(j_idx);
			setVector(prtl_j_pos, prtls_pos_vel[offSet[0]], prtls_pos_vel[offSet[1]], prtls_pos_vel[offSet[2]]);
			/*get and set VELOCITY of particle i*/
			setVector(ptl_j_vel, prtls_pos_vel[offSet[3]], prtls_pos_vel[offSet[4]], prtls_pos_vel[offSet[5]]);
			/*drag force on particle j*/
			VecCopy(drag_frc_j, ptl_j_vel);
			VecScale(drag_frc_j, sim_drag_const);

			// Simplified damper force (ks, kd, rstLen)
			double ks = sp_properties[0];
			double kd = sp_properties[1];
			double rst_ln = sp_properties[2];

			/*NOTE: start of Simplified damper force*/
			// x_i - x_j
			VecSubtract(dst_ij, ptrl_i_pos, prtl_j_pos);
			// vel_ij = vi - vj
			VecSubtract(vel_ij, prtl_i_vel, ptl_j_vel);
			// W = kd_ij*(vel_ij = vi - vj)
			VecScale(vel_ij, kd);
			// Fd_ij = -1.0 * W. W = damper force with +pos coeff
			VecScale(vel_ij, -1.0);
			VecCopy(dm_frc_ij, vel_ij);
			/*summation of damping force on particle i*/
			VecAdd(summed_forces[i_idx], summed_forces[i_idx], dm_frc_ij);
			// damper force on ptl j
			VecCopy(dm_frc_ji, vel_ij);
			VecScale(dm_frc_ji, -1.0);
			/*summation of damping force on particle j*/
			VecAdd(summed_forces[j_idx], summed_forces[j_idx], dm_frc_ji);
			/* end of damper force*/

			/* start of spring force*/
			// Spring force on plt ij, Fs_ij
			double sp_frc_coeff;
			double norm_len;
			// mag(x.._i - x..j)
			dst_len = VecLength(dst_ij);
			// 1 / mag(x.._i - x..j) coeff
			norm_len = 1.0 / dst_len;

			if (rst_ln < 0)
			{
				rst_ln = dst_len;
			}
			sp_frc_coeff = ks * (rst_ln - dst_len);
			// x.. means x is a vector
			// (1/mag(pos_i - pos_j)) * (pos.._i - pos.._j)
			VecScale(dst_ij, norm_len);
			// sp_coeff = ks_ij*(rst_ln - mag(pos.._i - pos.._j)
			VecScale(dst_ij, sp_frc_coeff);
			// Set Fsp_ij, spring force on particle i
			VecCopy(sp_frc_ij, dst_ij);
			/*summation of spring force on particle i*/
			VecAdd(summed_forces[i_idx], summed_forces[i_idx], sp_frc_ij);
			// Scale dst_ij by -1, spring force on particle j
			VecScale(dst_ij, -1.0);
			VecCopy(sp_frc_ji, dst_ij);
			/*summation of spring force on particle i*/
			VecAdd(summed_forces[j_idx], summed_forces[j_idx], sp_frc_ji);
		}
		
		thisTime = time;
		for (int n = 0; n < num_ptrls; n++)
		{
			/*if fixed_indices[n] > 0, then skip the particle at index k and don't change it's position or velocity.*/
			if (fixed_indices[n] < 0)
			{
				Vector nth_prtl_gf;
				setVector(nth_prtl_gf, 0.0, 1.0, 0.0);
				if (n == 0 && (thisTime > 5.3))
				{
					double checkT = 0.0;
				}
				offSet = get_ptrl_offset(n);
				/*add all forces for particle n*/
				double m = prtls_pos_vel[offSet[6]];
				// Scale gravity by particle i's mass
				Vector nth_prtl_v;
				Vector drag_frc_k;
				/*get and set VELOCITY of particle n*/
				setVector(nth_prtl_v, prtls_pos_vel[offSet[3]], prtls_pos_vel[offSet[4]], prtls_pos_vel[offSet[5]]);
				VecCopy(drag_frc_k, nth_prtl_v);
				// k_dr * v.._i, x.. is a vector.
				VecScale(drag_frc_k, sim_drag_const);
				// -1.0 * k_dr * v.._i
				VecScale(drag_frc_k, -1.0);
				if (thisTime > 0.01)
				{
					double checkT = 0.0;
				}
				// if mot_gr = -9.8, then nth_prtl_gf = (0.0, -9.8, 0.0)
				VecScale(nth_prtl_gf, sim_gravity);
				// m_i * g..
				VecScale(nth_prtl_gf, m);
				Vector frcs_on_ptrl_n;
				// summed forces on particle n 
				Vector summed_frcs_n;

				// set to: (-1.0 * k_dr * v.._i) + (m_i * g..)
				VecAdd(frcs_on_ptrl_n, drag_frc_k, nth_prtl_gf);
				VecCopy(summed_frcs_n, summed_forces[n]);
				// frcs_on_ptrl_n <- sigma(F_sp + F_d) on ptl_i.
				VecAdd(frcs_on_ptrl_n, frcs_on_ptrl_n, summed_frcs_n);

				

				double px, py, pz, vx, vy, vz;
				px = prtls_pos_vel[offSet[0]];
				py = prtls_pos_vel[offSet[1]];
				pz = prtls_pos_vel[offSet[2]];
				vx = prtls_pos_vel[offSet[3]];
				vy = prtls_pos_vel[offSet[4]];
				vz = prtls_pos_vel[offSet[5]];

				

				if (thisTime >= 3.58 && (n == 0))
				{
					double close_to = 0;
				}
				if (integr_method == "symplectic")
				{
					symplectic_euler(offSet, frcs_on_ptrl_n, m, thisTime, n);
					/*if (new_t >= 2.0 && (new_t < 4.0))
					{
						animTcl::OutputMessage("time: %lf py: %lf", new_t, py);
					}*/
				}
				else if (integr_method == "euler")
				{
					euler_method(offSet, frcs_on_ptrl_n, m, thisTime, n);
				}
				else
				{
					if (negative_time)
					{
						// set previous_pos to position of t = 0;
						setVector(previous_pos[n], prtls_pos_vel[offSet[0]], prtls_pos_vel[offSet[1]], prtls_pos_vel[offSet[2]]);
						// use euler_method to get x(t1)
						euler_method(offSet, frcs_on_ptrl_n, m, thisTime, n);
						// indicates to use verlet method next time. 2 particles: n0, n1. 2-1 = 1.
						if (n == (num_ptrls - 1))
						{
							negative_time = 0;
						}
					}
					else
					{
						verlet_method(offSet, frcs_on_ptrl_n, m, thisTime, n);
					}

				}
			}

		}
		if (one_sim == 0)
		{
			one_sim = 1;
		}

		/*update position and velocity*/
		// Need to use/send a vector with the spring links indexing set.
		/*assert(false);*/
		m_object->setState(prtls_pos_vel);
		/*m_object->setState(prtls_pos_vel);*/
	}
	else
	{
		animTcl::OutputMessage("The simulator must be linked to a non-empyt system");
	}
	return 0;
}

void ParticleSimulator::reset(double time)
{
	for (int i = 0; i < init_pos_vel.size(); i++)
	{
		prtls_pos_vel[i] = init_pos_vel[i];
	}
	negative_time = 1;
	one_sim = 0;
	m_object->setState(prtls_pos_vel);
	reset_sim = 1;
}

void ParticleSimulator::setSimSystem()
{
	// retrieve the system
	int m_object_null = 0;
	if (m_object)
	{
		m_object_null = 1;
	}
	else
	{
		m_object = GlobalResourceManager::use()->getSystem(sim_system_name);


		std::string t_name = m_object->retrieveName();
	}

}



int ParticleSimulator::command(int argc, myCONST_SPEC char** argv)
{
	if (argc < 1)
	{
		animTcl::OutputMessage("No Commands were given.");
	}
	else if (strcmp(argv[0], "link") == 0)
	{
		if (argc < 3)
		{
			return TCL_ERROR;
		}
		else
		{
			sim_system_name = argv[1];
			// number of springs
			num_of_sp = (int)atof(argv[2]);
			// link simulator to BaseSystem named with system_name
			setSimSystem();
			sp_ks_kd_rstLen = new std::vector<double>[num_of_sp];
			/*sp_names = new std::string[num_of_sp];*/
			spr_i_and_j = new std::vector<int>[num_of_sp];

			spring_forces = new Vector[num_of_sp];
			damper_forces = new Vector[num_of_sp];
			drag_forces = new Vector[num_of_sp];

			/*No easy way to get #of_ptls from sim class:
			2 * 7(px,py,pz,vx,vy,vz, mass) * num_of_sp*/

			
			if (system_num_ptrls == -1)
			{
				double* num_prtls_retrv;
				num_prtls_retrv = new double[1];
				num_prtls_retrv[0] = 1010.012345;
				/*num_prtls_retrv[1] = 1010.012345;*/
				m_object->getState(num_prtls_retrv);
				sim_num_prtls = (int)num_prtls_retrv[0];
				system_num_ptrls = 1;
			}
			/*int ptrl_properties_count = (14 * num_of_sp) + 2;*/
			int ptrl_properties_count = sim_num_prtls * (7 + sim_num_prtls);
			prtls_pos_vel = new double[ptrl_properties_count];
			fixed_indices = new int[sim_num_prtls];
			for (int i = 0; i < sim_num_prtls; i++)
			{
				// particles don't have negative indices
				fixed_indices[i] = -1;
			}
			animTcl::OutputMessage("Number of springs is: %d", num_of_sp);
		}
	}
	else if (strcmp(argv[0], "spring") == 0)
	{
		if (argc < 6)
		{
			return TCL_ERROR;
		}
		else
		{
			int ind_i = (double)atof(argv[1]);
			int ind_j = (double)atof(argv[2]);
			double ks = (double)atof(argv[3]);
			double kd = (double)atof(argv[4]);
			double rst_len = (double)atof(argv[5]);
			if (springs_count < num_of_sp)
			{
				/*sp_names[springs_count] = spring_name;*/
				spr_i_and_j[springs_count].push_back(ind_i);
				spr_i_and_j[springs_count].push_back(ind_j);
				sp_ks_kd_rstLen[springs_count].push_back(ks);
				sp_ks_kd_rstLen[springs_count].push_back(kd);
				sp_ks_kd_rstLen[springs_count].push_back(rst_len);
				springs_count += 1;
			}
			else
			{
				/*assert(false);*/
				animTcl::OutputMessage("World spring index out of bounds");
				return TCL_ERROR;
			}
		}
	}
	else if (strcmp(argv[0], "fix") == 0)
	{
		if (argc < 2)
		{
			animTcl::OutputMessage("ground command: simulator <sim_name> ground <ks> <kd>");
			return TCL_ERROR;
		}
		else
		{
			int fix_i = (int)atof(argv[1]);
			fixed_indices[fix_i] = 3;
		}
	}
	else if (strcmp(argv[0], "integration") == 0)
	{
		if (argc < 3)
		{
			animTcl::OutputMessage("integration command: simulator <sim_name> integration <euler|symplectic|verlet> <time step>");
			return TCL_ERROR;
		}
		else
		{
			integr_method = argv[1];
			dlt_t = (double)atof(argv[2]);
		}
	}
	else if (strcmp(argv[0], "ground") == 0)
	{
		if (argc < 3)
		{
			animTcl::OutputMessage("ground command: simulator <sim_name> ground <ks> <kd>");
			return TCL_ERROR;
		}
		else
		{
			double ks = (double)atof(argv[1]);
			double kd = (double)atof(argv[2]);

			ground_ks = ks;
			ground_kd = kd;
		}
	}
	else if (strcmp(argv[0], "gravity") == 0)
	{
		if (argc < 2)
		{
			animTcl::OutputMessage("gravity command: simulator <sim_name> gravity g");
			return TCL_ERROR;
		}
		else
		{
			double g = (double)atof(argv[1]);
			sim_gravity = g;
		}
	}
	else if (strcmp(argv[0], "drag") == 0)
	{
		if (argc < 2)
		{
			animTcl::OutputMessage("drag command: simulator <sim_name> drag kdrag");
			return TCL_ERROR;
		}
		else
		{
			double glb_value = (double)atof(argv[1]);
			sim_drag_const = glb_value;
		}
	}
	else
	{
		if (strcmp(argv[0], "other") == 0)
		{
			animTcl::OutputMessage("Simulator. All other!");
		}
	}


	return TCL_OK;
}
