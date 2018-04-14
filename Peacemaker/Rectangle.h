#pragma once

#include <GL\glew.h>
#include "ShaderProgram.h"

class Rectangle
{
private:
	GLuint VBO;
	GLuint VAO;
	
	GLfloat *vertices;
	GLuint texture;

public:
	Rectangle();

	void setTexture(GLuint texture);

	void draw(ShaderProgram *shader);
};