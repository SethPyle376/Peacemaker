#pragma once

#include <glm.hpp>

class Light
{
private:
	glm::vec3 position;

public:
	Light(glm::vec3 position)
	{
		this->position = position;
	}

	glm::vec3 getPosition()
	{
		return position;
	}

	void offSetPosition(glm::vec3 offset)
	{
		position += offset;
	}
};