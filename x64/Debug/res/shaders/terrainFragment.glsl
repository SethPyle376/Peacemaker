#version 330 core

out vec4 FragColor;

in vec3 tecColor;

void main()
{
	FragColor = vec4(tecColor, 1.0);
}