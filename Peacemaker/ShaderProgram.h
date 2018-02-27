#pragma once

#include <string>
#include <glm.hpp>
#include <gl\glew.h>


class ShaderProgram
{
private:
	int programID;
	int vertexShaderID;
	int fragmentShaderID;


public:
	ShaderProgram(std::string vertexFile, std::string fragmentFile);

	void start();
	void stop();

	void loadFloat(int location, float value);
	void loadInt(int location, int value);
	void loadVector(int location, glm::vec3 value);
	void load2DVector(int location, glm::vec2 value);
	void loadMatrix(int location, glm::mat4 value);


};