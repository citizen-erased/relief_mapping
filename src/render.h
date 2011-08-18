#ifndef RENDER_H
#define RENDER_H

#include <map>
#include <vector>
#include <string>
#include "types.h"

struct Light
{
    cml::vector4f ambient;
    cml::vector4f diffuse;
    cml::vector4f specular;
    cml::vector4f position;
    GLuint gl_id;
    bool on;
};

struct RenderObject
{
    bool visible;
    float relief_scale;
    ModelPtr model;
    //ShaderPtr shader;
    MaterialPtr material;

    //cml::vector4f ambient;
    //cml::vector4f diffuse;
    //cml::vector4f specular;
    //float shininess;

    cml::vector3f position;
    float rotation;

    cml::matrix44f_c transform;

};

class Render
{
public:
    CameraPtr camera;
    std::map<std::string, MaterialPtr> materials;
    std::map<std::string, TransmittanceMaterialPtr> transmittance_materials;

    std::vector<Light>          lights;
    std::vector<RenderObject>   objects;

    GLuint                      cubemap;
    bool                        debug_draw, draw_teapot;

public:
    Render();

    void initLights();
    void initMaterials();
    void initObjects();

    void tick();
    void draw();
    void drawCubeBox(float scale);
};

#endif /* RENDER_H */

