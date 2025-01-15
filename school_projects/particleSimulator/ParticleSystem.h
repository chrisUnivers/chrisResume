#pragma once
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


class ParticleSystem : public BaseSystem
{
public:
	ParticleSystem(const std::string& name);
	virtual void getState(double* p);
	virtual void setState(double* p);
	void reset(double time);

	void init_velocities(double uvx, double uvy, double uvz);
	void ParticleSystem::displayParticle(Vector p, float r);
	void displayLinks();
	void init_system_links_vec();
	void display(GLenum mode = GL_RENDER);

	int command(int argc, myCONST_SPEC char** argv);

protected:

	Vector m_pos;
	int prtl_index = -1;
	int init_all = 0;
	int sim_num_prtls_set = 0;
	int system_link_init = 0;
	int sys_links_vec_init = 0;
	int dsp_links_ok = 0;

	double prtl_mass = -1.0;
	double px, py, pz;
	double vx, vy, vz;
	Vector temp_vec;
	/*int num_particle = 8;*/
	int num_prtls;
	// the positions of the particles
	Vector* all_prtls_pos = NULL;
	/*Vector* all_particles_pos;*/
	// the velocities of the particles
	Vector* all_prtls_vel = NULL;
	// the mass of the particles
	double* all_prtls_mass = NULL;
	/*std::vector<int>* sys_spr_links = NULL;*/
	std::vector<std::vector<int>> sys_links_names;
};