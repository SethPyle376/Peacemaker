#pragma once

#include <vector>
#include <glm.hpp>
#include <GL\glew.h>

class UIElement
{

protected:
	glm::vec2 position;
	glm::vec2 size;

	UIElement *parent;
	std::vector<UIElement*> children;


public:
	UIElement(glm::vec2 position, glm::vec2 size);
	virtual void offsetPosition(glm::vec2 update) = 0;
	virtual void render() = 0;
};