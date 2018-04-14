#pragma once

#include <iostream>

#include <GL/glew.h>


class ShadowFrameBuffer
{
private:
	GLuint fbo;
	GLuint shadowMap;
	GLuint color;

	int width;
	int height;

public:
	ShadowFrameBuffer(int width, int height);
	~ShadowFrameBuffer();

	void bind();
	void unbind();
	GLuint getShadowMap();
};