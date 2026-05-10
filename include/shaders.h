#ifndef SHADERS_H_INCLUDED
#define SHADERS_H_INCLUDED
#include <string>

static unsigned int compileShader(unsigned int shaderType, std::string& shaderSource);

unsigned int createProgram(std::string& vertexShaderSource, std::string& fragmentShaderSource);

#endif 
