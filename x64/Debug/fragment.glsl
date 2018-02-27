#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

out vec3 color;

uniform sampler2D myTextureSampler;
uniform mat4 MV;
uniform vec3 LightPosition_worldspace;


void main(){
	vec3 LightColor = vec3(1, 1, 1);
	float LightPower = 50.0f;

	vec3 MaterialDiffuseColor = texture(myTextureSampler, UV).rgb;
	vec3 MaterialAmbientColor = vec3(0.1, 0.1, 0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3, 0.3, 0.3);

	//Distance to light
	float distance = length(LightPosition_worldspace - Position_worldspace);

	//Normal of fragment in camera space
	vec3 n = normalize(Normal_cameraspace);

	//Direction of light
	vec3 l = normalize(LightDirection_cameraspace);

	//Cosine of angle between normal and light direction
	//Clamped above 0
	float cosTheta = clamp(dot(n, l), 0, 1);

	//Eye vector
	vec3 E = normalize(EyeDirection_cameraspace);

	//Direction triangle reflects light
	vec3 R = reflect(-l, n);

	//Cosine of angle between eye and reflect vector
	float cosAlpha = clamp(dot(E, R), 0, 1);

	color =
		//Fake ambient lighting
		MaterialAmbientColor +
		//Diffuse coloring
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance * distance) +
		//Specular coloring
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha, 5) / (distance * distance);
}