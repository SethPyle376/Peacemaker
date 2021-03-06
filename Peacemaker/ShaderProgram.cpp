#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(std::string vertexFile, std::string fragmentFile)
{
	programID = LoadShaders(vertexFile.c_str(), fragmentFile.c_str());
}

void ShaderProgram::loadFloat(GLuint location, float value)
{
	glUniform1f(location, value);
}

void ShaderProgram::loadInt(GLuint location, GLuint value)
{
	glUniform1i(location, value);
}

void ShaderProgram::loadVector(GLuint location, glm::vec3 value)
{
	glUniform3f(location, value.x, value.y, value.z);
}

void ShaderProgram::loadVector4(GLuint location, glm::vec4 value)
{
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void ShaderProgram::loadVec2(GLuint location, glm::vec2 value)
{
}

void ShaderProgram::loadMatrix(GLuint location, glm::mat4 value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::loadBool(GLuint location, bool value)
{
	glUniform1i(location, value);
}

void ShaderProgram::start()
{
	glUseProgram(programID);
}

void ShaderProgram::stop()
{
	glUseProgram(0);
}

GLuint ShaderProgram::getProgramID()
{
	return programID;
}