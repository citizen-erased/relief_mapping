#ifndef MODEL_H
#define MODEL_H

#include <cml/cml.h>

struct Vert
{
    int vertex;
    int normal;
    int tex_coord;
};

struct Face
{
    Vert verts[3];
};


class Model
{
public:
    std::vector<cml::vector3f>      verts;
    std::vector<cml::vector3f>      norms;
    std::vector<cml::vector3f>      tex_coords;
    std::vector<cml::vector4f>      tangents4;
    std::vector<cml::vector3f>      vertex_normals;
    std::vector<Face>               faces;

    GLuint tex_id;
    GLuint tex_relief_id;
    GLuint tex_normal_id;

public:
    Model(const std::string &dir, const std::string &filename);

    void draw();
    void drawNormals(float scale = 1.0f);
    void drawTBNs(float scale = 1.0f);

private:
    void load(const std::string &dir, const std::string &filename);
    void loadMaterial(const std::string &dir, const std::string &filename);
    void computeTBN();
};

#endif /* MODEL_H */

