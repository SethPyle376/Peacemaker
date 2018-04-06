#pragma once

#include <string>
#include <map>

#include <glm.hpp>
#include <ft2build.h>
#include <freetype/freetype.h>
#include "GL\glew.h"
#include <gtc\matrix_transform.hpp>

#include "ShaderProgram.h"

struct Character
{
	GLuint textureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	GLuint Advance;
};

class Text
{
private:

	std::map<GLchar, Character> Characters;

	GLuint vao, vbo;

	FT_Library ft;
	FT_Face face;

	glm::mat4 projection;

public:
	Text();

	void renderText(ShaderProgram *shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

};