 #pragma once

#include <string>
#include <glm.hpp>
#include <gl\glew.h>

#include "ShaderManager.h"


class ShaderProgram
{
private:
	int programID;
	int vertexShaderID;
	int fragmentShaderID;


public:
	ShaderProgram(std::string vertexFile, std::string fragmentFile);

	GLuint getProgramID();

	void start();
	void stop();

	void loadFloat(GLuint location, float value);
	void loadInt(GLuint location, int value);
	void loadVector(GLuint location, glm::vec3 value);
	void loadVector4(GLuint location, glm::vec4 value);
	void loadVec2(GLuint location, glm::vec2 value);
	void loadMatrix(GLuint location, glm::mat4 value);


};