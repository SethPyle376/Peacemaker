#pragma once
#include <vector>
#include "glm.hpp"

#include "Renderer.h"
#include "vboindexer.h"
#include "OBJLoader.h"
#include "BMPLoader.h"

#include "glm.hpp"
#include <GLFW\glfw3.h>

class Actor
{
private:
	//Provide access to vertices for renderer
	friend class Renderer;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;

	GLuint texture;

	void loadModel(const char *path);
	void loadTexture(const char *path);

public:
	float x, y, z;
};