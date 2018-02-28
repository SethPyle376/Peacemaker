#pragma once
#include <GL\glew.h>
#include <vector>

#include <string>
#include <iostream>

#include "OBJLoader.h"
#include "vboindexer.h"


class Object
{
private:
	GLuint vbo;
	GLuint vao;
	GLuint nbo;
	GLuint uvo;
	GLuint elementBuffer;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;

public:
	Object(std::string fileLocation);

	void translate(glm::vec3 movement);

	void render();

};