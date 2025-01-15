#include "Particle.h"


/// <summary>
/// Really we are using this for rendering the joints as end points of the links. Otherwise it doesnt do much!
/// </summary>
/// <param name="name"></param>
Particle::Particle(const std::string& name)
    : BaseSystem(name), position_(0), color_(0, 1, 0), radius_(1) {} // Particle

Particle::Particle()
    : BaseSystem(""), position_(0), color_(0, 1, 0), radius_(1) {}

void Particle::getState(double* p) {
    p[0] = position_.x;
    p[1] = position_.y;
    p[2] = position_.z;
} // Particle::getState

void Particle::setState(double* p) {
    position_ = vec3((float)p[0], (float)p[1], (float)p[2]);
} // Particle::setState


int Particle::init(double time) {
    initial_position_ = position_;
    return 0;
}

int Particle::init(vec3 myPos) {
    initial_position_ = myPos;
    return 0;
}

void Particle::reset(double time) {
    position_ = initial_position_;
}

int Particle::step(float time_step, IntegrationMethod integration) { return 0; }

void Particle::readModel(char* fname) {
    model_.ReadOBJ(fname);
    glmFacetNormals(&model_);
    glmVertexNormals(&model_, 90);
}

int Particle::command(int argc, myCONST_SPEC char** argv) {
    if (strcmp(argv[0], "read") == 0) {
        if (argc == 2) {
            readModel(argv[1]);
        }
        else {
            animTcl::OutputMessage("Usage: read <file_name>");
            return TCL_ERROR;
        }
    }
    else if (strcmp(argv[0], "pos") == 0) {
        if (argc == 4) {
            setPosition(Particle::parse(argv[1], argv[2], argv[3]));
        }
        else {
            animTcl::OutputMessage("Usage: pos <x> <y> <z> ");
            return TCL_ERROR;
        }
    }
    else if (strcmp(argv[0], "flipNormals") == 0) {
        flipNormals();
        return TCL_OK;

    }
    else if (strcmp(argv[0], "reset") == 0) {
        setPosition(vec3(0));
    }

    glutPostRedisplay();
    return TCL_OK;
} // Particle::command

void Particle::DrawCirlcle(float h, float k, float rx, float ry, float rz, int num_seg)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < num_seg; i++)
    {
        float theta = 2.0f * 3.1315926f * float(i) / float(num_seg);
        float pi_o2 = 0.5f * 3.1315926f;
        float delt_x = h + (rx * cosf(theta));
        float delt_y = k + (ry * sinf(theta));
        /*float new_x = (cosf(theta) * delt_x) - (sinf(theta) * delt_y);
        float new_y = (sinf(theta) * delt_x) + (cosf(theta) * delt_y);*/
        /*float new_z = 3.1315926f * sinf(-1.0 * theta);*/
        float new_z = pi_o2 * cosf(0.0);
        if (new_z < -1.25)
        {
            int check_t = 0;
        }
        /*glVertex2f(ex + xe, ey + ne);*/
        float dz = 0.0;
        if ((particle_id_ == 11) || (particle_id_ == 14))
        {
            glVertex3f(delt_x, dz - 5.8, delt_y + 2.0);
        }
        else
        {
            glVertex3f(delt_x, delt_y, dz - 3.0);;
        }
    }
    glEnd();
}

