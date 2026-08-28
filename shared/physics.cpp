#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <physics.h>
#include <utils.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <fstream>

#include <math5d.h>

Physics::Physics(float externTimeInterval) : m_externTimeInterval(externTimeInterval), m_properTimeInterval(externTimeInterval) {}

glm::mat4 Physics::getGroupGeneratorMat4()
{
    return m_groupGeneratorMat.truncate();
}


glm::mat4 Physics::getPoincareGroupMat4()
{
    return m_poincareGroupMat.truncate();
}


glm::mat4 Physics::getRefChangeMat()
{
    return m_refChangeMat;
}

glm::mat3 Physics::getAlignmentMat()
{
    return m_alignmentMat;
}

glm::vec4 Physics::getFourPosition()
{
    return m_fourPosition;
}

glm::vec4 Physics::getFourVelocity()
{
    return m_fourVelocity;
}

glm::vec4 Physics::getPoincareTranslationVec()
{
    return m_poincareGroupMat[4].truncate();
}

glm::vec3 Physics::getVelocityVector()
{
    return m_velocityVector;
}

float Physics::getVelocityMagnitude()
{
    return m_velocityMagnitude;
}

float Physics::getGamma()
{
    return m_gamma;
}

float Physics::getProperTimeInterval()
{
    return m_properTimeInterval;
}


void Physics::setCenter(glm::vec3 center, glm::mat4 modelMat)
{
    glm::vec4 expandedCenter = glm::vec4(center[0], center[1], center[2], 1.0f);
    m_fourPosition = modelMat * expandedCenter;
}


void Physics::log(const char* logOutputPath)
{
    static unsigned int call = 0;

    std::ofstream stream(logOutputPath);

    if (stream.is_open())
    {
        stream << "Iteration: " << call << std::endl;
        stream << "--------------------------------------" << std::endl;
        stream << "groupGenerator Matrix:" << std::endl;
        printMat(m_groupGeneratorMat.value_ptr(), 5, stream);
        stream << "--------------------------------------" << std::endl;
        stream << "PoincareGroup Matrix:" << std::endl;
        printMat(m_poincareGroupMat.value_ptr(), 5, stream);
        stream << "--------------------------------------" << std::endl;
        stream << "PoincareGroup auxiliar Matrix:" << std::endl;
        printMat(m_auxPoincareGroupMat.value_ptr(), 5, stream);
        stream << "--------------------------------------" << std::endl;
        stream << "Reference Change Matrix:" << std::endl;
        print(m_refChangeMat, stream);
        stream << "--------------------------------------" << std::endl;
        stream << "Position four-vector:" << std::endl;
        print(m_fourPosition, stream);
        stream << "--------------------------------------" << std::endl;
        stream << "Velocity four-vector:" << std::endl;
        print(m_fourVelocity, stream);
        stream << "--------------------------------------" << std::endl;
        stream << "Velocity Magnitude: " << m_velocityMagnitude << std::endl;
        stream << "--------------------------------------" << std::endl;
        stream << "Gamma: " << m_gamma << std::endl;
        stream << "--------------------------------------" << std::endl;
        stream << "Proper Time Interval: " << m_properTimeInterval << std::endl;
        stream << "--------------------------------------" << std::endl;
        stream << "Extern Time Interval: " << m_externTimeInterval << std::endl;
        stream << "--------------------------------------" << std::endl;
        stream << std::endl;

        stream.close();
        call++;
    }

}

void Physics::update()
{
    if (m_motion == MOTION::hyperbolic)
    {
        m_poincareGroupMat = m_poincareGroupMat * m_auxPoincareGroupMat;
    }
    
    m_properTime += m_properTimeInterval;
}

void Physics::reset()
{
    m_groupGeneratorMat = Mat5(0.0f);
    m_refChangeMat = glm::mat4(1.0f);
    m_poincareGroupMat = m_auxPoincareGroupMat = Mat5(1.0f);
    m_velocityMagnitude = 0.0f;
    m_properTimeInterval = m_externTimeInterval;
    m_fourVelocity = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    m_fourPosition = glm::vec4(0.0f);
    m_gamma = 1.0f;
    m_properTime = 0.0f;
}

void Physics::setProperAngVelocity(float w)
{
    m_properAngularVelocity = w;
}

float Physics::getProperAngVelocity()
{
    return m_properAngularVelocity;
}

void Physics::buildPoincareGenerator(const glm::vec3& linAcceleration, const glm::vec3& angAcceleration, const glm::vec4& translation)
{
    m_groupGeneratorMat[0] = Vec5(0.0f, linAcceleration[0], linAcceleration[1], linAcceleration[2], translation[0]);
    m_groupGeneratorMat[1] = Vec5(linAcceleration[0], 0.0f, -angAcceleration[2], angAcceleration[1], translation[1]);
    m_groupGeneratorMat[2] = Vec5(linAcceleration[1], angAcceleration[2], 0.0f, -angAcceleration[0], translation[2]);
    m_groupGeneratorMat[3] = Vec5(linAcceleration[2], -angAcceleration[1], angAcceleration[0], 0.0f, translation[3]);
    m_groupGeneratorMat[4][4] = 1.0f;

    m_groupGeneratorMat = m_groupGeneratorMat * m_properTimeInterval;
    m_poincareGroupMat = m_groupGeneratorMat.exp();
    
    Vec5 aux(1, 0, 0, 0, 0);
    m_fourVelocity = (m_poincareGroupMat * aux).truncate();

    m_velocityVector[0] = m_fourVelocity[1] / m_fourVelocity[0];
    m_velocityVector[1] = m_fourVelocity[2] / m_fourVelocity[0];
    m_velocityVector[2] = m_fourVelocity[3] / m_fourVelocity[0];

    float velocityMetric = minkowskiMetric(m_fourVelocity, m_fourVelocity);
    m_velocityMagnitude = sqrt(pow(m_fourVelocity[0], 2) - 1) / m_fourVelocity[0];

    if (m_velocityMagnitude != m_velocityMagnitude)
        m_velocityMagnitude = 0.0f;

    m_properTimeInterval = sqrt(velocityMetric) * m_externTimeInterval;
    
    m_gamma = 1 / sqrt(1 - pow(m_velocityMagnitude, 2));

    m_refChangeMat = glm::mat4(
            m_gamma                       , -m_gamma * m_velocityMagnitude, 0, 0,
            -m_gamma * m_velocityMagnitude, m_gamma                       , 0, 0,
            0                             , 0                             , 1, 0,
            0                             , 0                             , 0, 1);

    /* definir as matrizes para alinhar o movimento */
    if (m_velocityMagnitude > 0.0001f)
    {
        glm::vec3 velocityVecNormalized = glm::normalize(m_velocityVector);
        glm::quat rotationQuat = glm::rotation(glm::vec3(1.0f, 0.0f, 0.0f), velocityVecNormalized);
        m_alignmentMat = glm::mat3_cast(rotationQuat);
    }
    else 
    {
        m_alignmentMat = glm::mat3(1.0f);
    }
}

float Physics::getProperTime()
{
    return m_properTime;
}

void Physics::setProperTime(float time)
{
    m_properTime = time;
}
