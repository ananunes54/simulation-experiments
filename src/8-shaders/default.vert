#version 330 core

layout (location = 0) in vec2 aPos;

out vec4 vertexColor;

uniform float u_time;

void main()
{
	mat2 rotationMatrix = mat2(cos(u_time), sin(u_time), -sin(u_time), cos(u_time));
	vec2 resultVector = rotationMatrix * aPos;
	gl_Position = vec4(resultVector.x, resultVector.y, 1.0, 1.0);

	vertexColor = vec4(aPos.x * 0.9, aPos.x * aPos.y, 0.0, 1.0);
}
