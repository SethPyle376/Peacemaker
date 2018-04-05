#include "Actor.h"

Actor::Actor(std::string modelLocation, std::string vertexLocation, std::string fragmentLocation, Scene *scene)
{
	this->model = new Model((GLchar*)modelLocation.c_str());
	shader = new ShaderProgram(vertexLocation, fragmentLocation);
	this->scene = scene;

	modelMatrix = glm::mat4(1.0f);

	matrixID = glGetUniformLocation(shader->getProgramID(), "MVP");
	viewMatrixID = glGetUniformLocation(shader->getProgramID(), "V");
	modelMatrixID = glGetUniformLocation(shader->getProgramID(), "M");
	lightID = glGetUniformLocation(shader->getProgramID(), "LightPosition_worldspace");

}

void Actor::render()
{
	shader->start();
	shader->loadMatrix(matrixID, scene->camera->getTransformationMatrix());
	shader->loadMatrix(modelMatrixID, modelMatrix);
	shader->loadMatrix(viewMatrixID, scene->camera->getViewMatrix());
	shader->loadVector(lightID, scene->lights[0]->getPosition());

	model->Draw(shader);

	shader->stop();
}