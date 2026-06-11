#include <iostream>
#include <physics.h>
#include <utils.h>

glm::mat3 Physics::getAccelerationMat()
{
    return m_accelerationMat;
}

glm::mat3 Physics::getMotionMat()
{
    return m_motionMat;
}

glm::mat3 Physics::getRefChangeMat()
{
    return m_refChangeMat;
}

glm::vec3 Physics::getFourPosition()
{
    return m_fourPosition;
}

glm::vec3 Physics::getFourVelocity()
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

void Physics::setFourPosition(glm::vec3 fourPosition)
{
    m_fourPosition = fourPosition;
}

void Physics::setAccelerationMat(glm::mat3 accelerationMat, float dq)
{
    m_accelerationMat = accelerationMat;
    // ====== debug =======
    for (auto i = 0; i < 3; i++)
    {
        for (auto j = 0; j < 3; j++)
        {
            std::cout << "[" << m_accelerationMat[j][i] << "]";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    //=====================

    m_auxMotionMat = scale(m_accelerationMat, dq);
    // ====== debug =======
    for (auto i = 0; i < 3; i++)
    {
        for (auto j = 0; j < 3; j++)
        {
            std::cout << "[" << m_auxMotionMat[j][i] << "]";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    //=====================
    m_motionMat = exp(m_auxMotionMat);
    // ====== debug =======
    for (auto i = 0; i < 3; i++)
    {
        for (auto j = 0; j < 3; j++)
        {
            std::cout << "[" << m_motionMat[j][i] << "]";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    //=====================
    
    m_fourVelocity = m_accelerationMat * m_fourPosition;
    // ====== debug =======
    for (auto i = 0; i < 3; i++)
        std::cout << "[" << m_fourVelocity[i] << "]";
    std::cout << std::endl;
    //=====================


    glm::vec3 nextFourPosition = m_motionMat * m_fourPosition;
    m_externTimeInterval = nextFourPosition[0];

    m_velocityMagnitude = m_fourVelocity[1] / m_fourVelocity[0];

    float velocityMetric = minkowskiMetric(m_fourVelocity, m_fourVelocity);
    m_properTimeInterval = sqrt(velocityMetric) * dq;
    
    m_gamma = 1 / sqrt(1 - pow(m_velocityMagnitude, 2));

    m_refChangeMat = glm::mat3(
            m_gamma, -m_gamma * m_velocityMagnitude, 0,
            -m_gamma * m_velocityMagnitude, m_gamma, 0,
            0, 0, 1);
}

void Physics::updateMotionMat()
{
    m_motionMat = m_motionMat * m_auxMotionMat;
}
