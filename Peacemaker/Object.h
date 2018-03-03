#pragma once
#include <GL\glew.h>
#include <vector>

#include <string>
#include <iostream>

#include "OBJLoader.h"
#include "vboindexer.h"
#include "BMPLoader.h"


class Object
{
private:
	//Buffer handles
	GLuint vbo;
	GLuint vao;
	GLuint nbo;
	GLuint uvo;
	GLuint elementBuffer;

	//vertice data
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;

	//Texture info
	GLuint texture;
	GLuint textureID;

public:
	Object(std::string objectLocation, std::string textureLocation, GLuint shaderID);

	void translate(glm::vec3 movement);

	void render();

	GLuint getTexture();
	GLuint getTextureID();
};