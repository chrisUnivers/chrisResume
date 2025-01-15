#include "BaseSystem.h"
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include <GLmodel/GLmodel.h>

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "shared/opengl.h"


#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(const std::string& name) : BaseSystem(name)
{
	/* Initialize all the particle positions and velocities*/
}


void ParticleSystem::getState(double* p_pos_vel)
{
	/* set memory of p_pos_vel to: [px, py, pz, vx, vy, vz, m_i]*/
	int t_index = 0;
	int s_index = 0;
	int count_pos = 0;
	if (all_prtls_pos)
	{
		if (p_pos_vel)
		{
			// if the simulator does not know the number of particles in the system
			if (sim_num_prtls_set == 0 && (p_pos_vel[0] == 1010.012345))
			{
				sim_num_prtls_set = 1;
				p_pos_vel[0] = (double)num_prtls;
			}
			else
			{
				for (int i = 0; i < num_prtls; i++)
				{
					int k = i * (7 + num_prtls);
					t_index = k + 6;
					Vector pos_values;
					Vector vel_values;
					VecCopy(pos_values, all_prtls_pos[i]);
					p_pos_vel[k] = pos_values[0]; // px
					p_pos_vel[k + 1] = pos_values[1]; // py
					p_pos_vel[k + 2] = pos_values[2]; // pz
					VecCopy(vel_values, all_prtls_vel[i]);
					p_pos_vel[k + 3] = vel_values[0]; //vx
					p_pos_vel[k + 4] = vel_values[1]; //vy
					p_pos_vel[k + 5] = vel_values[2]; //vz
					p_pos_vel[k + 6] = all_prtls_mass[i]; // mass
				}
				// a way to know number of particles in the simulator class.
				/*t_index += 1;
				p_pos_vel[t_index] = 1010.012345;*/
			}

		}
	}


}

void ParticleSystem::setState(double* new_pos_vel)
{
	std::vector<double> links_set;
	for (int i = 0; i < num_prtls; i++)
	{
		int k = i * (7 + num_prtls);
		Vector pos_i;
		Vector vel_i;

		double px_new, py_new, pz_new, vx_new, vy_new, vz_new;


		px_new = new_pos_vel[k]; //px
		py_new = new_pos_vel[k + 1]; //py
		pz_new = new_pos_vel[k + 2]; //pz
		links_set.push_back(px_new);
		links_set.push_back(py_new);
		links_set.push_back(pz_new);
		if (py_new < 0.0)
		{
			py_new = 0.0;
		}
		setVector(pos_i, px_new, py_new, pz_new);
		VecCopy(all_prtls_pos[i], pos_i);

		vx_new = new_pos_vel[k + 3]; //vx
		vy_new = new_pos_vel[k + 4]; //vy
		vz_new = new_pos_vel[k + 5]; //vz
		// set velocity
		setVector(vel_i, vx_new, vy_new, vz_new);
		VecCopy(all_prtls_vel[i], vel_i);

		// set up spring connections.
		if (sys_links_vec_init == 0)
		{
			sys_links_vec_init = 1;
			init_system_links_vec();
		}
		// spring connections also set once.
		if (system_link_init == 0)
		{
			int lk_offset = i * (7 + num_prtls) + 7;
			for (int j = 0; j < num_prtls; j++)
			{
				if (new_pos_vel[lk_offset + j] >= 0)
				{
					// new_pos_vel[lk_offset + j] = num/name of particle 0,1..
					int p_name = (int)(new_pos_vel[lk_offset + j]);
					sys_links_names[i].push_back(p_name);
				}
			}
		}

	}
	if (system_link_init == 0)
	{
		system_link_init = 1;
		dsp_links_ok = 1;
		links_set.push_back(-1.0);
	}
}
void ParticleSystem::init_system_links_vec()
{
	for (int i = 0; i < num_prtls; i++)
	{
		std::vector<int> t{};
		sys_links_names.push_back(t);
	}
}


void ParticleSystem::reset(double time)
{
	setVector(m_pos, 0, 0, 0);
}

