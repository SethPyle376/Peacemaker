#version 330 core

out vec4 positionData;
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec2 vertexUV;

out vec2 UV;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightPosition_worldspace;

uniform vec4 plane;

void main(){
	//output vertex position in clip space
	gl_Position = MVP * vec4(vertexPosition_modelspace, 1);

	//Vertex position in worldspace
	Position_worldspace = (M * vec4(vertexPosition_modelspace, 1)).xyz;

	gl_ClipDistance[0] = dot(vec4(Position_worldspace, 1), plane);

	//Vector from vertex to camera in camera space
	//Camera is at origin
	vec3 vertexPosition_cameraspace = (V * M * vec4(vertexPosition_modelspace, 1)).xyz;
	EyeDirection_cameraspace = vec3(0, 0, 0) - vertexPosition_cameraspace;

	//Vector from vertex to light in camera space
	vec3 LightPosition_cameraspace = (V * vec4(LightPosition_worldspace, 1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

	//Normal of the vertex in camera space
	Normal_cameraspace = (V * M * vec4(vertexNormal_modelspace, 0)).xyz;

	UV = vertexUV;
}