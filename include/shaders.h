#ifndef SHADERS_H_INCLUDED
#define SHADERS_H_INCLUDED
#include <string>

class Shader
{
    private:
        std::string m_vertexShaderPath;
        std::string m_fragmentShaderPath;
        unsigned int m_ID;

        int u_velocity;
        int u_gamma;
        int u_motionMat;
        int u_refChangeMat;
        int u_color;
        int u_time;
        int u_properTime;

    private:
        unsigned int compileShader(unsigned int shaderType, std::string& shaderSource);
        unsigned int createProgram();

    public:
        Shader(std::string& vertexShaderPath, std::string& fragmentShaderPath);
        ~Shader();
        unsigned int getID();
        int getVelocityUniform();
        int getGammaUniform();
        int getMotionMatUniform();
        int getRefChangeMatUniform();
        int getColorUniform();
        int getTimeUniform();
        int getProperTimeUniform();
};

#endif 
