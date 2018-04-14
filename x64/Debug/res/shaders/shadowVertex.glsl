#version 330

in vec3 in_position;

uniform mat4 projection;
uniform mat4 view;

void main(void) {
	gl_Position = projection * view * vec4(in_position, 1.0);
}
