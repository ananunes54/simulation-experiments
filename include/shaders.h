#ifndef SHADERS_H_INCLUDED
#define SHADERS_H_INCLUDED

#include <glad/glad.h>
#include <string>
#include <vector>

struct UniformInfo
{
    unsigned int program;
    unsigned int index;
    std::string name;
    GLenum type;
};


class Shader
{
    private:
        std::string m_vertexShaderPath;
        std::string m_fragmentShaderPath;
        unsigned int m_ID;
        int m_uniformsCount;
        std::vector<UniformInfo> m_uniforms;

    private:
        unsigned int compileShader(unsigned int shaderType, std::string& shaderSource);
        unsigned int createProgram();
        void introspectProgram();

    public:
        Shader(std::string& vertexShaderPath, std::string& fragmentShaderPath);
        ~Shader();
        unsigned int getID();
        int getUniformsCount();
        std::vector<UniformInfo> getUniforms();
};

#endif 
