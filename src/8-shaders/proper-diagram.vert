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

uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_projectionMat;

float solveT(float x0, float y0, float linVelocity, float angVelocity, float k)
{
    float t = k;

    for (int i = 0; i < 3; i++)
    {
        float function = t + linVelocity * (x0 * cos(angVelocity * t) - y0 * sin(angVelocity * t)) - k;
        float derivative = 1 + linVelocity * (- angVelocity * (x0 * sin(angVelocity * t) + y0 * cos(angVelocity * t)));
        t = t - function / derivative;
    }

    return t;
}


vec3 applyMat(mat4 m, vec3 v)
{
	vec4 tempVec = vec4(v.xyz, 1.0);
	tempVec = m * tempVec;
	return tempVec.xyz;
}

void main()
{
    vec4 properRefPosition = u_modelMat * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    float k = u_time / u_gamma;

    float newTime = solveT(properRefPosition.x, properRefPosition.y, u_velocity, u_angVelocity, k);
    float newAngle = u_angVelocity * newTime;
    float newAngleCos = cos(newAngle);
    float newAngleSin = sin(newAngle);
    float newX = properRefPosition.x * newAngleCos - properRefPosition.y * newAngleSin;
    float newY = properRefPosition.x * newAngleSin + properRefPosition.y * newAngleCos;

    //vec3 finalVector = applyMat(u_poincareGroupMat, vec3(newTime, newX, newY));

    vec4 finalVector = (u_poincareGroupMat * vec4(newTime, newX, newY, properRefPosition.z)) + u_poincareTranslationVec;

    gl_Position = u_projectionMat * u_viewMat * vec4(finalVector.y, finalVector.z, finalVector.w, 1.0);
    //gl_Position = vec4(finalVector.y, finalVector.z, 0.0, 1.0);
    vertexColor = vec4(1.0, 0.0, 0.0, 1.0);
}
