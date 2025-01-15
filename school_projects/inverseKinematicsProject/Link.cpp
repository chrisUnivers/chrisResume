#include "Link.h"

/// <summary>
/// Construct a link with two ends. There are many ways to do this. Here I am showing an example of usign shared pointers
/// The Link class has a parent and child reference to "Particles" really it is where the joints are. This is maybe overcomplicated 
/// for simple examples but again, there are many solutions. You can think of this one as representing a link as a start point and an end point
/// </summary>
/// <param name="name"></param>
/// <param name="p"></param>
/// <param name="c"></param>
Link::Link(const std::string& name, ParticleRef p, ParticleRef c)
    : BaseSystem(name), parent(p), child(c) {
    length = abs(distance(p->getPosition(), c->getPosition()));
}

void Link::init(vec3 l_start, vec3 l_end, int id) {
    parent->init(l_start);
    child->init(l_end);
    link_id_ = id;
    length = abs(distance(parent->getPosition(), child->getPosition()));
}


/// <summary>
/// This allows us to access the end points of the links easily!
/// </summary>
/// <param f_delt_z> the final delta in the z plane</param>
/// <returns></returns>
void Link::DrawCirlcle(float h, float k, float rx, float ry, float rz, int num_seg)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < num_seg; i++)
    {
        float theta = 2.0f * 3.1315926f * float(i) / float(num_seg);
        float delt_x = h - (rx * cosf(theta));
        float delt_y = k - (ry * sinf(theta));
        /*float new_x = (cosf(theta) * delt_x) - (sinf(theta) * delt_y);
        float new_y = (sinf(theta) * delt_x) + (cosf(theta) * delt_y);*/
        float new_z = 1.3 * cosf(1.0 * theta);
        if (new_z < -1.25)
        {
            int check_t = 0;
        }
        /*glVertex2f(ex + xe, ey + ne);*/
        glVertex3f(delt_x - rx, delt_y, rz);
    }
    glEnd();
}

void Link::display(GLenum mode) {
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_MODELVIEW);

    vec3 start = parent->getPosition();
    vec3 end = child->getPosition();

    float r, g, b;
    r = ((float)rand() / (float)RAND_MAX);
    g = ((float)rand() / (float)RAND_MAX);
    b = ((float)rand() / (float)RAND_MAX);

    glLineWidth(5.0);
    glColor3f(0.0, 0.0, 0.0);
    if (link_id_ == 1)
    {
        glColor3f(0.34, 0.71, 0.52);
    }
    else
    {
        glColor3f(0.0, 0.0, 0.0);
    }
    glBegin(GL_LINES);
    glVertex3f(start.x, start.y, start.z);
    glVertex3f(end.x, end.y, end.z);
    glEnd();

    glColor3f(0.97, 0.67, 0.67);
    float bx, by, bz;

    if (link_id_ == 0)
    {
        bx = 0.0;
        by = 0.0;
        bz = 0.5;
        /*DrawCirlcle(bx, by, 0.7, 100, bz);*/
    }
    if (link_id_ == 1)
    {
        /*bx = (end.x - start.x) * 0.5f;
        by = (end.y - start.y) * 0.5f;*/
        bx = 0.0;
        by = 0.0;
        bz = end.z - start.z;
        int num_seg = 100;
        float h, k, rx, ry, rz;
        rx = 0.4; ry = 1.6; rz = start.z; h = 1.5; k = 1.0;
        DrawCirlcle(h, k, rx, ry, rz, 100);
    }

    // board to write on
    glColor3f(0.274, 0.274, 0.274);
    glBegin(GL_POLYGON);
    glVertex3f(-5, -3, -0.2);
    glVertex3f(5, -3, -0.2);
    glVertex3f(5, 3, -0.2);
    glVertex3f(-5, 3, -0.2);
    glEnd();
    // white background
    glColor3f(0.8, .75, 0.75);
    glBegin(GL_POLYGON);
    glVertex3f(-8, -8, -1);
    glVertex3f(8, -8, -1);
    glVertex3f(8, 8, -1);
    glVertex3f(-8, 8, -1);
    glEnd();

    // ground
    glColor3f(0.98, 0.82, 0.36);
    glBegin(GL_POLYGON);
    glVertex3f(-8, -8.0, 8.0);
    glVertex3f(8, -8.0, 8.0);
    glVertex3f(8.0, -8.0, 0.0);
    glVertex3f(-8, -8.0, 0.0);
    glEnd();
}
