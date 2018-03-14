#pragma once
#include <string>
#include <iostream>

#include "GL\glew.h"
class WaterFrameBuffer
{
private:
	int reflectionWidth;
	int reflectionHeight;

	int refractionWidth;
	int refractionHeight;

	GLuint reflectionFBO;
	GLuint reflectionTexture;
	GLuint reflectionDepthBuffer;

	GLuint refractionFBO;
	GLuint refractionTexture;
	GLuint refractionDepthTexture;

	GLuint createFrameBuffer();
	GLuint createTextureAttachment(int width, int height);
	GLuint createDepthTextureAttachment(int width, int height);
	GLuint createDepthBufferAttachment(int width, int height);

	void initializeReflection();
	void initializeRefraction();

	void bindFrameBuffer(GLuint frameBuffer, int width, int height);

public:
	WaterFrameBuffer();

	void bindReflection();
	void bindRefraction();
	void unbindFBO();

	GLuint getReflectionTexture();
	GLuint getRefractionTexture();
	GLuint getRefractionDepthTexture();
};