void ParticleSystem::init_velocities(double uvx, double uvy, double uvz)
{
	setVector(temp_vec, uvx, uvy, uvz);
	for (int i = 0; i < num_prtls; i++)
	{
		// set this particles velocity
		VecCopy(all_prtls_vel[i], temp_vec);
	}

}


int ParticleSystem::command(int argc, myCONST_SPEC char** argv)
{

	if (argc < 1)
	{
		animTcl::OutputMessage("No Commands were given.");
	}
	else if (strcmp(argv[0], "dim") == 0)
	{
		if (argc < 2)
		{
			return TCL_ERROR;
		}
		else
		{
			num_prtls = (int)atof(argv[1]);
			animTcl::OutputMessage("num of all particles: %d", num_prtls);
			all_prtls_pos = new Vector[num_prtls];
			all_prtls_vel = new Vector[num_prtls];
			all_prtls_mass = new double[num_prtls];
		}
	}
	else if (strcmp(argv[0], "particle") == 0)
	{
		if (argc < 9)
		{
			return TCL_ERROR;
		}
		else
		{
			prtl_index = (int)atof(argv[1]);
			if (prtl_index < num_prtls)
			{
				prtl_mass = (double)atof(argv[2]);
				px = (double)atof(argv[3]);
				py = (double)atof(argv[4]);
				pz = (double)atof(argv[5]);
				vx = (double)atof(argv[6]);
				vy = (double)atof(argv[7]);
				vz = (double)atof(argv[8]);

				// set this particles position
				setVector(temp_vec, px, py, pz);
				VecCopy(all_prtls_pos[prtl_index], temp_vec);
				// set this particles velocity
				setVector(temp_vec, vx, vy, vz);
				VecCopy(all_prtls_vel[prtl_index], temp_vec);
				// set this particles mass
				all_prtls_mass[prtl_index] = prtl_mass;
				//
				glutPostRedisplay();
			}
			else
			{
				animTcl::OutputMessage("The given index must be less than the number of <dim>");
			}
		}
	}
	else if (strcmp(argv[0], "all_velocities") == 0)
	{
		if (argc < 4)
		{
			return TCL_ERROR;
		}
		else
		{
			double ax = (double)atof(argv[1]);
			double ay = (double)atof(argv[2]);
			double az = (double)atof(argv[3]);
			init_velocities(ax, ay, az);
		}
	}
	else
	{
		animTcl::OutputMessage("Command not supported");
	}

	return TCL_OK;
}

void ParticleSystem::displayParticle(Vector p, float r)
{
	glPointSize(r);
	glBegin(GL_POINTS);
	glVertex3dv(p);
	glEnd();
}

void ParticleSystem::displayLinks() {
	
	float line_r = 1.5;
	glLineWidth(line_r);

	glBegin(GL_LINE_STRIP);
	
	for (int i = 0; i < num_prtls; i++)
	{
		std::vector<int> neighbour_vec = sys_links_names[i];
		for (int j = 0; j < neighbour_vec.size(); j++)
		{
			int nei_init = neighbour_vec[j];
			float cRed = 0.0;
			glColor3f(cRed, cRed, cRed);
			glVertex3f(all_prtls_pos[i][0], all_prtls_pos[i][1], all_prtls_pos[i][2]);
			glVertex3f(all_prtls_pos[nei_init][0], all_prtls_pos[nei_init][1], all_prtls_pos[nei_init][2]);
		}
	}
	glEnd();
}

void ParticleSystem::display(GLenum mode) {
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_COLOR_MATERIAL);

	float pRed = 0.0;
	float pGreen = 0.0;
	float pBlue = 0.0;

	for (int i = 0; i < num_prtls; i++)
	{
		double scale = 0.0;
		glColor3f(pRed, pGreen, pBlue);
		displayParticle(all_prtls_pos[i], 10.0);
		pGreen += 0.10375;
		pRed += 0.5;
		pBlue += pRed / 3.0;
	}
	if (dsp_links_ok == 1)
	{
		displayLinks();
	}

	glPopAttrib();
}