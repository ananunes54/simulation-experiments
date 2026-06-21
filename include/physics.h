#ifndef PHYSICS_INCLUDED_H
#define PHYSICS_INCLUDED_H

#include <glm/glm.hpp>

// por enquanto, as dimensoes para os vetores sao: t, x, 1
// as matrizes sao "extendidas", tem um vetor de translacao adicionado

class Physics 
{
    glm::mat4 m_accelerationMat;
    glm::mat4 m_motionMat;
    glm::mat4 m_auxMotionMat;
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
    glm::mat4 getAccelerationMat();
    float* getAccelerationMatPtr();
    glm::mat4 getMotionMat();
    float* getMotionMatPtr();
    glm::mat4 getRefChangeMat();
    float* getRefChangeMatPtr();
    glm::vec4 getFourVelocity();
    glm::vec4 getFourPosition();
    float getVelocityMagnitude();
    float getGamma();
    float getProperTimeInterval();
    float getExternTimeInterval();

    void setCenter(glm::vec3 center);
    void setAccelerationMat(glm::mat4 mat, float dq);

    void updateMotionMat();
};

#endif
