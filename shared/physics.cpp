#include <iostream>
#include <physics.h>
#include <utils.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

glm::mat4 Physics::getGroupGeneratorMat4()
{
    glm::mat4 tempMat(1.0f);
    for (auto j = 0; j < 4; j++)
    {
        for (auto i = 0; i < 4; i++)
        {
            tempMat[j][i] = m_groupGeneratorMat5[j*5 + i];
        }
    }
    return tempMat;
}


glm::mat4 Physics::getPoincareGroupMat4()
{
    glm::mat4 tempMat(1.0f);
    for (auto j = 0; j < 4; j++)
    {
        for (auto i = 0; i < 4; i++)
        {
            tempMat[j][i] = m_poincareGroupMat5[j*5 + i];
        }
    }
    return tempMat;
}


glm::mat4 Physics::getRefChangeMat()
{
    return m_refChangeMat;
}


glm::vec4 Physics::getFourPosition()
{
    return m_fourPosition;
}

glm::vec4 Physics::getFourVelocity()
{
    return m_fourVelocity;
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

float Physics::getExternTimeInterval()
{
    return m_externTimeInterval;
}

void Physics::setCenter(glm::vec3 center, glm::mat4 modelMat)
{
    glm::vec4 expandedCenter = glm::vec4(center[0], center[1], center[2], 1.0f);
    m_fourPosition = modelMat * expandedCenter;
}

void Physics::setGroupGeneratorMat(glm::mat4 groupGeneratorMat, float dq, MOTION motion)
{
    m_groupGeneratorMat = groupGeneratorMat;

    m_auxPoincareGroupMat = scale(m_groupGeneratorMat, dq);

    m_poincareGroupMat = exp(m_auxPoincareGroupMat);
    m_auxPoincareGroupMat = m_poincareGroupMat;
    
    if (motion == MOTION::inertial)
    {
        m_fourVelocity = m_poincareGroupMat * glm::vec4(1, 0, 0, 0);
        glm::vec4 nextFourPosition = m_fourPosition + (m_fourVelocity * dq);
        m_externTimeInterval = nextFourPosition[0];
    }
    
    else 
    {
        m_fourVelocity = m_groupGeneratorMat * m_fourPosition;
        glm::vec4 nextFourPosition = m_poincareGroupMat * m_fourPosition;
        m_externTimeInterval = nextFourPosition[0];
    }

    m_velocityMagnitude = m_fourVelocity[1] / m_fourVelocity[0];
    if (m_velocityMagnitude != m_velocityMagnitude)
        m_velocityMagnitude = 0.0f;

    float velocityMetric = minkowskiMetric(m_fourVelocity, m_fourVelocity);
    m_properTimeInterval = sqrt(velocityMetric) * dq;
    
    m_gamma = 1 / sqrt(1 - pow(m_velocityMagnitude, 2));

    m_refChangeMat = glm::mat4(
            m_gamma                       , -m_gamma * m_velocityMagnitude, 0, 0,
            -m_gamma * m_velocityMagnitude, m_gamma                       , 0, 0,
            0                             , 0                             , 1, 0,
            0                             , 0                             , 0, 1);
}


void Physics::setGroupGeneratorMat(const float sourceMat[25], float dq, MOTION motion)
{
    for (auto i = 0; i < 25; i++)
    {
        m_groupGeneratorMat5[i] = sourceMat[i];
    }

    scaleMat5(m_groupGeneratorMat5, m_auxPoincareGroupMat5, dq);
    expMat5(m_auxPoincareGroupMat5, m_poincareGroupMat5);

    for (auto i = 0; i < 25; i++)
    {
        m_auxPoincareGroupMat5[i] = m_poincareGroupMat5[i];
    }

    if (motion == MOTION::inertial)
    {
        float tempTargetVector[5];
        float temp4VelocityVector[5] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f};
        multiplyMat5Vec5(m_poincareGroupMat5, temp4VelocityVector, tempTargetVector);
        m_fourVelocity = glm::vec4(tempTargetVector[0], tempTargetVector[1], tempTargetVector[2], tempTargetVector[3]);
        glm::vec4 nextFourPosition = m_fourPosition + (m_fourVelocity * dq);
        m_externTimeInterval = nextFourPosition[0];
    }
    
    else 
    {
        float tempTargetVector[5];
        float temp4PositionVector[5] = {m_fourPosition[0], m_fourPosition[1], m_fourPosition[2], m_fourPosition[3], 1.0f};
        multiplyMat5Vec5(m_groupGeneratorMat5, temp4PositionVector, tempTargetVector);
        m_fourVelocity = glm::vec4(tempTargetVector[0], tempTargetVector[1], tempTargetVector[2], tempTargetVector[3]);
        multiplyMat5Vec5(m_poincareGroupMat5, temp4PositionVector, tempTargetVector);
        glm::vec4 nextFourPosition = glm::vec4(tempTargetVector[0], tempTargetVector[1], tempTargetVector[2], tempTargetVector[3]);
        m_externTimeInterval = nextFourPosition[0];
    }

    m_velocityMagnitude = m_fourVelocity[1] / m_fourVelocity[0];
    if (m_velocityMagnitude != m_velocityMagnitude)
        m_velocityMagnitude = 0.0f;

    float velocityMetric = minkowskiMetric(m_fourVelocity, m_fourVelocity);
    m_properTimeInterval = sqrt(velocityMetric) * dq;
    
    m_gamma = 1 / sqrt(1 - pow(m_velocityMagnitude, 2));

    m_refChangeMat = glm::mat4(
            m_gamma                       , -m_gamma * m_velocityMagnitude, 0, 0,
            -m_gamma * m_velocityMagnitude, m_gamma                       , 0, 0,
            0                             , 0                             , 1, 0,
            0                             , 0                             , 0, 1);
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
        print(m_groupGeneratorMat, stream);
        stream << "--------------------------------------" << std::endl;
        stream << "PoincareGroup Matrix:" << std::endl;
        print(m_poincareGroupMat, stream);
        stream << "--------------------------------------" << std::endl;
        stream << "PoincareGroup auxiliar Matrix:" << std::endl;
        print(m_auxPoincareGroupMat, stream);
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

void Physics::updatePoincareGroupMat()
{
    m_poincareGroupMat = m_auxPoincareGroupMat * m_poincareGroupMat;
}
