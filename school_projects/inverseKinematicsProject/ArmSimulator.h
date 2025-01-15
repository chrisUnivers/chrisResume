#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <util/vectorObj.h>
#include <GL/glm/glm.hpp>

#include "BaseSimulator.h"
#include "Link.h"
#include "Particle.h"
#include "Hermite.h"



using namespace glm;

class ArmSimulator : public BaseSimulator {
public:
    ArmSimulator(const std::string& name, LinkRef link1, LinkRef link2, LinkRef link3);
    void setStart(vec3 s) { start_ = s; }
    vec3 getStart() { return start_; }
    void setEnd(vec3 e) { end_ = e; }
    vec3 getEnd() { return end_; }

    vec4 getJacobian_colOne();
    vec4 getJacobian_colTwo();
    vec4 getJacobian_colThree();
    vec4 getJacobian_colFour();
    vec4 getJacobian_colFive();
    vec4 getJacobian_colSix();
    vec4 getJacobian_colSvn();
    std::vector<vec4> computeJacobian();
    void mainCompute(int nextPoint);
    void loadSpline();
    void displaySpline(float r, int sz);

    int init(double time);
    int step(double time);
    void display(GLenum mode = GL_RENDER);

    int command(int argc, myCONST_SPEC char** argv);


protected:
    double prev_time;
    float min_r_;
    float max_r_;
    float speed_;

    float theta_1_ = 0.2118;
    float theta_2_ = 0.2338;
    float theta_3_ = 0.3511;
    float theta_4_ = 0.4326;
    float theta_5_ = 0.5961;
    float theta_6_ = 0.6181;
    float theta_7_ = 0.6411;
    float epsK = 0.005;

    vec3 l_one_start_ = vec3(0.0, 3.0, 0.0);
    vec3 l_one_end_ = vec3(-1.0, 3.0, 0.0);
    vec3 l_two_start_ = vec3(-2.15, 3.0, 0.0);
    vec3 l_two_end_ = vec3(-4.15, 3.0, 0.0);
    vec3 l_thr_start_ = vec3(-4.20, 3.0, 0.0);
    vec3 l_thr_end_ = vec3(-5.20, 3.0, 0.0);


    int nextPoint_;
    int arm_sec_ = 0;
    std::string inFileName = "";
    Hermite* hermiteSystem;


    vec3 dx_ = vec3(0.073, 0.081, 0.079);
    LinkRef link1_, link2_, link3_;
    vec3 start_, end_, origin_;
    ParticleRef control_point_;
    ParticleRef hand_;

    ParticleRef armFrame_x;
    ParticleRef armFrame_y;
    ParticleRef armFrame_z;

    std::vector<vec3> spl_points_;
    int hasSplineOK = 0;

private:
    float getTheta1(vec3 position, float theta_t);
    float getTheta2(vec3 position, float theta_t);
    void rotateLink1(vec3 position, float theta_t);
    void rotateLink2(vec3 position, float theta_t);
    void rotateLink3(vec3 position, float theta_t);
};