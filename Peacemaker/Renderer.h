#pragma once

#include <iostream>

#include "Scene.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"

class Renderer
{
private:
	GLFWwindow * window;

	

public:
	Renderer(int width, int height);

	void update(Scene *scene);

	GLFWwindow* getWindow();
};