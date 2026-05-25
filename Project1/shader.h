#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
class Shader
{
public:
    GLuint Program;

    Shader(const char* vertexPath, const char* fragmentPath);

    void Use();

    void setFloat(const std::string& name, float value) const;

    void setMat3(const std::string& name, const glm::mat3& mat) const;

    void setVec3(const std::string& name, float x, float y, float z);

    void setVec4(const std::string& name, float x, float y, float z, float w);

    void setMat4(const std::string& name, const glm::mat4& mat);
};

#endif