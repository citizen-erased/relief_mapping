#ifndef DATA_CACHE_H
#define DATA_CACHE_H

#include "types.h"

namespace DataCache
{
ModelPtr getModel(const std::string &dir, const std::string &filename);
GLuint getTexture(const std::string &filename);
GLuint getCubeMap(const std::vector<std::string> &names);
FontPtr getFont(const std::string &filename, unsigned int size);
ShaderPtr getShader(const std::string &vert_filename, const std::string &frag_filename);
}

#endif /* DATA_CACHE_H */
