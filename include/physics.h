#ifndef PHYSICS_INCLUDED_H
#define PHYSICS_INCLUDED_H

#include <glm/glm.hpp>
#include <math5d.h>

enum class MOTION {inertial, hyperbolic};

// por enquanto, as dimensoes para os vetores sao: t, x, 1
// as matrizes sao "extendidas", tem um vetor de translacao adicionado

class Physics 
{
    Mat5 m_groupGeneratorMat = Mat5(0.0f);
    Mat5 m_poincareGroupMat = Mat5(1.0f);
    Mat5 m_auxPoincareGroupMat = Mat5(1.0f);

    glm::mat3 m_alignmentMat = glm::mat3(1.0f);

    glm::mat4 m_refChangeMat = glm::mat4(1.0f);
    //a quadrivelocidade diz respeito a posicao do centro do objeto no "tempo inicial"
    glm::vec4 m_fourVelocity = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    // a quadriposicao diz respeito a posicao do centro do objeto no "tempo inicial"
    glm::vec4 m_fourPosition = glm::vec4(0.0f);
    // o modulo da velocidade diz respeito a velocidade do centro do objeto
    glm::vec3 m_velocityVector = glm::vec3(0.0f);
    float m_velocityMagnitude = 0.0f;
    float m_gamma = 1.0f;
    float m_properTimeInterval = 0.0f;
    float m_externTimeInterval = 0.0f;
    float m_properAngularVelocity = 0.0f;

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
    Mat5 buildPoincareGenerator(const glm::vec3& linAcceleration, const glm::vec3& angAcceleration, const glm::vec4& translation);

    void reset();
};

#endif
