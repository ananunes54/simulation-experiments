#version 330 core

layout (location = 0) in vec2 aPos;

out vec4 vertexColor;

uniform float u_time;
uniform mat3 u_lorentzMat;

vec2 applyLorentz(mat3 m, vec2 v)
{
	vec3 tempVec = vec3(v.xy, 1.0);
	tempVec = m * tempVec;
	return tempVec.xy;
}

void main()
{
	mat2 rotationMatrix = mat2(cos(u_time), -sin(u_time), sin(u_time), cos(u_time));
	vec2 resultVector = rotationMatrix * aPos;
	resultVector = applyLorentz(u_lorentzMat, resultVector);


	gl_Position = vec4(resultVector.x, resultVector.y, 1.0, 1.0);

	vertexColor = vec4(aPos.x * 0.9, aPos.x * aPos.y, 0.0, 1.0);
}
