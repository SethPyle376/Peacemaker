#include "Scene.h"

Scene::Scene(glm::vec2 resolution)
{
	camera = new Camera();
	skybox = new Skybox(2000, this);

	this->resolution = resolution;

	rectangle = new Rectangle(glm::vec2(0, 1080), glm::vec2(540, 270), resolution, glm::vec3(1.0f, 0.0f, 0.0f));

	terrain = new Terrain(this, 300);
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

	rectangle->draw();
}