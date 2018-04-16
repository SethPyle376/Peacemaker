#include "ShadowFrameBuffer.h"

ShadowFrameBuffer::ShadowFrameBuffer(int width, int height)
{
	this->width = width;
	this->height = height;

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width * 4, height * 4, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR: SHADOW FRAMEBUFFER NOT INITIALIZED" << std::endl;

	unbind();
}

void ShadowFrameBuffer::bind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glViewport(0, 0, width * 4, height * 4);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_ALPHA_TEST);
	glCullFace(GL_FRONT);
}

void ShadowFrameBuffer::unbind()
{
	glCullFace(GL_BACK);
	glEnable(GL_ALPHA_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1920, 1080);
}

GLuint ShadowFrameBuffer::getShadowMap()
{
	return shadowMap;
}

ShadowFrameBuffer::~ShadowFrameBuffer()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &shadowMap);
}