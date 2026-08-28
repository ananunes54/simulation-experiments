#ifndef TRANSFORM_INCLUDED_H
#define TRANSFORM_INCLUDED_H

#include <glm/glm.hpp>

class Transform
{
    private:
        glm::mat4 m_modelMat = glm::mat4(1.0f);
        glm::vec3 m_position = glm::vec3(0.0f);
        glm::vec3 m_rotationAngles = glm::vec3(0.0f);
        float m_scalingFactor = 1.0f;

        void calculateModelMat();

    public:
        void setPosition(const glm::vec3& position);
        void setRotation(const glm::vec3& angles);
        void setScaling(float scale);
        void set(const glm::vec3& position, const glm::vec3& angles, float scale);

        glm::mat4 getModelMat();
};

#endif
