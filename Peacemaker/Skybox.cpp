#include "Skybox.h"

Skybox::Skybox(int scale, Scene *scene)
{
	for (int i = 0; i < 108; i++)
	{
		skyboxVertices[i] *= scale;
	}

	glGenVertexArrays(1, &skyboxVAO);
	
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glBindVertexArray(0);

	std::vector<const GLchar*> faces;
	faces.push_back("res/textures/skybox/bluecloud_rt.jpg");
	faces.push_back("res/textures/skybox/bluecloud_lf.jpg");
	faces.push_back("res/textures/skybox/bluecloud_up.jpg");
	faces.push_back("res/textures/skybox/bluecloud_dn.jpg");
	faces.push_back("res/textures/skybox/bluecloud_bk.jpg");
	faces.push_back("res/textures/skybox/bluecloud_ft.jpg");

	cubemapTexture = loadCubemap(faces);
	this->scene = scene;

	shader = new ShaderProgram("res/shaders/skyboxVertex.glsl", "res/shaders/skyboxFragment.glsl");
}

void Skybox::draw()
{
	glDepthFunc(GL_LEQUAL);
	shader->start();

	shader->loadMatrix(glGetUniformLocation(shader->getProgramID(), "view"), scene->camera->getViewMatrix());
	shader->loadMatrix(glGetUniformLocation(shader->getProgramID(), "projection"), scene->camera->getProjectionMatrix());

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
	shader->stop();

}

GLuint Skybox::loadTexture(GLchar *path)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int imageWidth, imageHeight;

	unsigned char *image = SOIL_load_image(path, &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	SOIL_free_image_data(image);

	return textureID;
}



GLuint Skybox::loadCubemap(std::vector<const GLchar * > faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int imageWidth, imageHeight;
	unsigned char *image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

