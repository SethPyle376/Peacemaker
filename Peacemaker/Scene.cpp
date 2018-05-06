#include "Scene.h"

Scene::Scene(glm::vec2 resolution, GLFWwindow *window)
{
	camera = new Camera();
	skybox = new Skybox(2000, this);

	this->resolution = resolution;

	rectangle = new Rectangle(glm::vec2(0, 1080), glm::vec2(540, 270), resolution, glm::vec3(1.0f, 0.0f, 0.0f));

	terrain = new Terrain(this, 300);

	controlPanel = new DemoUI(this, window);
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
	camera->update(window, this);
	skybox->draw();

	//for (int i = 0; i < actors.size(); i++)
	//{
		//actors[i]->render(shadowMap);
	//}

	terrain->render();

	controlPanel->render();
}