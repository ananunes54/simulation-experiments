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

float solveDeltaT(float x0, float y0, float linVelocity, float angVelocity, float centerTime)
{
    float dt = 0.0;

    for (int i = 0; i < 3; i++)
    {
        float localTime = centerTime + dt;
        float sinCalculated = sin(angVelocity * localTime);
        float cosCalculated = cos(angVelocity * localTime);
        float currentX = x0 * cosCalculated - y0 * sinCalculated;
        float currentY = x0 * sinCalculated + y0 * cosCalculated;

        float function = dt + linVelocity * currentX;
        float derivative = 1.0 + linVelocity * (- angVelocity * currentY);
        dt = dt - function / derivative;
    }

    return dt;
}


void main()
{
    vec4 rotated = u_modelMatRotation * vec4(aPos, 1.0);
    vec3 properRefPosition = u_alignmentMatInverse * rotated.xyz;

    float deltaT = solveDeltaT(properRefPosition.x, properRefPosition.y, u_velocity, u_angVelocity, u_properTime);
    float newAngle = u_angVelocity * (deltaT + u_properTime);
    float newAngleCos = cos(newAngle);
    float newAngleSin = sin(newAngle);
    float newX = rotated.x * newAngleCos - rotated.y * newAngleSin;
    float newY = rotated.x * newAngleSin + rotated.y * newAngleCos;

    float visualScale = 1.0f;

    vec4 scaledTranslation = vec4(
            u_poincareTranslationVec.x,
            u_poincareTranslationVec.y * visualScale,
            u_poincareTranslationVec.z * visualScale,
            u_poincareTranslationVec.w * visualScale);

    vec4 finalVector = (u_poincareGroupMat * vec4(deltaT, newX, newY, rotated.z)) + scaledTranslation;

    vec4 worldPos = u_modelMat * vec4(finalVector.yzw, 1.0);

    gl_Position = u_projectionMat * u_viewMat * worldPos;

    vertexColor = vec4(1.0, 0.0, 0.0, 1.0);
}
