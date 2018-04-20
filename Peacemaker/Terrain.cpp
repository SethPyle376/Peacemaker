#include "Terrain.h"

Terrain::Terrain(Scene *scene)
{
	this->scene = scene;

	shader = new ShaderProgram("res/shaders/terrainVertex.glsl", "res/shaders/terrainFragment.glsl");

	noise = new FastNoise(1337);
	noise->SetFrequency(0.05f);

	int count = 0;

	float minimum = 100;
	float maximum = 0;
	float mean = 0;

	for (int x = 0; x < 200; x++)
	{
		for (int y = 0; y < 200; y++)
		{
			TerrainVertex vertex;
			vertex.position = glm::vec3(x - 100, noise->GetPerlinFractal(x, y) * 10, y - 100);

			if (vertex.position.y > maximum)
				maximum = vertex.position.y;

			if (vertex.position.y < minimum)
				minimum = vertex.position.y;

			mean += vertex.position.y;
			count++;

			if (vertex.position.y > 1)
				vertex.color = glm::vec3(0.0, 0.0, 1.0);
			else if (vertex.position.y < -2)
				vertex.color = glm::vec3(1.0, 0.0, 0.0);
			else
				vertex.color = glm::vec3(0.0, 1.0, 0.0);

			vertices.push_back(vertex);
		}
	}

	mean /= count;

	std::cout << "TERRAIN MAXIMUM: " << maximum << std::endl;
	std::cout << "TERRAIN MINIMUM: " << minimum << std::endl;
	std::cout << "TERRAIN MEAN: " << mean << std::endl;

	for (int x = 0; x < 199; x++)
	{
		for (int y = 0; y < 199; y++)
		{
			
			indices.push_back(((x + 1) * 200) + y + 1);
			indices.push_back(((x + 1) * 200) + y);
			indices.push_back((x * 200) + y);

			indices.push_back((x * 200) + y + 1);
			indices.push_back(((x + 1) * 200) + y + 1);
			indices.push_back((x * 200) + y);

		}
	}

	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &this->vbo);
	glGenBuffers(1, &this->ebo);

	glBindVertexArray(this->vao);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(TerrainVertex), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (GLvoid *)offsetof(TerrainVertex, color));
	glBindVertexArray(0);
}

void Terrain::render()
{
	shader->start();

	shader->loadMatrix(glGetUniformLocation(shader->getProgramID(), "projection"), scene->camera->getProjectionMatrix());
	shader->loadMatrix(glGetUniformLocation(shader->getProgramID(), "view"), scene->camera->getViewMatrix());

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (GLvoid *)offsetof(TerrainVertex, color));

	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
