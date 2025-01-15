#pragma once

#include <shared/defs.h>
#include <util/util.h>
#include <GLmodel/GLmodel.h>


#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "animTcl.h"
#include "BaseSystem.h"
#include "BaseSimulator.h"
#include "GlobalResourceManager.h"

#include "shared/opengl.h"

class ParticleSimulator : public BaseSimulator
{
public:
	ParticleSimulator(const std::string& name, BaseSystem* target);
	~ParticleSimulator();

	int step(double time);
	void reset(double time);
	int init(double time)
	{
		return 0;
	};

	void setSimSystem();
	std::string indices_to_strings(int a, int b);
	std::vector<int> get_ptrl_offset(int k);
	// Get the number of particles * 6 from the simulation class. See definition for more info.
	void symplectic_euler(std::vector<int> indices, Vector frc, double m, double euler_t, int prtl_i);
	void euler_method(std::vector<int> indices, Vector frc, double m, double euler_t, int prtl_i);
	void verlet_method(std::vector<int> indices, Vector frc, double m, double euler_t, int prtl_i);
	void init_links();
	/*std::vector<double> get_neighbours_vel_zeros(int prtl_i);*/
	double set_self_yvel_zero(int prtl_i);
	double ground_collision_forces(std::vector<int> indices, double prev_y);
	

	int command(int argc, myCONST_SPEC char** argv);

protected:
	
	Vector m_vec;
	//int num_of_all_motios;
	int num_of_sp; // the number of all springs
	int fix_all_pts = 0;
	int springs_count= 0; // used when adding spring names
	int negative_time = 1;
	int reset_sim = 0;
	int init_done = 0;
	int links_set = 0;
	int links_offset_init = 0;
	int system_num_ptrls = -1;
	int sim_num_prtls = 0;
	int one_sim = 0;
	
	double dlt_t;
	double ground_ks;
	double ground_kd;
	// simulator's gravity
	double sim_gravity = 0.0;
	// simulator's drag
	double sim_drag_const = 0.0;

	double thisTime = 0.0;

	double* prtls_pos_vel;
	std::vector<double> init_pos_vel;
	int bounce = 0;

	/*double* all_ptl_forces;*/

	/*store previous position*/
	Vector* previous_pos = NULL;
	Vector point_on_xz;
	Vector xz_plane_norm;

	// point to array of spring rest lengths
	/*double* sp_restLens = NULL;*/
	std::vector<double>* sp_ks_kd_rstLen = NULL;
	// give the 'global' indices of 2 connected particles
	std::vector<int>* spr_i_and_j = NULL;
	
	// array of fixed indices
	int* fixed_indices;

	// pointer to array of springs' names
	/*std::string* sp_names = NULL;*/
	
	std::string integr_method = "";
	// Holds the positions of the motions
	/*Vector* all_mot_pos = NULL; */
	// Holds the velocities of the motions
	/*Vector* all_mot_vel = NULL;*/

	// Holds the spring force for all springs
	Vector* spring_forces = NULL;
	// Holds the damper force for all springs
	Vector* damper_forces = NULL;
	// Holds the drag force for all springs
	Vector* drag_forces = NULL;

	// Holds the drag force for all springs
	Vector* summed_forces = NULL;
	

	std::string sim_system_name = "";
	BaseSystem* m_object;
	double prev_time;
	
};