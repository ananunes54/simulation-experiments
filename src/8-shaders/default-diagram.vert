#version 330 core

layout (location = 0) in vec3 aPos;

out vec4 vertexColor;

uniform float u_time;
uniform float u_properTime;
uniform float u_velocity;
uniform float u_gamma;
uniform mat4 u_motionMat;
uniform mat4 u_refChangeMat;

uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_projectionMat;

vec3 applyMat(mat4 m, vec3 v)
{
	vec4 tempVec = vec4(v.xyz, 1.0);
	tempVec = m * tempVec;
	return tempVec.xyz;
}

void main()
{
    mat3 rotate = mat3(
            1, 0, 0,
            0, cos(5*u_properTime), -sin(5*u_properTime),
            0, sin(5*u_properTime), cos(5*u_properTime)
            );

    vec3 rotated = rotate * aPos;

	vec3 finalVector = applyMat(u_motionMat, rotated);
	
    gl_Position = u_projectionMat * u_viewMat * u_modelMat * vec4(finalVector.y, finalVector.x, finalVector.z, 1.0);
    vertexColor = vec4(1.0, 0.0, 0.0, 1.0);
}
