#include "Rectangle.h"

Rectangle::Rectangle(glm::vec2 position, glm::vec2 size, glm::vec2 resolution, glm::vec3 color)
{
	this->color = color;

	glm::vec2 topLeft;
	glm::vec2 topRight;
	glm::vec2 bottomLeft;
	glm::vec2 bottomRight;

	topLeft.x = -1.0f + (position.x / ((1.0f / 2.0f) * resolution.x));
	topLeft.y = -1.0f + (position.y / ((1.0f / 2.0f) * resolution.y));

	bottomLeft.x = topLeft.x;
	bottomLeft.y = (-1.0f + ((position.y - size.y) / ((1.0f / 2.0f) * resolution.y)));

	topRight.x = -1.0f + ((position.x + size.x) / ((1.0f / 2.0f) * resolution.x));
	topRight.y = topLeft.y;

	bottomRight.x = topRight.x;
	bottomRight.y = bottomLeft.y;

	std::cout << "TOP LEFT" << topLeft.x << ":" << topLeft.y << std::endl;
	std::cout << "Bottom LEFT" << bottomLeft.x << ":" << bottomLeft.y << std::endl;
	std::cout << "TOP RIGHT" << topRight.x << ":" << topRight.y << std::endl;
	std::cout << "Bottom RIGHT" << bottomRight.x << ":" << bottomRight.y << std::endl;

	const static GLfloat vertices[24]{
		topLeft.x, topLeft.y, 0.0f, 1.0f,
		bottomLeft.x, bottomLeft.y, 0.0f, 0.0f,
		bottomRight.x, bottomRight.y, 1.0f, 0.0f,

		topLeft.x, topLeft.y, 0.0f, 1.0f,
		bottomRight.x, bottomRight.y, 1.0f, 0.0f,
		topRight.x, topRight.y, 1.0f, 1.0f
	};

	shader = new ShaderProgram("res/shaders/rectangleVertex.glsl", "res/shaders/rectangleFragment.glsl");

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void Rectangle::draw()
{
	shader->start();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	shader->loadInt(glGetUniformLocation(shader->getProgramID(), "myTexture"), 0);
	shader->loadVector(glGetUniformLocation(shader->getProgramID(), "color"), color);

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

void Rectangle::offsetPosition(glm::vec2 position)
{

}

void Rectangle::updateSize(glm::vec2 size)
{

}

void Rectangle::updateColor(glm::vec3 color)
{
	this->color = color;
}
