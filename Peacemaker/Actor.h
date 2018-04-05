#pragma once

#include <string>

#include "Model.h"

#include "ShaderProgram.h"
#include "Scene.h"

#include "GL\glew.h"

//Forward definition of cross referenced class
class Scene;

class Actor
{
private:
	Model *model;
	ShaderProgram *shader;
	Scene *scene;

	glm::mat4 modelMatrix;

	GLuint matrixID;
	GLuint modelMatrixID;
	GLuint viewMatrixID;
	GLuint lightID;
	

public:
	Actor(std::string modelLocation, std::string vertexLocation, std::string fragmentLocation, Scene *scene);

	virtual void render();
};