#include "Scene.h"

Scene::Scene()
{
	camera = new Camera();
	skybox = new Skybox(1000, this);
}

void Scene::debugInput()
{

}

void Scene::update(GLFWwindow *window)
{
	camera->update(window, this);

	skybox->draw();

	for (int i = 0; i < actors.size(); i++)
		actors[i]->render();


}