#version 330 core

layout (location = 0) in vec3 aPos;

out vec4 vertexColor;

uniform float u_time;
uniform float u_properTime;
uniform float u_velocity;
uniform float u_gamma;
uniform mat4 u_motionMat;
uniform mat4 u_refChangeMat;
//float newTime = u_gamma * u_properTime + u_gamma * u_gamma * u_velocity * resultVector.y - u_gamma * u_gamma * u_velocity * u_velocity * resultVector.x;




float solveT(float k, float x0)
{
    float t = k;
    for (int i = 0; i < 3; i++)
    {
        float func = t - u_velocity*x0 - k;
        float der = 1;
        t = t - func;
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
	vec3 resultVector = applyMat(u_motionMat, aPos.xyz);
	
    float k = u_properTime / u_gamma;
    float testTime = solveT(k, resultVector.y);

	float newX = aPos.y + u_velocity * testTime;

	vec3 finalVector = applyMat(u_refChangeMat, vec3(testTime, newX, resultVector.z));
	
    gl_Position = vec4(finalVector.y, finalVector.x, finalVector.z, 1.0);
    vertexColor = vec4(1.0, 0.0, 0.0, 1.0);
}
