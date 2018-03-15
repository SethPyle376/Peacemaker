#pragma once

#include "GL\glew.h"
#include <glm.hpp>

#include "ShaderProgram.h"

class Water
{
	GLfloat waterVertices[18] = {
		-1, 0, -1,
		-1, 0, 1,
		1, 0, -1,
		1, 0, -1,
		-1, 0, 1,
		1, 0, 1
	};

	GLuint waterVAO;
	GLuint waterVBO;

public:
	Water(float height, float sizeWidth, float sizeHeight);

	void render(ShaderProgram shader);

};