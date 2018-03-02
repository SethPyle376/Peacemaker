#pragma once
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Object.h"
#include <gtc\matrix_transform.hpp>
#include "controller.h"

#include "ShaderManager.h"



//Savannah was here

using namespace std;

double rotateX = 0;
double rotateY = 0;
int height;
int width;
float posX = 0;
float posY = 0;
float posZ = 0;

Object* monkey;
Object* monkeyTwo;

static const GLfloat vertexBuffer[] = {
	-1.0, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};


int main()
{
	float deltaTime;
	float speed = 3;
	GLFWwindow *window = NULL;
	const GLubyte *renderer;
	const GLubyte *version;

	ShaderManager *shaderManager = new ShaderManager();

	width = 1920;
	height = 1080;
	
	glfwInit();
	window = glfwCreateWindow(width, height, "TEST", NULL, NULL);

	if (!window)
	{
		cout << "ERROR OPENING WINDOW" << endl;
	}

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

	GLuint programID = shaderManager->LoadShaders("vertex.glsl", "fragment.glsl");

	monkey = new Object("test.obj", "uvmap.dds", programID);
	monkeyTwo = new Object("test.obj", "uvmap.dds", programID);

	renderer = glGetString(GL_RENDERER);
	version = glGetString(GL_VERSION);

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);

		deltaTime = computeMatricesFromInputs(window);
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
		glBindTexture(GL_TEXTURE_2D, monkey->getTexture());
		glUniform1i(monkey->getTextureID(), 0);

		monkey->render();
		monkeyTwo->render();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			break;
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			glm::vec3 translation(0, speed * deltaTime, 0);
			monkey->translate(translation);
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			glm::vec3 translation(0, -speed * deltaTime, 0);
			monkey->translate(translation);
		}
		else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			glm::vec3 translation(-speed * deltaTime, 0, 0);
			monkey->translate(translation);
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			glm::vec3 translation(speed * deltaTime, 0, 0);
			monkey->translate(translation);
		}
		else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			glm::vec3 translation(0, 0, -speed * deltaTime);
			monkey->translate(translation);
		}
		else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			glm::vec3 translation(0, 0, speed * deltaTime);
			monkey->translate(translation);
		}

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}