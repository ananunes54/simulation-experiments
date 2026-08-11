#ifndef PHYSICS_INCLUDED_H
#define PHYSICS_INCLUDED_H

#include <glm/glm.hpp>

enum class MOTION {inertial, hyperbolic};

// por enquanto, as dimensoes para os vetores sao: t, x, 1
// as matrizes sao "extendidas", tem um vetor de translacao adicionado

class Physics 
{
    float m_groupGeneratorMat5[25];
    float m_poincareGroupMat5[25];
    float m_auxPoincareGroupMat5[25];
    glm::mat4 m_groupGeneratorMat;
    glm::mat4 m_poincareGroupMat;
    glm::mat4 m_auxPoincareGroupMat;
    glm::mat4 m_refChangeMat;
    //a quadrivelocidade diz respeito a posicao do centro do objeto no "tempo inicial"
    glm::vec4 m_fourVelocity;
    // a quadriposicao diz respeito a posicao do centro do objeto no "tempo inicial"
    glm::vec4 m_fourPosition;
    // o modulo da velocidade diz respeito a velocidade do centro do objeto
    float m_velocityMagnitude;
    float m_gamma;
    float m_properTimeInterval;
    float m_externTimeInterval;

public:
    Physics() {};
    glm::mat4 getGroupGeneratorMat4();
    glm::mat4 getPoincareGroupMat4();
    void getGroupGeneratorMat5(float targetMat[25]);
    void getPoincareGroupMat5(float targetMat[25]);
    glm::mat4 getRefChangeMat();
    glm::vec4 getFourVelocity();
    glm::vec4 getFourPosition();
    float getVelocityMagnitude();
    float getGamma();
    float getProperTimeInterval();
    float getExternTimeInterval();

    void setCenter(glm::vec3 center, glm::mat4 modelMat);
    void setGroupGeneratorMat(glm::mat4 mat, float dq, MOTION motion);
    void setGroupGeneratorMat(const float sourceMat[25], float dq, MOTION motion);

    void log(const char* logOutputPath);

    void updatePoincareGroupMat();
};

#endif
