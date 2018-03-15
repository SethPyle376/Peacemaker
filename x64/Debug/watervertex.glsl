#version 330 core

in vec3 position;

out vec2 textureCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position.x, position.y, position.z, 1.0);
	textureCoords = vec2(position.x/2.0 + 0.5, position.y/2.0 + 0.5);
}