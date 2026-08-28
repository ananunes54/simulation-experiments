#include <iostream>
#include <control.h>
#include <glm/glm.hpp>


void PhysicsConfig::reset()
{
    linearAccel = glm::vec3(0.0f);
    angularAccel = glm::vec3(0.0f);
    properAngVelocity = 0.0f;
}

void TransformConfig::reset()
{
    objectPosition = glm::vec3(0.0f);
    objectRotation = glm::vec3(0.0f);
    objectScale = 1.0f;
}

void CameraConfig::reset()
{
    viewPosition = glm::vec3(0.0f);
    viewTarget = glm::vec3(0.0f);
    viewUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

void SimulationState::resetAll()
{
    paused = true;
    stateAltered = false;
    time = 0.0f;
}

void SimulationState::resetTime()
{
    time = 0.0f;
}

void SimulationState::updateTime()
{
    time += dTime;
}