void Particle::display(GLenum mode) {
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glTranslated(position_.x, position_.y, position_.z);

    if (particle_id_ == 4) // head id = 4;
    {
        int num_seg = 100;
        float h, k, rx, ry, rz;
        rx = 0.4; ry = 0.6; rz = position_.z; h = position_.x; k = position_.y;
        glColor3f(0.0, 0.0, 0.0);
        DrawCirlcle(-1.0, 5.0, rx, ry, rz, 100);
    }
    else if (particle_id_ == 5) // body id = 5;
    {
        int num_seg = 100;
        float h, k, rx, ry, rz;
        rx = 0.8; ry = 1.6; rz = position_.z; h = position_.x; k = 1.6 + 1.0;
        glColor3f(0.0, 0.0, 0.0);
        DrawCirlcle(-1.2, k, rx, ry, rz, 100);
    }
    else if (particle_id_ == 6) // right arm one = 6;
    {
        int num_seg = 100;
        float h, k, rx, ry, rz;
        rx = 0.3; ry = 1.2; rz = position_.z; h = position_.x; k = 1.8 + 1.0;
        glColor3f(0.0, 0.0, 0.0);
        DrawCirlcle(0.1, k, rx, ry, rz, 100);
    }
    else if (particle_id_ == 7) // right arm two = 7;
    {
        int num_seg = 100;
        float h, k, rx, ry, rz;
        rx = 0.3; ry = 0.8; rz = position_.z; h = position_.x; k = -0.4 + 1.0;
        glColor3f(0.0, 0.0, 0.0);
        DrawCirlcle(0.1, k, rx, ry, rz, 100);
    }
    else if (particle_id_ == 8) // right arm three = 8;
    {
        int num_seg = 100;
        float h, k, rx, ry, rz;
        rx = 0.3; ry = 0.4; rz = position_.z; h = position_.x; k = -1.75 + 1.0;
        glColor3f(0.0, 0.0, 0.0);
        DrawCirlcle(0.1, k, rx, ry, rz, 100);
    }
    else if (particle_id_ == 9) // right leg id = 9;
    {
        int num_seg = 100;
        float h, k, rx, ry, rz;
        rx = 0.3; ry = 1.25; rz = position_.z; h = position_.x; k = -1.75 + 1.25;
        glColor3f(0.0, 0.0, 0.0);
        DrawCirlcle(-0.6, k, rx, ry, rz, 100);
    }
    else if (particle_id_ == 10) // right leg id = 10;
    {
        int num_seg = 100;
        float h, k, rx, ry, rz;
        rx = 0.3; ry = 1.75; rz = position_.z; h = position_.x; k = -1.75 + -2.0;;
        glColor3f(0.0, 0.0, 0.0);
        DrawCirlcle(-0.6, k, rx, ry, rz, 100);
    }
    else if (particle_id_ == 11) // right leg id = 11;
    {
        int num_seg = 100;
        float h, k, rx, ry, rz;
        rx = 0.23; ry = 0.27; rz = position_.z; h = position_.x; k = -5.5;
        glColor3f(0.0, 0.0, 0.0);
        DrawCirlcle(-0.6, k, rx, ry, rz, 100);
    }
    else if (particle_id_ == 12) // left leg id = 12;
    {
        int num_seg = 100;
        float h, k, rx, ry, rz;
        rx = 0.3; ry = 1.25; rz = position_.z; h = -1.8; k = -1.75 + 1.25;
        glColor3f(0.0, 0.0, 0.0);
        DrawCirlcle(h, k, rx, ry, rz, 100);
    }
    else if (particle_id_ == 13) // left leg id = 13;
    {
        int num_seg = 100;
        float h, k, rx, ry, rz;
        rx = 0.3; ry = 1.75; rz = position_.z; h = -1.8; k = -1.75 + -2.0;
        glColor3f(0.0, 0.0, 0.0);
        DrawCirlcle(h, k, rx, ry, rz, 100);
    }
    else
    {
        int fsec = 0;
    }

    if (particle_id_ == 14)
    {
        int num_seg = 100;
        float h, k, rx, ry, rz;
        rx = 0.23; ry = 0.27; rz = position_.z; h = position_.x; k = -5.5;
        glColor3f(0.0, 0.0, 0.0);
        DrawCirlcle(-1.8, k, rx, ry, rz, 100);
    }
    glScalef(0.2, 0.2, 0.2);
    int locDim = 0;
    if (model_.numvertices > 0) {
        glmDraw(&model_, GLM_SMOOTH | GLM_MATERIAL);
    }
    else {
        glColor3f(color_.x, color_.y, color_.z);
        glutSolidSphere(radius_, 20, 20);
    }

    glPopMatrix();
    glPopAttrib();
} // Particle::display


/// <summary>
/// This allows us to access the end points of the links easily!
/// </summary>
/// <param name="name"></param>
/// <returns></returns>
ParticleRef Particle::create(std::string name) {
    return std::make_shared<Particle>(name);
}