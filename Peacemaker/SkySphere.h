#pragma once

#include <iostream>
#include <vector>
#include <map>

#include "GL\glew.h"
#include <glm.hpp>

#include "ShaderProgram.h"
#include "Scene.h"


class Scene;

struct Triangle
{
	int v0;
	int v1;
	int v2;

	Triangle(int v0, int v1, int v2)
	{
		this->v0 = v0;
		this->v1 = v1;
		this->v2 = v2;
	}
};

class SkySphere
{
private:
	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	std::vector<Triangle> triangles;

	std::map<int64_t, int> middlePointCache;

	int addVertex(glm::vec3 vertex);
	int getMiddlePoint(int p1, int p2);

	Scene *scene;

	ShaderProgram *shader;

	float radius;


public:
	SkySphere(float radius, int recursionLevel, Scene *scene);

	float timeOfDay;

	void render();


};