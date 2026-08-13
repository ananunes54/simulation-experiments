#ifndef PHYSICS_INCLUDED_H
#define PHYSICS_INCLUDED_H

#include <glm/glm.hpp>
#include <math5d.h>

enum class MOTION {inertial, hyperbolic};

// por enquanto, as dimensoes para os vetores sao: t, x, 1
// as matrizes sao "extendidas", tem um vetor de translacao adicionado

class Physics 
{
    Mat5 m_groupGeneratorMat;
    Mat5 m_poincareGroupMat;
    Mat5 m_auxPoincareGroupMat;

    glm::mat3 m_alignmentMat;

    glm::mat4 m_refChangeMat;
    //a quadrivelocidade diz respeito a posicao do centro do objeto no "tempo inicial"
    glm::vec4 m_fourVelocity;
    // a quadriposicao diz respeito a posicao do centro do objeto no "tempo inicial"
    glm::vec4 m_fourPosition;
    // o modulo da velocidade diz respeito a velocidade do centro do objeto
    glm::vec3 m_velocityVector;
    float m_velocityMagnitude;
    float m_gamma;
    float m_properTimeInterval;
    float m_externTimeInterval;
    float m_properAngularVelocity;

public:
    Physics() {};
    glm::mat4 getGroupGeneratorMat4();
    glm::mat4 getPoincareGroupMat4();
    glm::vec4 getPoincareTranslationVec();
    glm::mat4 getRefChangeMat();
    glm::mat3 getAlignmentMat();
    glm::vec4 getFourVelocity();
    glm::vec4 getFourPosition();
    glm::vec3 getVelocityVector();
    float getVelocityMagnitude();
    float getGamma();
    float getProperTimeInterval();
    float getExternTimeInterval();
    float getProperAngVelocity();

    void setProperAngVelocity(float w);
    void setCenter(glm::vec3 center, glm::mat4 modelMat);
    void setGroupGeneratorMat(const Mat5 sourceMat, float dq, MOTION motion);

    void log(const char* logOutputPath);

    void updatePoincareGroupMat();

    void reset();
};

#endif
