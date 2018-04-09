#pragma once

#include <iostream>

#include "Scene.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include "Text.h"
#include "Rectangle.h"


class Renderer
{
private:
	GLFWwindow * window;
	FT_Library ft;
	FT_Face face;

	Rectangle *rectangle;
	Text *text;
	ShaderProgram *textShader;
	ShaderProgram *rectangleShader;
	int tickCount;
	int fps;

	int width;
	int height;
	

public:
	Renderer(int width, int height);

	void update(Scene *scene);

	GLFWwindow* getWindow();
};