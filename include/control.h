#ifndef CONTROL_INCLUDED_H
#define CONTROL_INCLUDED_H

#include <glm/glm.hpp>

struct PhysicsConfig
{
    glm::vec3 linearAccel = glm::vec3(0.0f);
    glm::vec3 angularAccel = glm::vec3(0.0f);
    float properAngVelocity = 0.0f;
    
    glm::vec2 linearAccelLimit = glm::vec2(-20.0f, 20.0f);
    glm::vec2 angularAccelLimit = glm::vec2(-5.0f, 0.5f);
    glm::vec2 properAngVelocityLimit = glm::vec2(-5.0f, 5.0f);

    void reset();
};

struct TransformConfig
{
    glm::vec3 objectPosition = glm::vec3(0.0f);
    glm::vec3 objectRotation = glm::vec3(0.0f);
    float objectScale = 1.0f;

    void reset();
};

struct CameraConfig
{
    glm::vec3 viewPosition = glm::vec3(0.0f);
    glm::vec3 viewTarget = glm::vec3(0.0f);
    glm::vec3 viewUp = glm::vec3(0.0f, 1.0f, 0.0f);

    void reset();
};

struct SimulationState
{
    bool paused = true;
    bool stateAltered = false;
    float dTime = 0.01;
    float time = 0.0f;

    void resetAll();
    void resetTime();
    void updateTime();
};
#endif
