#include <transform.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Transform::setPosition(const glm::vec3& position)
{
    m_position = position;
    calculateModelMat();
}

void Transform::setRotation(const glm::vec3& angles)
{
    m_rotationAngles = angles;
    calculateModelMat();
}

void Transform::setScaling(float scale)
{
    m_scalingFactor = scale;
    calculateModelMat();
}

void Transform::set(const glm::vec3& position, const glm::vec3& angles, float scale)
{
    m_position = position;
    m_rotationAngles = angles;
    m_scalingFactor = scale;
    calculateModelMat();
}

void Transform::calculateModelMat()
{
    m_modelMat = glm::translate(glm::mat4(1.0f), m_position);
    m_modelMat = glm::rotate(m_modelMat, glm::radians(m_rotationAngles[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    m_modelMat = glm::rotate(m_modelMat, glm::radians(m_rotationAngles[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    m_modelMat = glm::rotate(m_modelMat, glm::radians(m_rotationAngles[2]), glm::vec3(0.0f, 0.0f, 1.0f));
    m_modelMat = glm::scale(m_modelMat, glm::vec3(m_scalingFactor));
}

glm::mat4 Transform::getModelMat()
{
    return m_modelMat;
}
