#ifndef PHYSICS_INCLUDED_H
#define PHYSICS_INCLUDED_H

#include <glm/glm.hpp>

// por enquanto, as dimensoes para os vetores sao: t, x, 1
// as matrizes sao "extendidas", tem um vetor de translacao adicionado

class Physics 
{
    glm::mat3 m_accelerationMat;
    glm::mat3 m_motionMat;
    glm::mat3 m_auxMotionMat;
    glm::mat3 m_refChangeMat;
    //a quadrivelocidade diz respeito a posicao do centro do objeto no "tempo inicial"
    glm::vec3 m_fourVelocity;
    // a quadriposicao diz respeito a posicao do centro do objeto no "tempo inicial"
    glm::vec3 m_fourPosition;
    // o modulo da velocidade diz respeito a velocidade do centro do objeto
    float m_velocityMagnitude;
    float m_gamma;
    float m_properTimeInterval;
    float m_externTimeInterval;

public:
    Physics() {};
    glm::mat3 getAccelerationMat();
    glm::mat3 getMotionMat();
    glm::mat3 getRefChangeMat();
    glm::vec3 getFourVelocity();
    glm::vec3 getFourPosition();
    float getVelocityMagnitude();
    float getGamma();
    float getProperTimeInterval();
    float getExternTimeInterval();

    void setFourPosition(glm::vec3 fourPosition);
    void setAccelerationMat(glm::mat3 mat, float dq);

    void updateMotionMat();
};

#endif
