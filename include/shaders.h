#ifndef SHADERS_H_INCLUDED
#define SHADERS_H_INCLUDED

#include <glad/glad.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <variant>

using UniformType = std::variant<int, float, glm::vec3, glm::vec4, glm::mat3, glm::mat4>;


class Shader
{
    private:
        std::string m_vertexShaderPath;
        std::string m_fragmentShaderPath;
        unsigned int m_ID;
        std::unordered_map<const char*, UniformType> m_uniforms;

    private:
        unsigned int compileShader(unsigned int shaderType, std::string& shaderSource);
        unsigned int createProgram();

    public:
        Shader(std::string& vertexShaderPath, std::string& fragmentShaderPath);
        ~Shader();
        unsigned int getID();
        void setFloat(const char* name, float value);
        void setGlmMat3(const char* name, glm::mat3 value);
        void setGlmMat4(const char* name, glm::mat4 value);
        void setGlmVec3(const char* name, glm::vec3 value);
        void setGlmVec4(const char* name, glm::vec4 value);

        void bind();
        void unbind();
};

#endif 
