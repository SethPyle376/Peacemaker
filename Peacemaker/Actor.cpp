#include "Actor.h"

Actor::Actor(std::string modelLocation, std::string vertexLocation, std::string fragmentLocation, Scene *scene)
{
	blinn = true;
	this->model = new Model((GLchar*)modelLocation.c_str());
	shader = new ShaderProgram(vertexLocation, fragmentLocation);
	shadowShader = new ShaderProgram("res/shaders/shadowVertex.glsl", "res/shaders/shadowFragment.glsl");
	this->scene = scene;

	modelMatrix = glm::mat4(1.0f);

	matrixID = glGetUniformLocation(shader->getProgramID(), "projection");
	viewMatrixID = glGetUniformLocation(shader->getProgramID(), "view");

	lightID = glGetUniformLocation(shader->getProgramID(), "lightPos");

	camPos = glGetUniformLocation(shader->getProgramID(), "viewPos");
	blinnPos = glGetUniformLocation(shader->getProgramID(), "blinn");

}

void Actor::render()
{
	shader->start();
	shader->loadMatrix(matrixID, scene->camera->getProjectionMatrix());
	shader->loadMatrix(viewMatrixID, scene->camera->getViewMatrix());

	shader->loadVector(lightID, scene->lights[0]->getPosition());
	shader->loadVector(camPos, scene->camera->getPosition());
	shader->loadBool(blinnPos, blinn);

	model->Draw(shader);

	shader->stop();
}

void Actor::renderShadows()
{
	shadowShader->start();
	shadowShader->loadMatrix(glGetUniformLocation(shadowShader->getProgramID(), "projection"), scene->camera->getProjectionMatrix());
	shadowShader->loadMatrix(glGetUniformLocation(shadowShader->getProgramID(), "view"), scene->camera->getViewMatrix());
	model->Draw(shadowShader);
	shadowShader->stop();
}