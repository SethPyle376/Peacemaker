#pragma once

#include <GL\glew.h>
#include <glm.hpp>
#include "ShaderProgram.h"

class Rectangle
{
private:
	GLuint VBO;
	GLuint VAO;
	
	GLfloat *vertices;
	GLuint texture;

	ShaderProgram *shader;

	glm::vec3 color;
	glm::vec2 position;
	glm::vec2 size;
	glm::vec2 resolution;

public:
	Rectangle(glm::vec2 position, glm::vec2 size, glm::vec2 resolution, glm::vec3 color);

	void setTexture(GLuint texture);

	void draw();
};