#include "Scene.h"

Scene::Scene()
{
	camera = new Camera();
}

void Scene::debugInput()
{

}

void Scene::update(GLFWwindow *window)
{
	camera->update(window);

	for (int i = 0; i < actors.size(); i++)
		actors[i]->render();
}