#pragma once

#include <vector>
#include <iostream>

#include <glm.hpp>
#include "GL\glew.h"

#include "FastNoise.h"
#include "ShaderProgram.h"
#include "Scene.h"

class Scene;

struct TerrainVertex
{
	glm::vec3 position;
	glm::vec3 color;
};

class Terrain
{
private:
	std::vector<TerrainVertex> vertices;
	std::vector<GLuint> indices;

	FastNoise *noise;

	GLuint vbo;
	GLuint vao;
	GLuint ebo;

	ShaderProgram *shader;

	Scene *scene;

public:
	Terrain(Scene *scene);

	void render();

	
};