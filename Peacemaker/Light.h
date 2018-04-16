#pragma once

#include <glm.hpp>

class Light
{
private:
	glm::vec3 position;

	

public:

	glm::mat4 lightSpaceMatrix;


	Light(glm::vec3 position)
	{
		this->position = position;

		float nearPlane = 1.0f;
		float farPlane = 7.5f;

		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);

		glm::mat4 lightView = glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		lightSpaceMatrix = lightProjection * lightView;
	}

	glm::vec3 getPosition()
	{
		return position;
	}

	void offSetPosition(glm::vec3 offset)
	{
		position += offset;

		float nearPlane = 1.0f;
		float farPlane = 100.0f;

		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);

		glm::mat4 lightView = glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		lightSpaceMatrix = lightProjection * lightView;
	}
};