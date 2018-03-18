#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;
in float fog_factor;

out vec3 color;

uniform sampler2D texture_diffuse1;
uniform mat4 MV;
uniform vec3 LightPosition_worldspace;


void main(){
	vec3 LightColor = vec3(0.75, 0.25, 0);
	float LightPower = 9000.0f;

	vec3 MaterialDiffuseColor = texture(texture_diffuse1, UV).rgb;
	vec3 MaterialAmbientColor = vec3(0.5, 0.5, 0.5) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3, 0.3, 0.3);

	vec4 emissive_color = vec4(0.0, 1.0, 0.0, 1.0); // green
    vec4 ambient_color  = vec4(1.0, 1.0, 1.0, 1.0); // white
    vec4 diffuse_color  = vec4(1.0, 1.0, 1.0, 1.0); // white
    vec4 specular_color = vec4(0.0, 0.0, 1.0, 1.0);

    float emissive_contribution = 0.02;
    float ambient_contribution  = 0.20;
    float diffuse_contribution  = 0.40;
    float specular_contribution = 0.38;

    float d = dot(Normal_cameraspace, LightDirection_cameraspace);


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

	//color = color * (1.0-fog_factor) + vec3(0.25, 0.75, 0.65) * (fog_factor);

	//color = MaterialDiffuseColor;


}