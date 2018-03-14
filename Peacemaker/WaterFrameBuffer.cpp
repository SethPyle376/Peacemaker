#include "WaterFrameBuffer.h"

WaterFrameBuffer::WaterFrameBuffer()
{
	initializeReflection();
	initializeRefraction();
}

void WaterFrameBuffer::initializeReflection()
{
	reflectionFBO = createFrameBuffer();
	
}

GLuint WaterFrameBuffer::createFrameBuffer()
{
	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	return frameBuffer;
}

GLuint WaterFrameBuffer::createTextureAttachment(int width, int height)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
		0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char*)NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
	return texture;
}

GLuint WaterFrameBuffer::createDepthTextureAttachment(int width, int height)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, (unsigned char*)NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
	return texture;
}