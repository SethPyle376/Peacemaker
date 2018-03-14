#pragma once
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Object.h"
#include <gtc\matrix_transform.hpp>
#include "controller.h"



#include "ShaderProgram.h"
#include "Model.h"
#include "Skybox.h"



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
Object* terrain;

static const GLfloat vertexBuffer[] = {
	-1.0, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};


int main()
{
	float deltaTime;
	float speed = 30;
	GLFWwindow *window = NULL;
	const GLubyte *renderer;
	const GLubyte *version;

	width = 1920;
	height = 1080;
	

	glfwInit();
	window = glfwCreateWindow(width, height, "TEST", NULL, NULL);

	glm::vec3 lightPos = glm::vec3(0, 50, 50);

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

	ShaderProgram *shader = new ShaderProgram("vertex.glsl", "fragment.glsl");
	ShaderProgram *skyboxShader = new ShaderProgram("skyboxVertex.glsl", "skyboxFragment.glsl");

	//Model *model = new Model("res/models/nanosuit.obj");
	Model *model = new Model("islandsmall.obj");
	Skybox *skybox = new Skybox(750);

	//Get renderer version
	renderer = glGetString(GL_RENDERER);
	version = glGetString(GL_VERSION);

	//Get shader variable locations, need to encapsulate this into shader class later.
	GLuint MatrixID = glGetUniformLocation(shader->getProgramID(), "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(shader->getProgramID(), "V");
	GLuint ModelMatrixID = glGetUniformLocation(shader->getProgramID(), "M");
	GLuint LightID = glGetUniformLocation(shader->getProgramID(), "LightPosition_worldspace");

	//Enable depth testing and backface culling
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	GLuint skyView = glGetUniformLocation(skyboxShader->getProgramID(), "view");
	GLuint skyProj = glGetUniformLocation(skyboxShader->getProgramID(), "projection");

	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		//Clear frame and set to default color (navy blue)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Compute perspective matrices and return deltaTime
		deltaTime = computeMatricesFromInputs(window);
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = computeMVP();

		//Start shader and hand it perspective/light info
		shader->start();
		shader->loadMatrix(MatrixID, MVP);
		shader->loadMatrix(ModelMatrixID, ModelMatrix);
		shader->loadMatrix(ViewMatrixID, getViewMatrix());
		shader->loadVector(LightID, lightPos);

		//Stop shader, not totally needed.
		model->Draw(*shader);
		shader->stop();

		skyboxShader->start();
		shader->loadMatrix(skyView, getViewMatrix());
		shader->loadMatrix(skyProj, getProjectionMatrix());

		skybox->draw(*skyboxShader);
		skyboxShader->stop();

		//Input stuff for moving objects around, should probably encapsulate
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			break;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			glm::vec3 translation(0, speed * deltaTime, 0);
			//monkey->translate(translation);

			lightPos = glm::vec3(lightPos.x, lightPos.y + speed * deltaTime, lightPos.z);
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			glm::vec3 translation(0, -speed * deltaTime, 0);
			//monkey->translate(translation);
			lightPos = glm::vec3(lightPos.x, lightPos.y - speed * deltaTime, lightPos.z);
		}
		else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			glm::vec3 translation(-speed * deltaTime, 0, 0);
			//monkey->translate(translation);
			lightPos = glm::vec3(lightPos.x - speed * deltaTime, lightPos.y, lightPos.z);
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			glm::vec3 translation(speed * deltaTime, 0, 0);
			//monkey->translate(translation);
			lightPos = glm::vec3(lightPos.x + speed * deltaTime, lightPos.y, lightPos.z);
		}
		else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			glm::vec3 translation(0, 0, -speed * deltaTime);
			//monkey->translate(translation);
			lightPos = glm::vec3(lightPos.x, lightPos.y, lightPos.z - speed * deltaTime);
		}
		else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			glm::vec3 translation(0, 0, speed * deltaTime);
			//monkey->translate(translation);
			lightPos = glm::vec3(lightPos.x, lightPos.y, lightPos.z + speed * deltaTime);
		}

		//Handle inputs and swap buffer to screen
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//Kill
	glfwTerminate();
	return 0;
}