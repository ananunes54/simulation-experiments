#include <iostream>
#include <physics.h>
#include <utils.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

glm::mat4 Physics::getAccelerationMat()
{
    return m_accelerationMat;
}

float* Physics::getAccelerationMatPtr()
{
    return glm::value_ptr(m_accelerationMat);
}

glm::mat4 Physics::getMotionMat()
{
    return m_motionMat;
}

float* Physics::getMotionMatPtr()
{
    return glm::value_ptr(m_motionMat);
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

void Physics::setCenter(glm::vec3 center)
{
    m_fourPosition = glm::vec4(center[0], center[1], center[2], 1.0f);
}

void Physics::setAccelerationMat(glm::mat4 accelerationMat, float dq)
{
    m_accelerationMat = accelerationMat;

    m_auxMotionMat = scale(m_accelerationMat, dq);

    m_motionMat = exp(m_auxMotionMat);
    m_auxMotionMat = m_motionMat;
    
    m_fourVelocity = m_accelerationMat * m_fourPosition;

    glm::vec4 nextFourPosition = m_motionMat * m_fourPosition;
    m_externTimeInterval = nextFourPosition[0];

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
        stream << "Acceleration Matrix:" << std::endl;
        print(m_accelerationMat, stream);
        stream << "--------------------------------------" << std::endl;
        stream << "Motion Matrix:" << std::endl;
        print(m_motionMat, stream);
        stream << "--------------------------------------" << std::endl;
        stream << "Motion auxiliar Matrix:" << std::endl;
        print(m_auxMotionMat, stream);
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

void Physics::updateMotionMat()
{
    m_motionMat = m_auxMotionMat * m_motionMat;
}
