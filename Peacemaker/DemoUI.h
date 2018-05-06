#pragma once

#include "Scene.h"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

class Scene;

class DemoUI
{
private:
	Scene * scene;
	GLFWwindow *window;

public:
	DemoUI(Scene *scene, GLFWwindow *window);

	void render();
};