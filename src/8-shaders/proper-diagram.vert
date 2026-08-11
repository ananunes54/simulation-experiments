#version 330 core

layout (location = 0) in vec3 aPos;

out vec4 vertexColor;

uniform float u_time;
uniform float u_properTime;
uniform float u_velocity;
uniform float u_gamma;
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
    float k = u_time / u_gamma;

    float angVelocity = 0;

    float newTime = solveT(aPos.x, aPos.y, 0, angVelocity, 0);
    float newAngle = angVelocity * newTime;
    float newAngleCos = cos(newAngle);
    float newAngleSin = sin(newAngle);
    float newX = aPos.x * newAngleCos - aPos.y * newAngleSin;
    float newY = aPos.x * newAngleSin + aPos.y * newAngleCos;

    vec3 finalVector = applyMat(u_poincareGroupMat, vec3(newTime, newX, newY));

    gl_Position = u_projectionMat * u_viewMat * u_modelMat * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    //gl_Position = vec4(finalVector.y, finalVector.z, 0.0, 1.0);
    vertexColor = vec4(1.0, 0.0, 0.0, 1.0);
}
