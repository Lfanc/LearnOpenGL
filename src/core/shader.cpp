#include "shader.h"
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

namespace
{
    // 读取文件，失败时打印出具体路径
    std::string readShaderFile(const char* path, bool& ok)
    {
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            file.open(path);
            std::stringstream stream;
            stream << file.rdbuf();
            file.close();
            ok = true;
            return stream.str();
        }
        catch (const std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << path
                << "  (" << e.what() << ")" << std::endl;
            ok = false;
            return std::string();
        }
    }

    // 编译单个着色器，失败时打印出具体路径与类型
    unsigned int compileShader(GLenum type, const char* path,
        const std::string& source, const char* typeName)
    {
        unsigned int shader = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader);

        int success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::" << typeName << "::COMPILATION_FAILED ["
                << path << "]\n" << infoLog << std::endl;
            glDeleteShader(shader);
            return 0;
        }
        return shader;
    }
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. 从文件路径中获取顶点/片段着色器
    bool vOK = false, fOK = false;
    std::string vertexCode = readShaderFile(vertexPath, vOK);
    std::string fragmentCode = readShaderFile(fragmentPath, fOK);

    if (!vOK || !fOK)
    {
        ID = 0;
        return;
    }

    // 2. 编译着色器
    unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexPath, vertexCode, "VERTEX");
    unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentPath, fragmentCode, "FRAGMENT");

    if (vertex == 0 || fragment == 0)
    {
        if (vertex)   glDeleteShader(vertex);
        if (fragment) glDeleteShader(fragment);
        ID = 0;
        return;
    }

    // 着色器程序
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    int success = 0;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED [vertex: " << vertexPath
            << ", fragment: " << fragmentPath << "]\n" << infoLog << std::endl;
        glDeleteProgram(ID);
        ID = 0;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::Shader(const char* vertexPath, const char* geometryPath, const char* fragmentPath)
{
    // 1. 从文件路径中获取顶点/几何/片段着色器
    bool vOK = false, gOK = false, fOK = false;
    std::string vertexCode = readShaderFile(vertexPath, vOK);
    std::string geometryCode = readShaderFile(geometryPath, gOK);
    std::string fragmentCode = readShaderFile(fragmentPath, fOK);

    if (!vOK || !gOK || !fOK)
    {
        ID = 0;
        return;
    }

    // 2. 编译着色器
    unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexPath, vertexCode, "VERTEX");
    unsigned int geometry = compileShader(GL_GEOMETRY_SHADER, geometryPath, geometryCode, "GEOMETRY");
    unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentPath, fragmentCode, "FRAGMENT");

    if (vertex == 0 || geometry == 0 || fragment == 0)
    {
        if (vertex)   glDeleteShader(vertex);
        if (geometry) glDeleteShader(geometry);
        if (fragment) glDeleteShader(fragment);
        ID = 0;
        return;
    }

    // 着色器程序
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, geometry);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    int success = 0;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED [vertex: " << vertexPath
            << ", geometry: " << geometryPath
            << ", fragment: " << fragmentPath << "]\n" << infoLog << std::endl;
        glDeleteProgram(ID);
        ID = 0;
    }

    glDeleteShader(vertex);
    glDeleteShader(geometry);
    glDeleteShader(fragment);
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(GLint location, float value)
{
    glUniform1f(location, value);
}
void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}