#ifndef CAMERA_INCLUDED_H
#define CAMERA_INCLUDED_H

#include <glm/glm.hpp>

class Camera
{
    private:
        glm::vec3 m_viewPosition = glm::vec3(0.0f);
        glm::vec3 m_viewTarget = glm::vec3(0.0f);
        glm::vec3 m_viewUp = glm::vec3(0.0f, 1.0f, 0.0f);

        float m_projectionAngle = 45.0f;
        float m_projectionAspectRatio = 1.0f;
        float m_near = 0.1f;
        float m_far = 100.0f;

        glm::mat4 m_viewMat = glm::mat4(1.0f);
        glm::mat4 m_projectionMat = glm::mat4(1.0f);
        glm::mat4 m_cameraMat = glm::mat4(1.0f);

        void calculateCamera();

    public:
        void setViewMat(const glm::vec3& viewPosition, const glm::vec3& viewTarget, const glm::vec3& viewUp);
        void setProjectionMat(float angle, float aspectRatio, float near, float far);
        void setProjectionRatio(float aspectRatio);

        glm::mat4 getViewMat();
        glm::mat4 getProjectionMat();
        glm::mat4 getCameraMat();

        
};

#endif
