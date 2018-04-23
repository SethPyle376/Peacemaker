#pragma once

#include <iostream>

#include "Rectangle.h"
#include "Scene.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "ShadowFrameBuffer.h"

#include "Text.h"



class Renderer
{
private:
	GLFWwindow * window;

	Text *text;
	ShaderProgram *textShader;
	ShaderProgram *rectangleShader;
	Rectangle *rectangle;

	ShadowFrameBuffer *shadows;

	int tickCount;
	int fps;

	
	

public:
	Renderer(int width, int height);

	int width;
	int height;

	void update(Scene *scene);

	GLFWwindow* getWindow();
};