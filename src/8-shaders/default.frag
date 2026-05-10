#version 330 core

in vec4 vertexColor;

out vec4 FragColor;

uniform vec4 u_color;
uniform float u_time;

void main()
{
	FragColor = vec4(u_color.x * abs(cos(u_time)), u_color.y * abs(sin(u_time)), u_color.z * abs(sin(u_time)), 1.0);
}
