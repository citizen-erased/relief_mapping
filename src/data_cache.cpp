#include "precompiled.h"
#include "types.h"
#include "model.h"
#include "shader.h"
#include "data_cache.h"

namespace
{
    std::map<std::string, ModelPtr> models;
    std::map<std::string, GLuint> textures;
    std::map<std::string, GLuint> cubemaps;
    std::map<std::string, FontPtr> fonts;
    std::map<std::string, ShaderPtr> shaders;


void loadImage(const std::string &filename, sf::Image &img, bool flip=true)
{
    if(!img.LoadFromFile(filename))
        fatal(format("failed to load image \"%1%\"\n") % filename);

    if(flip)
    {
        int width = img.GetWidth();
        int height = img.GetHeight();

        for(unsigned int y = 0 ; y < height/2; y++)
            for(unsigned int x = 0 ; x < width; x++)
            {
                sf::Color tmp = img.GetPixel(x, y);
                img.SetPixel(x, y, img.GetPixel(x, height-y-1));
                img.SetPixel(x, height-y-1, tmp);
            }
    }
}

} /* end anonymous namespace */

ModelPtr DataCache::getModel(const std::string &dir, const std::string &filename)
{
    if(models.find(dir+filename) != models.end())
        return models[dir+filename];

    ModelPtr m = ModelPtr(new Model(dir, filename));
    models[dir+filename] = m;
    return m;
}

GLuint DataCache::getTexture(const std::string &filename)
{
    if(textures.find(filename) != textures.end())
        return textures[filename];

    sf::Image img;

    print(format("loading texture: \"%1%\"\n") % filename);
    loadImage(filename, img);

    GLint pushed_tex_id;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &pushed_tex_id);

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.GetWidth(), img.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.GetPixelsPtr());
    print(format("created OpenGL texture id=%1%\n") % id);

    glBindTexture(GL_TEXTURE_2D, pushed_tex_id);

    textures[filename] = id;
    return id;
}

GLuint DataCache::getCubeMap(const std::vector<std::string> &names)
{
    std::string key = names[0] + names[1] + names[2] + names[3] + names[4] + names[5];
    if(cubemaps.find(key) != cubemaps.end())
        return cubemaps[key];

    GLuint cube_map_id;
    glGenTextures(1, &cube_map_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map_id);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

    GLenum targets[] = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
    };

    for(int i = 0; i < 6; i++)
    {
        sf::Image img;
        loadImage(names[i], img, false);
        glTexImage2D(targets[i], 0, GL_RGBA, img.GetWidth(), img.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.GetPixelsPtr());
    }

    cubemaps[key] = cube_map_id;
    return cube_map_id;
}

FontPtr DataCache::getFont(const std::string &filename, unsigned int size)
{
    std::string key = boost::str(format("%1%%2%") % filename % size);

    if(fonts.find(key) != fonts.end())
        return fonts[key];

    FontPtr f(new sf::Font());
    if(!f->LoadFromFile(filename, size))
    {
        warning(format("failed to load font \"%1%\"\n") % filename);
        f = FontPtr(new sf::Font(sf::Font::GetDefaultFont()));
    }

    fonts[key] = f;
    return fonts[key];
}

ShaderPtr DataCache::getShader(const std::string &vert_filename, const std::string &frag_filename)
{
    std::string key = vert_filename + frag_filename;
    if(shaders.find(key) != shaders.end())
        return shaders[key];

    ShaderPtr s = ShaderPtr(new Shader(vert_filename, frag_filename));
    shaders[key] = s;
    return s;
}

