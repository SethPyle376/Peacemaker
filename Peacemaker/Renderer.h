#pragma once

#include <iostream>

#include "Scene.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include "Text.h"


class Renderer
{
private:
	GLFWwindow * window;
	FT_Library ft;
	FT_Face face;

	Text *text;
	ShaderProgram *textShader;
	

public:
	Renderer(int width, int height);

	void update(Scene *scene);

	GLFWwindow* getWindow();
};