#version 330 core
out vec3 FragColor;

in vec2 TexCoord;
in vec3 position;
in float sunDistance;

uniform sampler2D myTexture;
uniform float time;

void main()
{
	vec3 sunColor;
	float haloStrength = 80;
	float horizonHeight;
	float horizonDistance;

	float timeOfDay = time;

	if (timeOfDay > 12)
	{
		timeOfDay -= 12;
	}

	float distanceFromZero = abs(0 - timeOfDay);
	float distanceFromTwelve = abs(12 - timeOfDay);


	if (distanceFromZero < distanceFromTwelve)
	{
		horizonDistance = distanceFromZero;
	}
	else
	{
		horizonDistance = distanceFromTwelve;
	}



	if (position.y < 0)
	{
		FragColor = vec3(0.0, 0.0, 0.0);
	}
	else
	{
    	vec3 skyColor = vec3((1.0 - (horizonDistance / 6)) - position.y * (6) / 1000, position.y / 3000, position.y / 1000);
    	vec3 sunHalo = vec3(haloStrength / (sunDistance), haloStrength / (sunDistance), haloStrength / (sunDistance));

    	if (sunDistance < 35)
    	{
    		sunColor = vec3(1.0, 1.0, 1.0);
    		FragColor = sunHalo + sunColor;
    	}
    	else
    	{
    		sunColor = vec3(0.0, 0.0, 0.0);
    		FragColor = skyColor + sunHalo + sunColor;
    	}

    	if (FragColor.z < 0.3)
    	{
    		FragColor.z = 0.3;
    	}
    }
} 