#include "WaterFrameBuffer.h"

WaterFrameBuffer::WaterFrameBuffer()
{
	initializeReflection();
	initializeRefraction();
}

void WaterFrameBuffer::initializeReflection()
{
	reflectionFBO = createFrameBuffer();
	reflectionTexture = createTextureAttachment(reflectionWidth, reflectionHeight);
	reflectionDepthBuffer = createDepthTextureAttachment(reflectionWidth, reflectionHeight);
	unbindFBO();
}

void WaterFrameBuffer::initializeRefraction()
{
	refractionFBO = createFrameBuffer();
	refractionTexture = createTextureAttachment(refractionWidth, refractionHeight);
	refractionDepthTexture = createDepthTextureAttachment(refractionWidth, refractionHeight);
	unbindFBO();
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

GLuint WaterFrameBuffer::createDepthBufferAttachment(int width, int height)
{
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	return depthBuffer;
}

void WaterFrameBuffer::bindFrameBuffer(GLuint frameBuffer, int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, width, height);
}

void WaterFrameBuffer::bindReflection()
{
	bindFrameBuffer(reflectionFBO, reflectionWidth, reflectionHeight);
}

void WaterFrameBuffer::bindRefraction()
{
	bindFrameBuffer(refractionFBO, refractionWidth, refractionHeight);
}

void WaterFrameBuffer::unbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1920, 1080);
}

GLuint WaterFrameBuffer::getReflectionTexture()
{
	return reflectionTexture;
}

GLuint WaterFrameBuffer::getRefractionTexture()
{
	return refractionTexture;
}

GLuint WaterFrameBuffer::getRefractionDepthTexture()
{
	return refractionDepthTexture;
}