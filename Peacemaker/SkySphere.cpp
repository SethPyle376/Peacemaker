#include "SkySphere.h"

SkySphere::SkySphere(float radius, int recursionLevel, Scene *scene)
{
	std::cout << "STARTING SKYSPHERE INIT" << std::endl;
	shader = new ShaderProgram("res/shaders/basicVert.glsl", "res/shaders/basicFrag.glsl");
	this->scene = scene;
	this->radius = radius;

	float t = (1.0f + sqrt(5.0f)) / 2.0f;

	addVertex(glm::vec3(-1, t, 0));
	addVertex(glm::vec3(1, t, 0));
	addVertex(glm::vec3(-1, -t, 0));
	addVertex(glm::vec3(1, -t, 0));

	addVertex(glm::vec3(0, -1, t));
	addVertex(glm::vec3(0, 1, t));
	addVertex(glm::vec3(0, -1, -t));
	addVertex(glm::vec3(0, 1, -t));

	addVertex(glm::vec3(t, 0, -1));
	addVertex(glm::vec3(t, 0, 1));
	addVertex(glm::vec3(-t, 0, -1));
	addVertex(glm::vec3(-t, 0, 1));

	triangles.push_back(Triangle(0, 11, 5));
	triangles.push_back(Triangle(0, 5, 1));
	triangles.push_back(Triangle(0, 1, 7));
	triangles.push_back(Triangle(0, 7, 10));
	triangles.push_back(Triangle(0, 10, 11));

	triangles.push_back(Triangle(1, 5, 9));
	triangles.push_back(Triangle(5, 11, 4));
	triangles.push_back(Triangle(11, 10, 2));
	triangles.push_back(Triangle(10, 7, 6));
	triangles.push_back(Triangle(7, 1, 8));

	triangles.push_back(Triangle(3, 9, 4));
	triangles.push_back(Triangle(3, 4, 2));
	triangles.push_back(Triangle(3, 2, 6));
	triangles.push_back(Triangle(3, 6, 8));
	triangles.push_back(Triangle(3, 8, 9));

	triangles.push_back(Triangle(4, 9, 5));
	triangles.push_back(Triangle(2, 4, 11));
	triangles.push_back(Triangle(6, 2, 10));
	triangles.push_back(Triangle(8, 6, 7));
	triangles.push_back(Triangle(9, 8, 1));

	for (int i = 0; i < recursionLevel; i++)
	{
		std::vector<Triangle> newTriangles;

		for (int i = 0; i < triangles.size(); i++)
		{
			int a = getMiddlePoint(triangles[i].v0, triangles[i].v1);
			int b = getMiddlePoint(triangles[i].v1, triangles[i].v2);
			int c = getMiddlePoint(triangles[i].v2, triangles[i].v0);

			newTriangles.push_back(Triangle(triangles[i].v0, a, c));
			newTriangles.push_back(Triangle(triangles[i].v1, b, a));
			newTriangles.push_back(Triangle(triangles[i].v2, c, b));
			newTriangles.push_back(Triangle(a, b, c));
		}
		triangles = newTriangles;
		std::cout << "SPHERE CONTAINS: " << triangles.size() << " AFTER LOOP " << i << std::endl;
	}

	for (int i = 0; i < triangles.size(); i++)
	{
		indices.push_back(triangles[i].v0);
		indices.push_back(triangles[i].v1);
		indices.push_back(triangles[i].v2);
	}

	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &this->vbo);
	glGenBuffers(1, &this->ebo);

	glBindVertexArray(this->vao);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glBindVertexArray(0);

}

int SkySphere::addVertex(glm::vec3 vertex)
{
	double length = sqrt(vertex.x * vertex.x + vertex.y * vertex.y + vertex.z * vertex.z) / radius;
	vertices.push_back(glm::vec3(vertex.x / length, vertex.y / length, vertex.z / length));
	return vertices.size() - 1;
}

int SkySphere::getMiddlePoint(int p1, int p2)
{
	bool firstSmaller = p1 < p2;

	int64_t smallerIndex = firstSmaller ? p1 : p2;
	int64_t greaterIndex = firstSmaller ? p2 : p1;
	int64_t key = (smallerIndex << 32) + greaterIndex;
	
	/*if (middlePointCache.count(key))
	{
		return middlePointCache[key];
	}*/

	glm::vec3 point1 = vertices[p1];
	glm::vec3 point2 = vertices[p2];
	glm::vec3 middle = glm::vec3((point1.x + point2.x) / 2, (point1.y + point2.y) / 2, (point1.z + point2.z) / 2);

	int index = addVertex(middle);
	middlePointCache[key, index];
	return index;
}

void SkySphere::render()
{
	glDisable(GL_CULL_FACE);

	shader->start();

	shader->loadMatrix(glGetUniformLocation(shader->getProgramID(), "projection"), scene->camera->getProjectionMatrix());
	shader->loadMatrix(glGetUniformLocation(shader->getProgramID(), "view"), scene->camera->getViewMatrix());

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glEnable(GL_CULL_FACE);

}

