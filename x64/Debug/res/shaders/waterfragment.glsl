#version 330 core

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

in vec4 clipSpace;

out vec4 out_Color;




void main(void)
{

	vec2 ndc = (clipSpace.xy/clipSpace.w) / 2.0 + 0.5;
	vec2 refractTexCoords = vec2(ndc.x, ndc.y);
	vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);
	vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
	vec4 refractColor = texture(refractionTexture, refractTexCoords);


	out_Color = mix(reflectColor, refractColor, 0.5);
	//out_Color = vec4(0.0, 0.0, 1.0, 1.0);
}