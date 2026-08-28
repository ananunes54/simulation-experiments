#include <camera.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Camera::setViewMat(const glm::vec3& viewPosition, const glm::vec3& viewTarget, const glm::vec3& viewUp)
{
    if (m_viewPosition == viewPosition &&  m_viewTarget == viewTarget && m_viewUp == viewUp)
        return;
    
    m_viewPosition = viewPosition;
    m_viewTarget = viewTarget;
    m_viewUp = viewUp;

    calculateCamera();
}

void Camera::setProjectionMat(float angle, float aspectRatio, float near, float far)
{
    if (m_projectionAngle == angle && m_projectionAspectRatio == aspectRatio && m_near == near && m_far == far)
        return;

    m_projectionAngle = angle;
    m_projectionAspectRatio = aspectRatio;
    m_near = near;
    m_far = far;

    calculateCamera();
}

glm::mat4 Camera::getViewMat()
{
    return  m_viewMat;
}

glm::mat4 Camera::getProjectionMat()
{
    return m_projectionMat;
}

glm::mat4 Camera::getCameraMat()
{
    return m_cameraMat;
}

void Camera::calculateCamera()
{
    m_viewMat = glm::lookAt(m_viewPosition, m_viewTarget, m_viewUp);
    m_projectionMat = glm::perspective(m_projectionAngle, m_projectionAspectRatio, m_near, m_far);
    m_cameraMat = m_projectionMat * m_viewMat;

    cameraAltered = true;

}
