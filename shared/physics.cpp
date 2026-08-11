#include <iostream>
#include <physics.h>
#include <utils.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

glm::mat4 Physics::getGroupGeneratorMat()
{
    return m_groupGeneratorMat;
}

float* Physics::getGroupGeneratorMatPtr()
{
    return glm::value_ptr(m_groupGeneratorMat);
}

glm::mat4 Physics::getPoincareGroupMat()
{
    return m_poincareGroupMat;
}

float* Physics::getPoincareGroupMatPtr()
{
    return glm::value_ptr(m_poincareGroupMat);
}

glm::mat4 Physics::getRefChangeMat()
{
    return m_refChangeMat;
}

float* Physics::getRefChangeMatPtr()
{
    return glm::value_ptr(m_refChangeMat);
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
