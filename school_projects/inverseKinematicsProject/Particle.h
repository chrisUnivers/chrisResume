#pragma once

#include <memory>

#include "BaseSystem.h"
#include "animTcl.h"
#include <GL/glm/glm.hpp>
#include <GLmodel/GLmodel.h>
#include <shared/defs.h>
#include <util/util.h>

#ifdef Success
#undef Success
#endif
#include <LNALG/eigen-3.4.0/Eigen/Dense>

#include "shared/opengl.h"


using namespace glm;

enum IntegrationMethod {
    EulerIntegration,
    SymplecticIntegration,
    VerletIntegration
};

typedef std::shared_ptr<class Particle> ParticleRef;

class Particle : public BaseSystem {
public:
    Particle(const std::string& name);
    Particle();
    virtual void getState(double* p);
    virtual void setState(double* p);

    void setPosition(vec3 p) { position_ = p; }
    vec3 getPosition() { return position_; }

    void setId(int id) { particle_id_ = id; }
    int getId() { return particle_id_; }

    void setColor(vec3 c) { color_ = c; }
    vec3 getColor() { return color_; }

    std::string getName() { return m_name; }

    int getNumPoints() { return numOfSamplePoints_; }
    void setNumPoints(int sPoints) { numOfSamplePoints_ = sPoints; }

    int init(double time);
    int init(vec3 myPos);
    void reset(double time);

    int step(float time_step, IntegrationMethod integration);

    void display(GLenum mode = GL_RENDER);
    void DrawCirlcle(float ex, float ey, float rx, float ry, float f_delt_z, int num_seg);

    void readModel(char* fname);
    void flipNormals(void) { glmReverseWinding(&model_); }
    int command(int argc, myCONST_SPEC char** argv);

    static vec3 parse(char* a, char* b, char* c) {
        return vec3(atof(a), atof(b), atof(c));
    }

    static ParticleRef create(std::string name);

protected:
    vec3 position_, initial_position_, color_;
    float radius_;
    int numOfSamplePoints_;
    int particle_id_ = 0;
    GLMmodel model_;
};