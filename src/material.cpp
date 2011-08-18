#include "precompiled.h"
#include "data_cache.h"
#include "shader.h"
#include "model.h"
#include "render.h"
#include "camera.h"
#include "material.h"


GenericShaderMaterial::GenericShaderMaterial(const std::string &vs_file, const std::string &fs_file)
{
    shader = DataCache::getShader(vs_file, fs_file);
}

void GenericShaderMaterial::draw(Render *render, RenderObject &o)
{
    //glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient.data());
    //glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse.data());
    //glMaterialfv(GL_FRONT, GL_SPECULAR,  specular.data());
    //glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);

    shader->bind();
    o.model->draw();
}


GenericShaderMaterialTextured::GenericShaderMaterialTextured(const std::string &vs_file, const std::string &fs_file)
{
    shader = DataCache::getShader(vs_file, fs_file);
}

void GenericShaderMaterialTextured::draw(Render *render, RenderObject &o)
{
    glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient.data());
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse.data());
    glMaterialfv(GL_FRONT, GL_SPECULAR,  specular.data());
    glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);

    glBindTexture(GL_TEXTURE_2D, o.model->tex_id);
    

    shader->bind();
    shader->setUniform("tex0", 0);
    o.model->draw();
}


TransmittanceMaterial::TransmittanceMaterial(const std::string &vs_file, const std::string &fs_file, GLuint cube_map)
{
    shader = DataCache::getShader(vs_file, fs_file);
    this->cube_map = cube_map;

    eta_r = 0.84f;
    eta_g = 0.87f;
    eta_b = 0.91f;
}

void TransmittanceMaterial::draw(Render *render, RenderObject &o)
{
    cml::vector4f eye = render->camera->getTransform() * cml::vector4f(0.0f, 0.0f, 0.0f, 1.0f);

    float F = ((1.0f - eta_g) * (1.0f - eta_g)) / ((1.0f + eta_g) * (1.0f + eta_g));
    float Fr = ((1.0f - eta_r) * (1.0f - eta_r)) / ((1.0f + eta_r) * (1.0f + eta_r));
    float Fg = ((1.0f - eta_g) * (1.0f - eta_g)) / ((1.0f + eta_g) * (1.0f + eta_g));
    float Fb = ((1.0f - eta_b) * (1.0f - eta_b)) / ((1.0f + eta_b) * (1.0f + eta_b));

    glBindTexture(GL_TEXTURE_2D, cube_map);

    shader->bind();
    shader->setUniform("cube_map", 0);
    shader->setUniform("w_eye_position", cml::vector3f(eye.data()));
    shader->setUniform("m_world", o.transform);
    shader->setUniform("eta_r", eta_r);
    shader->setUniform("eta_g", eta_g);
    shader->setUniform("eta_b", eta_b);
    shader->setUniform("F", F);
    shader->setUniform("Fr", Fr);
    shader->setUniform("Fg", Fg);
    shader->setUniform("Fb", Fb);
    o.model->draw();
}


BumpMaterial::BumpMaterial(const std::string &vs_file, const std::string &fs_file)
{
    shader = DataCache::getShader(vs_file, fs_file);
}

void BumpMaterial::draw(Render *render, RenderObject &o)
{
    cml::vector4f eye = render->camera->getTransform() * cml::vector4f(0.0f, 0.0f, 0.0f, 1.0f);

    cml::matrix44f_c world_to_obj = cml::inverse(o.transform);
    cml::vector4f obj_eye = world_to_obj * eye;
    cml::vector4f obj_light = world_to_obj * render->lights[0].position;


    shader->bind();
    shader->setUniform("eye_pos", cml::vector3f(obj_eye.data()));
    shader->setUniform("light_pos", cml::vector3f(obj_light.data()));

    std::vector<cml::vector3f> &verts = o.model->verts;
    std::vector<cml::vector3f> &vertex_normals = o.model->vertex_normals;
    std::vector<cml::vector4f> &tangents = o.model->tangents4;
    std::vector<cml::vector3f> &tex_coords = o.model->tex_coords;
    std::vector<Face>          &faces = o.model->faces;
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, o.model->tex_id);
    shader->setUniform("diffuse_map", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, o.model->tex_normal_id);
    shader->setUniform("normal_map", 1);

    shader->validate();
    GLint tangent_loc = shader->getAttributeLocation("tangent");

    glBegin(GL_TRIANGLES);
    for(unsigned int face_i = 0; face_i < faces.size(); face_i++)
    {
        Face &face = faces[face_i];

        for(unsigned int vert_i = 0; vert_i < 3; vert_i++)
        {
            Vert &vert = face.verts[vert_i];

            cml::vector3f &pos = verts[vert.vertex];
            cml::vector3f &norm = vertex_normals[vert.vertex];
            cml::vector3f &tex_coord = tex_coords[vert.tex_coord];

            shader->setAttribute(tangent_loc, tangents[vert.vertex]);
            glTexCoord2fv(tex_coord.data());
            glNormal3fv(norm.data());
            glVertex3fv(pos.data());
        }
    }
    glEnd();

    glActiveTexture(GL_TEXTURE0);
}









ReliefMaterial::ReliefMaterial(const std::string &vs_file, const std::string &fs_file)
{
    shader = DataCache::getShader(vs_file, fs_file);
}

void ReliefMaterial::draw(Render *render, RenderObject &o)
{
    cml::vector4f eye = render->camera->getTransform() * cml::vector4f(0.0f, 0.0f, 0.0f, 1.0f);

    cml::matrix44f_c world_to_obj = cml::inverse(o.transform);
    cml::vector4f obj_eye = world_to_obj * eye;
    cml::vector4f obj_light = world_to_obj * render->lights[0].position;


    shader->bind();
    shader->setUniform("eye_pos", cml::vector3f(obj_eye.data()));
    shader->setUniform("light_pos", cml::vector3f(obj_light.data()));
    shader->setUniform("scale", o.relief_scale);

    std::vector<cml::vector3f> &verts = o.model->verts;
    std::vector<cml::vector3f> &vertex_normals = o.model->vertex_normals;
    std::vector<cml::vector4f> &tangents = o.model->tangents4;
    std::vector<cml::vector3f> &tex_coords = o.model->tex_coords;
    std::vector<Face>          &faces = o.model->faces;
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, o.model->tex_id);
    shader->setUniform("diffuse_map", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, o.model->tex_relief_id);
    shader->setUniform("relief_map", 1);

    shader->validate();
    GLint tangent_loc = shader->getAttributeLocation("tangent");


    glBegin(GL_TRIANGLES);
    for(unsigned int face_i = 0; face_i < faces.size(); face_i++)
    {
        Face &face = faces[face_i];

        for(unsigned int vert_i = 0; vert_i < 3; vert_i++)
        {
            Vert &vert = face.verts[vert_i];

            cml::vector3f &pos = verts[vert.vertex];
            cml::vector3f &norm = vertex_normals[vert.vertex];
            cml::vector3f &tex_coord = tex_coords[vert.tex_coord];

            shader->setAttribute(tangent_loc, tangents[vert.vertex]);
            glTexCoord2fv(tex_coord.data());
            glNormal3fv(norm.data());
            glVertex3fv(pos.data());
        }
    }
    glEnd();
}


