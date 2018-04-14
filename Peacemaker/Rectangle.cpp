#include "Rectangle.h"

Rectangle::Rectangle()
{

	const static GLfloat vertices[24]{
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 1.0f, 1.0f, 1.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void Rectangle::draw(ShaderProgram *shader)
{
	shader->start();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	shader->loadInt(glGetUniformLocation(shader->getProgramID(), "myTexture"), 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	shader->stop();
}

void Rectangle::setTexture(GLuint texture)
{
	this->texture = texture;
}