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
	modelMatrixID = glGetUniformLocation(shader->getProgramID(), "model");

	lightID = glGetUniformLocation(shader->getProgramID(), "lightSpaceMatrix");
}

void Actor::render(GLuint shadowMap)
{
	shader->start();
	shader->loadMatrix(matrixID, scene->camera->getProjectionMatrix());
	shader->loadMatrix(viewMatrixID, scene->camera->getViewMatrix());
	shader->loadMatrix(modelMatrixID, glm::mat4(1.0f));

	shader->loadMatrix(lightID, scene->lights[0]->lightSpaceMatrix);
	shader->loadVector(glGetUniformLocation(shader->getProgramID(), "lightPos"), scene->lights[0]->getPosition());
	shader->loadVector(glGetUniformLocation(shader->getProgramID(), "viewPos"), scene->camera->getPosition());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	model->Draw(shader);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	shader->stop();
}

void Actor::renderShadows()
{
	shadowShader->start();
	shadowShader->loadMatrix(glGetUniformLocation(shadowShader->getProgramID(), "lightSpaceMatrix"), scene->lights[0]->lightSpaceMatrix);
	shadowShader->loadMatrix(glGetUniformLocation(shadowShader->getProgramID(), "model"), glm::mat4(1.0f));
	model->Draw(shadowShader);
	shadowShader->stop();
}