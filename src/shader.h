#ifndef SHADER_H
#define SHADER_H

#include <string>

class Shader
{
private:
    enum { INFO_SHADER, INFO_PROGRAM };

private:
    GLuint vert_id, frag_id, prog_id;

public:
    Shader(const std::string &vert_file, const std::string &frag_file);

    bool validate();
    void bind() const;

    GLint getUniformLocation(const char *name) const;
    GLint getAttributeLocation(const char *name) const;

    void setUniform(const std::string &name, GLint i) const;
    void setUniform(const std::string &name, float f) const;
    void setUniform(const std::string &name, const cml::vector3f &vec) const;
    void setUniform(const std::string &name, const cml::matrix44f_c &mat) const;

    void setAttribute(GLint location, const cml::vector3f &vec) const;
    void setAttribute(GLint location, const cml::vector4f &vec) const;
    void setAttribute(GLint location, const cml::matrix33f_c &mat) const;

private:
    bool            compileShader(GLint id, const std::string &filename);
    bool            linkProgram(GLint id);
    void            loadSource(const std::string &filename, std::vector<char> &buffer) const;
    std::string     getLog(GLint id, int info_type);
};

#endif /* SHADER_H */

