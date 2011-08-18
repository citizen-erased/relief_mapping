#include "precompiled.h"
#include "shader.h"
#include "material.h"
#include "data_cache.h"
#include "model.h"
#include "draw.h"
#include "render.h"

Render::Render()
{
    initLights();
    initMaterials();
    initObjects();
    debug_draw = true;
    draw_teapot = false;
}

void Render::initMaterials()
{
    ReliefMaterialPtr relief_mat(new ReliefMaterial("shaders/relief.vs", "shaders/relief.fs"));
    materials["relief"] = relief_mat;
}

void Render::initObjects()
{
    objects.resize(5);

    objects[0].model     = DataCache::getModel("data/", "cube_relief.obj");
    objects[0].material  = materials["relief"];
    objects[0].position  = cml::vector3f(0.0f, 0.0f, 0.0f);
    objects[0].visible   = true;
    objects[0].relief_scale = 0.1f;

    objects[1].model     = DataCache::getModel("data/", "cube_relief2.obj");
    objects[1].material  = materials["relief"];
    objects[1].position  = cml::vector3f(4.0f, 0.0f, 0.0f);
    objects[1].visible   = true;
    objects[1].relief_scale = 0.05f;

    objects[2].model     = DataCache::getModel("data/", "cube_relief3.obj");
    objects[2].material  = materials["relief"];
    objects[2].position  = cml::vector3f(8.0f, 0.0f, 0.0f);
    objects[2].visible   = true;
    objects[2].relief_scale = 0.2f;

    objects[3].model     = DataCache::getModel("data/", "cube_relief4.obj");
    objects[3].material  = materials["relief"];
    objects[3].position  = cml::vector3f(12.0f, 0.0f, 0.0f);
    objects[3].visible   = true;
    objects[3].relief_scale = 0.1f;

    objects[4].model     = DataCache::getModel("data/", "cube_relief5.obj");
    objects[4].material  = materials["relief"];
    objects[4].position  = cml::vector3f(16.0f, 0.0f, 0.0f);
    objects[4].visible   = true;
    objects[4].relief_scale = 0.02f;
}

void Render::initLights()
{
    lights.resize(8);

    for(unsigned int i = 0; i < lights.size(); i++)
    {
        lights[i].gl_id    = GL_LIGHT0 + i;
        lights[i].ambient  = cml::vector4f(0.0f, 0.0f, 0.0f, 1.0f);
        lights[i].diffuse  = cml::vector4f(1.0f, 1.0f, 1.0f, 1.0f);
        lights[i].specular = cml::vector4f(1.0f, 1.0f, 1.0f, 1.0f);
        lights[i].position = cml::vector4f(3.0f, 2.5f, 0.0f, 1.0f);
        lights[i].on       = false;
    }

    lights[0].on = true;
}

void Render::tick()
{
    for(unsigned int i = 0; i < objects.size(); i++)
    {
        RenderObject &o = objects[i];
        o.rotation += 0.01f;

        if(o.rotation > 2.0f*M_PI)
            o.rotation -= 2.0f*M_PI;

        cml::matrix_affine_transform(o.transform, cml::vector3f(0.0f, 1.0f, 0.0f), o.rotation, o.position);
    }
}

void Render::draw()
{
    glUseProgram(0);
    for(unsigned int i = 0; i < lights.size(); i++)
    {
        Light &l = lights[i];
        if(!l.on) continue;

        glLightfv(l.gl_id, GL_POSITION, l.position.data());
        glLightfv(l.gl_id, GL_AMBIENT,  l.ambient.data());
        glLightfv(l.gl_id, GL_DIFFUSE,  l.diffuse.data());
        glLightfv(l.gl_id, GL_SPECULAR, l.specular.data());

        glColor3fv(l.diffuse.data());
        drawSphere(cml::vector3f(l.position.data()), 0.2f);
    }

    for(unsigned int i = 0; i < objects.size(); i++)
    {
        RenderObject &o = objects[i];

        if(!o.visible) continue;

        glPushMatrix();
            glMultMatrixf(o.transform.data());
            o.material->draw(this, o);

            if(debug_draw)
            {
                glUseProgram(0);
                glDisable(GL_TEXTURE_2D);
                glDisable(GL_LIGHTING);
                glDisable(GL_BLEND);
                o.model->drawTBNs(0.1f);
                o.model->drawNormals(0.2f);

                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glScalef(1.01f, 1.01f, 1.01f);
                o.model->draw();
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        glPopMatrix();
    }

    //drawSphere(0.7f, 1.0f, 0.0f, 0.8f);
    //drawCube(0.7f, 1.0f, 0.0f, 0.8f);

    if(draw_teapot)
    {
        glDisable(GL_LIGHTING);
        glColor3f(1.0, 1.0, 1.0);
        glActiveTexture(GL_TEXTURE0);
        glTranslatef(0.4f, 0.6f, 0.4f);
        glScalef(0.6f, 0.6f, 0.6f);
        ModelPtr t = DataCache::getModel("data/", "teapot_textured.obj");
        t->draw();
    }


    glUseProgram(0);
}

