#version 330 core

layout (location = 0) in vec2 aPos;

out vec4 vertexColor;

uniform float u_time;
uniform float u_properTime;
uniform float u_velocity;
uniform float u_gamma;
uniform mat3 u_motionMat;
uniform mat3 u_refChangeMat;

vec2 applyMat(mat3 m, vec2 v)
{
	vec3 tempVec = vec3(v.xy, 1.0);
	tempVec = m * tempVec;
	return tempVec.xy;
}


void main()
{
	vec2 resultVector = applyMat(u_motionMat, aPos);

	float newTime = u_gamma * u_properTime + u_gamma * u_gamma * u_velocity * resultVector.y - u_gamma * u_gamma * u_velocity * u_velocity * resultVector.x;

	float newX = aPos.y + u_velocity * newTime;

	resultVector = applyMat(u_refChangeMat, vec2(newTime, newX));

	gl_Position = vec4(resultVector.y, resultVector.x, 0.0, 1.0);

	vertexColor = vec4(aPos.x * 0.9, aPos.x * aPos.y, 0.0, 1.0);
}
