#pragma once

#include "UIElement.h"
#include "Rectangle.h"

#include <glm.hpp>

class Frame : public UIElement
{
private:
	Rectangle *rectangle;
public:
	Frame(glm::vec2 position, glm::vec2 size);
};