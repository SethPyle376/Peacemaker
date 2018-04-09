#pragma once

#include <vector>

#include <glm.hpp>

#include "ShaderProgram.h"

#include <GL\glew.h>

class Rectangle
{
private:
	glm::vec2 position;
	int size;


public:
	Rectangle(int x, int y, int size);

	void draw(ShaderProgram *program);
};