#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include "controller.h"

#include "Tester.h"
#include "ShaderManager.h"
#include "BMPLoader.h"
#include "OBJLoader.h"
#include "vboindexer.h"

using namespace std;

double rotateX = 0;
double rotateY = 0;
int height;
int width;
float posX = 0;
float posY = 0;
float posZ = 0;

static const GLfloat vertexBuffer[] = {
	-1.0, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		posX -= 1;

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		posX += 1;

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		posZ -= 1;

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		posZ += 1;

	cout << "X: " << rotateX << " Y: " << rotateY << endl;
}

int main()
{
	GLFWwindow *window = NULL;
	const GLubyte *renderer;
	const GLubyte *version;

	ShaderManager *shaderManager = new ShaderManager();


	GLuint vao;
	GLuint vbo;

	GLuint uvo;

	width = 1024;
	height = 768;
	
	glfwInit();
	window = glfwCreateWindow(width, height, "TEST", NULL, NULL);

	if (!window)
	{
		cout << "ERROR OPENING WINDOW" << endl;
	}

	Tester *tester = new Tester();
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool res = loadObj("C:/ForgottenServer/Peacemaker/x64/Debug/test.obj", vertices, uvs, normals);

	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

	GLuint programID = shaderManager->LoadShaders("C:/ForgottenServer/Peacemaker/x64/Debug/vertex.glsl", "C:/ForgottenServer/Peacemaker/x64/Debug/fragment.glsl");

	std::cout << "VERTEX COUNT: " << vertices.size() << std::endl;
	std::cout << "PACKED VERTEX COUNT: " << indexed_vertices.size() << std::endl;


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvo);
	glBindBuffer(GL_ARRAY_BUFFER, uvo);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	GLuint normalBuffer;
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	// Generate a buffer for the indices as well
	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	renderer = glGetString(GL_RENDERER);
	version = glGetString(GL_VERSION);

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	GLuint texture = loadDDS("C:/ForgottenServer/Peacemaker/x64/Debug/uvmap.dds");
	GLuint textureID = glGetUniformLocation(programID, "myTextureSampler");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);

		computeMatricesFromInputs(window);
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		glm::vec3 lightPos = glm::vec3(4, 4, 4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(textureID, 0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvo);
		glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glVertexAttribPointer(
			2,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		
		glDrawElements(
			GL_TRIANGLES,
			indices.size(),
			GL_UNSIGNED_SHORT,
			(void*)0
		);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);


		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			break;
		}

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}