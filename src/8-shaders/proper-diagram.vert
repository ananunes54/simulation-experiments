#version 330 core

layout (location = 0) in vec3 aPos;

out vec4 vertexColor;

uniform float u_time;
uniform float u_properTime;
uniform float u_velocity;
uniform float u_gamma;
uniform float u_angVelocity;
uniform vec4 u_poincareTranslationVec;
uniform mat4 u_poincareGroupMat;
uniform mat4 u_refChangeMat;
uniform mat3 u_alignmentMat;
uniform mat3 u_alignmentMatInverse;

uniform mat4 u_modelMatRotation;

uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_projectionMat;

float solveT(float x0, float y0, float linVelocity, float angVelocity, float k)
{
    float t = k;

    for (int i = 0; i < 3; i++)
    {
        float sinCalculated = sin(angVelocity * t);
        float cosCalculated = cos(angVelocity * t);
        float function = t + linVelocity * (x0 * cosCalculated - y0 * sinCalculated) - k;
        float derivative = 1 + linVelocity * (- angVelocity * (x0 * sinCalculated + y0 * cosCalculated));
        t = t - function / derivative;
    }

    return t;
}


void main()
{
    vec4 rotated = u_modelMatRotation * vec4(aPos, 1.0);
    vec3 properRefPosition = u_alignmentMatInverse * rotated.xyz;
    float k = u_time / u_gamma;

    float newTime = solveT(properRefPosition.x, properRefPosition.y, u_velocity, u_angVelocity, k);
    float newAngle = u_angVelocity * newTime;
    float newAngleCos = cos(newAngle);
    float newAngleSin = sin(newAngle);
    float newX = rotated.x * newAngleCos - rotated.y * newAngleSin;
    float newY = rotated.x * newAngleSin + rotated.y * newAngleCos;

    vec4 finalVector = (u_poincareGroupMat * vec4(newTime, newX, newY, rotated.z)) + u_poincareTranslationVec;

    vec4 worldPos = u_modelMat * vec4(finalVector.yzw, 1.0);

    gl_Position = u_projectionMat * u_viewMat * worldPos;

    vertexColor = vec4(1.0, 0.0, 0.0, 1.0);
}
