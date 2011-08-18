#include "precompiled.h"
#include "types.h"
#include "model.h"
#include "data_cache.h"
#include "draw.h"

void drawAxes(float scale)
{
    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(scale, 0.0f, 0.0f);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, scale, 0.0f);

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, scale);
    glEnd();
}

void drawLine(const cml::vector3f &p0, const cml::vector3f &p1)
{
    glBegin(GL_LINES);
        glVertex3fv(p0.data());
        glVertex3fv(p1.data());
    glEnd();
}

void drawVector(cml::vector3f origin, cml::vector3f vec)
{
    glBegin(GL_LINES);
        glVertex3fv(origin.data());
        cml::vector3f p2 = origin+vec;
        glVertex3fv(p2.data());
    glEnd();
}

void drawGrid(cml::vector3f origin, float w, float h, float spacing)
{
    float min_x = -w;
    float max_x = w;
    float min_y = -h;
    float max_y = h;

    glPushMatrix();
    glTranslatef(origin[0], origin[1], origin[2]);
    glBegin(GL_LINES);
        for(float x = min_x; x < max_x; x+=spacing)
        {
            glVertex3f(x, 0.0f, min_y);
            glVertex3f(x, 0.0f, max_y);
        }

        for(float y = min_y; y < max_y; y+=spacing)
        {
            glVertex3f(min_y, 0.0f, y);
            glVertex3f(max_y, 0.0f, y);
        }
    glEnd();
    glPopMatrix();
}

void drawPlane(const cml::vector3f &p, const cml::vector3f &n, float w, float h)
{
    cml::matrix44f_c mat;
    cml::matrix_aim_at(mat, p, p+n, cml::vector3f(1.0f, 0.0f, 0.0f), cml::axis_order_yxz);
    glPushMatrix();
    glMultMatrixf(mat.data());
    glBegin(GL_QUADS);
        glVertex3f(-w, 0.0f,  h);
        glVertex3f( w, 0.0f,  h);
        glVertex3f( w, 0.0f, -h);
        glVertex3f(-w, 0.0f, -h);
    glEnd();
    glPopMatrix();
}

void drawCube(float x, float y, float z, float scale)
{
    ModelPtr cube = DataCache::getModel("data/", "cube.obj");

    glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(scale, scale, scale);
        cube->draw();
    glPopMatrix();
}

void drawCube(const cml::vector3f &pos, float scale)
{
    drawCube(pos[0], pos[1], pos[2], scale);
}

void drawSphere(float x, float y, float z, float scale)
{
    ModelPtr cube = DataCache::getModel("data/", "sphere.obj");

    glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(scale, scale, scale);
        cube->draw();
    glPopMatrix();
}

void drawSphere(const cml::vector3f &pos, float scale)
{
    drawSphere(pos[0], pos[1], pos[2], scale);
}

