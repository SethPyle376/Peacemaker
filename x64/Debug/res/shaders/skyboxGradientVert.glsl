#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 sunPosition;
uniform float time;

out vec3 position;
out float sunDistance;


void main(){
	position = aPos;
	gl_Position = projection * view * vec4(aPos, 1.0);
	sunDistance = distance(position, sunPosition);
}