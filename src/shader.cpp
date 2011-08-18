#include "precompiled.h"
#include "shader.h"

Shader::Shader(const std::string &vert_file, const std::string &frag_file)
{
    print(format("Creating shader: vert_file=\"%1%\" frag_file=\"%2%\"\n") % vert_file % frag_file);

    prog_id = glCreateProgram();
    vert_id = glCreateShader(GL_VERTEX_SHADER);
    frag_id = glCreateShader(GL_FRAGMENT_SHADER);

    compileShader(vert_id, vert_file);
    compileShader(frag_id, frag_file);

    glAttachShader(prog_id, vert_id);
    glAttachShader(prog_id, frag_id);
    linkProgram(prog_id);
}

bool Shader::compileShader(GLint id, const std::string &filename)
{
    /* load source code */
    std::vector<char> buffer;
    loadSource(filename, buffer);
    const char *src = &buffer[0];
    glShaderSource(id, 1, static_cast<const GLchar**>(&src), NULL);

    /* compile */
    print(format("Compiling \"%1%\"...") % filename); 
    glCompileShader(id);

    /* check compile status and print any errors */
    GLint status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);

    if(status == GL_FALSE)
    {
        print(format("ERROR\n%1%") % getLog(id, INFO_SHADER));
        return false;
    }

    print("OK\n");
    return true;
}

bool Shader::linkProgram(GLint id)
{
    /* Link */
    print("Linking program...");
    glLinkProgram(id);

    /* check the link status */
    GLint status;
    glGetProgramiv(id, GL_LINK_STATUS, &status);

    if(status == GL_FALSE)
    {
        print(format("ERROR\n%1%") % getLog(id, INFO_PROGRAM));
        return false;
    }

    print("OK\n");
    return true;
}

void Shader::loadSource(const std::string &filename, std::vector<char> &buffer) const
{
    std::ifstream in(filename.c_str());

    in.seekg(0, std::ios::end);
    int len = in.tellg();
    in.seekg(0, std::ios::beg);

    if(len > 0)
    {
        buffer.resize(len + 1);
        in.read(&buffer[0], len);
        in.close();
    }
    else
        buffer.resize(1);

    buffer[buffer.size() - 1] = '\0';

    in.close();
}

std::string Shader::getLog(GLint id, int info_type)
{
    std::string str;
    GLint log_len;

    if(info_type == INFO_SHADER)
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_len);
    else if(info_type == INFO_PROGRAM)
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_len);
    else
        return str;

    if(log_len > 0)
    {
        GLint len;
        std::vector<char> log_chars(log_len);

        if(info_type == INFO_SHADER)
            glGetShaderInfoLog(id, log_len, &len, &log_chars[0]);
        else if(info_type == INFO_PROGRAM)
            glGetProgramInfoLog(id, log_len, &len, &log_chars[0]);

        str += &log_chars[0];
    }

    return str;
}

bool Shader::validate()
{
    GLint status;

    glValidateProgram(prog_id);
    glGetProgramiv(prog_id, GL_VALIDATE_STATUS, &status);

    if(status == GL_FALSE)
    {
        print(format("VALIDATION ERROR\n%1%\n") % getLog(prog_id, INFO_PROGRAM));
        return false;
    }

    return true;
}

void Shader::bind() const
{
    glUseProgram(prog_id);
}

GLint Shader::getUniformLocation(const char *name) const
{
    return glGetUniformLocation(prog_id, name);
}

GLint Shader::getAttributeLocation(const char *name) const
{
    return glGetAttribLocation(prog_id, name);
}

void Shader::setUniform(const std::string &name, GLint i) const
{
    GLint loc = glGetUniformLocation(prog_id, name.c_str());
    glUniform1i(loc, i);
}

void Shader::setUniform(const std::string &name, float f) const
{
    GLint loc = glGetUniformLocation(prog_id, name.c_str());
    glUniform1f(loc, f);
}

void Shader::setUniform(const std::string &name, const cml::vector3f &vec) const
{
    GLint loc = glGetUniformLocation(prog_id, name.c_str());
    glUniform3fv(loc, 1, vec.data());
}

void Shader::setUniform(const std::string &name, const cml::matrix44f_c &mat) const
{
    GLint loc = glGetUniformLocation(prog_id, name.c_str());
    glUniformMatrix4fv(loc, 1, false, mat.data());
}


void Shader::setAttribute(GLint location, const cml::vector3f &vec) const
{
    glVertexAttrib3fv(location, vec.data());
}

void Shader::setAttribute(GLint location, const cml::vector4f &vec) const
{
    glVertexAttrib4fv(location, vec.data());
}

void Shader::setAttribute(GLint location, const cml::matrix33f_c &mat) const
{
    glVertexAttrib3fv(location,   mat.data());
    glVertexAttrib3fv(location+1, mat.data()+3);
    glVertexAttrib3fv(location+2, mat.data()+6);
}


