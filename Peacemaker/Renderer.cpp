#include "Renderer.h"

Renderer::Renderer(int width, int height)
{
	this->width = width;
	this->height = height;
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

	text = new Text(width, height);
	rectangle = new Rectangle(0, 0, 0);
	textShader = new ShaderProgram("res/shaders/textVertex.glsl", "res/shaders/textFragment.glsl");
	rectangleShader = new ShaderProgram("res/shaders/rectangleVertex.glsl", "res/shaders/rectangleFragment.glsl");
}

void Renderer::update(Scene *scene)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->update(window);

	tickCount++;

	if (tickCount > 100)
	{
		fps = (int)scene->camera->currentFrames;
		tickCount = 0;
	}

	text->renderText(textShader, std::to_string(fps) + " FPS", 0.0f, height - 48, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	glfwPollEvents();
	glfwSwapBuffers(window);
}

GLFWwindow* Renderer::getWindow()
{
	return window;
}