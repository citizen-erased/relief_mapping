#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include "types.h"

class Material
{
public:
    cml::vector4f ambient;
    cml::vector4f diffuse;
    cml::vector4f specular;
    float shininess;

public:
    Material(){}
    virtual ~Material(){}

    virtual void draw(Render *render, RenderObject &o){}
};



class GenericShaderMaterial : public Material
{
private:
    ShaderPtr shader;

public:
    GenericShaderMaterial(const std::string &vs_file, const std::string &fs_file);

    void draw(Render *render, RenderObject &o);
};


class GenericShaderMaterialTextured : public Material
{
private:
    ShaderPtr shader;

public:
    GenericShaderMaterialTextured(const std::string &vs_file, const std::string &fs_file);

    void draw(Render *render, RenderObject &o);
};


class TransmittanceMaterial : public Material
{
private:
    ShaderPtr shader;
    GLuint cube_map;

public:
    float eta_r, eta_g, eta_b;

public:
    TransmittanceMaterial(const std::string &vs_file, const std::string &fs_file, GLuint cube_map);

    void draw(Render *render, RenderObject &o);
};


class BumpMaterial : public Material
{
private:
    ShaderPtr shader;

public:
    BumpMaterial (const std::string &vs_file, const std::string &fs_file);

    void draw(Render *render, RenderObject &o);
};


class ReliefMaterial : public Material
{
private:
    ShaderPtr shader;

public:
    ReliefMaterial(const std::string &vs_file, const std::string &fs_file);
    void draw(Render *render, RenderObject &o);
};



#endif /* MATERIAL_H */

