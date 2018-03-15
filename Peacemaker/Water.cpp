#include "Water.h"

Water::Water(float height, float sizeWidth, float sizeHeight)
{
	for (int i = 1; i < 18; i += 3)
	{
		waterVertices[i] += height;
	}

	for (int i = 0; i < 18; i += 3)
	{
		waterVertices[i] *= sizeWidth;
	}

	for (int i = 2; i < 18; i += 3)
	{
		waterVertices[i] *= sizeHeight;
	}

	glGenVertexArrays(1, &waterVAO);
	glGenBuffers(1, &waterVBO);
	glBindVertexArray(waterVAO);
	glBindBuffer(GL_ARRAY_BUFFER, waterVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(waterVertices), &waterVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glBindVertexArray(0);
}

void Water::render(ShaderProgram shader)
{
	glBindVertexArray(waterVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
