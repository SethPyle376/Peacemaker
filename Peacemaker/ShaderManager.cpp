#include "ShaderManager.h"

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path)
{
	GLuint VertexID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertexCode;
	std::ifstream vertexStream(vertex_file_path, std::ios::in);

	if (vertexStream.is_open())
	{
		std::string Line = "";
		while (getline(vertexStream, Line))
			vertexCode += Line + "\n";
		vertexStream.close();
	}
	else
	{
		std::cout << "ERROR LOADING VERTEX SHADER" << std::endl;
		getchar();
		return 0;
	}

	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
	else
	{
		std::cout << "ERROR LOADING FRAGMENT SHADER" << std::endl;
	}

	GLint Result = GL_FALSE;
	int logLength;

	std::cout << "COMPILING VERTEX SHADER" << std::endl;

	char const * vertexSourcePointer = vertexCode.c_str();
	glShaderSource(VertexID, 1, &vertexSourcePointer, NULL);
	glCompileShader(VertexID);

	glGetShaderiv(VertexID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexID, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		std::cout << "ERROR COMPILING VERTEX SHADER" << std::endl;
	}

	const char *fragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentID, 1, &fragmentSourcePointer, NULL);
	glCompileShader(FragmentID);

	glGetShaderiv(FragmentID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentID, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		std::cout << "ERROR COMPILING FRAGMENT SHADER" << std::endl;
	}

	std::cout << "LINKING SHADER PROGRAMS" << std::endl;
	GLuint programID = glCreateProgram();
	glAttachShader(programID, VertexID);
	glAttachShader(programID, FragmentID);
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		std::cout << "ERROR LINKING SHADERS" << std::endl;
		std::vector<char> ProgramErrorMessage(logLength + 1);
		glGetProgramInfoLog(programID, logLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(programID, VertexID);
	glDetachShader(programID, FragmentID);

	glDeleteShader(VertexID);
	glDeleteShader(FragmentID);
	return programID;
}