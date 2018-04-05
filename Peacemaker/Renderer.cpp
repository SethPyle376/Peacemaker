#include "Renderer.h"

Renderer::Renderer(int width, int height)
{
	glfwInit();

	window = glfwCreateWindow(width, height, "Peacemaker", NULL, NULL);

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
	glEnable(GL_CULL_FACE);
}

void Renderer::update(Scene *scene)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->update(window);

	glfwPollEvents();
	glfwSwapBuffers(window);
}

GLFWwindow* Renderer::getWindow()
{
	return window;
}