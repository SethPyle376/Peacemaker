#pragma once
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <map>

#include "Camera.h"
#include "ShaderProgram.h"
#include "Skybox.h"
#include "Scene.h"
#include "Renderer.h"
#include "Light.h"

#include <ft2build.h>
#include <freetype/freetype.h>


using namespace std;

int main()
{
	Renderer *renderer = new Renderer(1920, 1080);

	Scene *scene = new Scene(glm::vec2(renderer->width, renderer->height));

	Light *light = new Light(glm::vec3(2.0f, 3.0f, 4.0f));

	Actor *sponza = new Actor("res/models/islandsmall.obj", "res/shaders/vertex.glsl", "res/shaders/fragment.glsl", scene);

	scene->lights.push_back(light);
	scene->actors.push_back(sponza);

	while (!glfwWindowShouldClose(renderer->getWindow()))
	{
		renderer->update(scene);
	}

	//Kill
	glfwTerminate();
	return 0;
}



