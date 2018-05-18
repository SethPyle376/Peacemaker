#include "Terrain.h"

Terrain::Terrain(Scene *scene, int dimension)
{
	this->scene = scene;

	shader = new ShaderProgram("res/shaders/terrainVertex.glsl", "res/shaders/terrainFragment.glsl");

	noise = new FastNoise(3760);
	
	int amplitude = 60;
	int count = 0;
	int size = 5;

	this->dimension = dimension;
	

	float minimum = 100;
	float maximum = 0;
	float mean = 0;

	float totalDif;

	float bottom = 0;
	float coast = 2.5f;
	float middle = 0;
	float top = 0;

	bottom = (-(1.0f / 2.0f) * amplitude) + (1.0f / 3.0f) * amplitude;

	middle = bottom + (1.0f / 3.0f) * amplitude;

	top = middle + (1.0f / 3.0f) * amplitude;

	std::cout << "Top: " << top << std::endl;
	std::cout << "Middle: " << middle << std::endl;
	std::cout << "Bottom: " << bottom << std::endl;

	for (int x = 0; x < dimension; x++)
	{
		for (int y = 0; y < dimension; y++)
		{
			noise->SetFrequency(0.00525f);
			noise->SetFractalOctaves(7);
			int biome = 0;
			TerrainVertex vertex;
			vertex.position = glm::vec3((x - (dimension / 2)), noise->GetPerlinFractal(x, y) * amplitude, (y - (dimension / 2)));

			if (vertex.position.y > maximum)
				maximum = vertex.position.y;

			if (vertex.position.y < minimum)
				minimum = vertex.position.y;

			mean += vertex.position.y;
			count++;

			if (vertex.position.y > middle)
			{
				vertex.color = glm::vec3(0.0, 0.0, 1.0);
				biome = 0;
			}
			else if (vertex.position.y < bottom)
			{
				vertex.color = glm::vec3(1.0, 0.0, 0.0);
				biome = 1;
			}
			else
			{
				vertex.color = glm::vec3(0.0, 1.0, 0.0);
				biome = 2;
			}

			if (vertex.position.y > bottom && (vertex.position.y < bottom + coast))
			{
				vertex.color = glm::vec3(1.0f, 1.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}
	}

	mean /= count;

	std::cout << "TERRAIN MAXIMUM: " << maximum << std::endl;
	std::cout << "TERRAIN MINIMUM: " << minimum << std::endl;
	std::cout << "TERRAIN MEAN: " << mean << std::endl;

	for (int x = 0; x < dimension - 1; x++)
	{
		for (int y = 0; y < dimension - 1; y++)
		{
			
			indices.push_back(((x + 1) * dimension) + y + 1);
			indices.push_back(((x + 1) * dimension) + y);
			indices.push_back((x * dimension) + y);

			indices.push_back((x * dimension) + y + 1);
			indices.push_back(((x + 1) * dimension) + y + 1);
			indices.push_back((x * dimension) + y);

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

void Terrain::reload()
{
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vao);

	vertices.clear();
	indices.clear();


	noise = new FastNoise(seed);

	int amplitude = 60;
	int count = 0;


	float minimum = 100;
	float maximum = 0;
	float mean = 0;

	float totalDif;

	float bottom = 0;
	float middle = 0;
	float top = 0;

	bottom = (-(1.0f / 2.0f) * amplitude) + (1.0f / 3.0f) * amplitude;

	middle = bottom + (1.0f / 3.0f) * amplitude;

	top = middle + (1.0f / 3.0f) * amplitude;

	std::cout << "Top: " << top << std::endl;
	std::cout << "Middle: " << middle << std::endl;
	std::cout << "Bottom: " << bottom << std::endl;

	for (int x = 0; x < dimension; x++)
	{
		for (int y = 0; y < dimension; y++)
		{
			noise->SetFrequency(0.00525f);
			noise->SetFractalOctaves(7);
			int biome = 0;
			TerrainVertex vertex;
			vertex.position = glm::vec3((x - (dimension / 2)), noise->GetPerlinFractal(x, y) * amplitude, (y - (dimension / 2)));

			if (vertex.position.y > maximum)
				maximum = vertex.position.y;

			if (vertex.position.y < minimum)
				minimum = vertex.position.y;

			mean += vertex.position.y;
			count++;

			if (vertex.position.y > middle)
			{
				vertex.color = glm::vec3(0.0, 0.0, 1.0);
				biome = 0;
			}
			else if (vertex.position.y < bottom)
			{
				vertex.color = glm::vec3(1.0, 0.0, 0.0);
				biome = 1;
			}
			else
			{
				vertex.color = glm::vec3(0.0, 1.0, 0.0);
				biome = 2;
			}
			vertices.push_back(vertex);
		}
	}

	mean /= count;

	std::cout << "TERRAIN MAXIMUM: " << maximum << std::endl;
	std::cout << "TERRAIN MINIMUM: " << minimum << std::endl;
	std::cout << "TERRAIN MEAN: " << mean << std::endl;

	for (int x = 0; x < dimension - 1; x++)
	{
		for (int y = 0; y < dimension - 1; y++)
		{

			indices.push_back(((x + 1) * dimension) + y + 1);
			indices.push_back(((x + 1) * dimension) + y);
			indices.push_back((x * dimension) + y);

			indices.push_back((x * dimension) + y + 1);
			indices.push_back(((x + 1) * dimension) + y + 1);
			indices.push_back((x * dimension) + y);

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
