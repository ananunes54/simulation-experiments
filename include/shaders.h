#ifndef SHADERS_H_INCLUDED
#define SHADERS_H_INCLUDED

#include <glad/glad.h>
#include <string>
#include <vector>



class Shader
{
    private:
        std::string m_vertexShaderPath;
        std::string m_fragmentShaderPath;
        unsigned int m_ID;

    private:
        unsigned int compileShader(unsigned int shaderType, std::string& shaderSource);
        unsigned int createProgram();

    public:
        Shader(std::string& vertexShaderPath, std::string& fragmentShaderPath);
        ~Shader();
        unsigned int getID();
};

#endif 
