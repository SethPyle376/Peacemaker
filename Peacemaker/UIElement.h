#pragma once

#include <glm.hpp>
#include <GL\glew.h>

class UIElement
{
private:
	glm::vec2 position;
	glm::vec2 size;


public:
	UIElement(glm::vec2 position, glm::vec2 size);
	void offsetPosition(glm::vec2 update);
	void render();
};