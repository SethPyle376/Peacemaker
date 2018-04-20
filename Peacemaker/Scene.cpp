#include "Scene.h"

Scene::Scene()
{
	camera = new Camera();
	skybox = new Skybox(2000, this);

	terrain = new Terrain(this);
}

void Scene::debugInput()
{

}

void Scene::drawShadows()
{
	for (int i = 0; i < actors.size(); i++)
		actors[i]->renderShadows();
}

void Scene::update(GLFWwindow *window, GLuint shadowMap)
{
	skybox->draw();
	camera->update(window, this);

	//for (int i = 0; i < actors.size(); i++)
	//{
		//actors[i]->render(shadowMap);
	//}

	terrain->render();
}