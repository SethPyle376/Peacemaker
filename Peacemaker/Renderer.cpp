#include "Renderer.h"

Renderer::Renderer(int width, int height)
{
	this->width = width;
	this->height = height;
	glfwInit();

	window = glfwCreateWindow(width, height, "Peacemaker", NULL, NULL);

	//window = glfwCreateWindow(width, height, "Peacemaker", glfwGetPrimaryMonitor(), NULL);

	if (!window)
		std::cout << "ERROR OPENING OPENGL WINDOW" << std::endl;

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	glewInit();

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwPollEvents();
	glfwSetCursorPos(window, width / 2, height / 2);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);

	shadows = new ShadowFrameBuffer(width, height);
	text = new Text(width, height);
	textShader = new ShaderProgram("res/shaders/textVertex.glsl", "res/shaders/textFragment.glsl");
}

void Renderer::update(Scene *scene)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	tickCount++;

	if (tickCount > 100)
	{
		fps = (int)scene->camera->currentFrames;
		tickCount = 0;
	}

	//shadows->bind();
	//scene->drawShadows();
	//shadows->unbind();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	scene->update(window, shadows->getShadowMap());
	glPolygonMode(GL_FRONT, GL_FILL);

	glfwPollEvents();
	glfwSwapBuffers(window);
}

GLFWwindow* Renderer::getWindow()
{
	return window;
}