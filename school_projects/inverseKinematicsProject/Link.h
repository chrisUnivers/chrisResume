#pragma once

#include <memory>

#include <GL/glm/glm.hpp>

#include "BaseSystem.h"
#include "Particle.h"

using namespace glm;

typedef std::shared_ptr<class Link> LinkRef;

class Link : public BaseSystem {
public:
    Link(const std::string& name, ParticleRef p, ParticleRef c);
    void getState(double* p) {}
    void setState(double* p) {}

    vec3 getStart() { return parent->getPosition(); }
    vec3 getEnd() { return child->getPosition(); }
    vec3 getDir() { return normalize(getEnd() - getStart()); }

    void init(vec3 l_start, vec3 l_end, int id);
    void DrawCirlcle(float ex, float ey, float rx, float ry, float f_delt_z, int num_seg);
    void display(GLenum mode = GL_RENDER);

    ParticleRef parent, child;
    int link_id_ = 0;
    float length;
    
};