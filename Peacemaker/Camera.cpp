#include "Camera.h"

#include <cmath>
#include <gtc/matrix_transform.hpp>

static const float MAX_VERT_ANGLE = 85.0f;

Camera::Camera()
{
	currentFrames = 0;
	position = glm::vec3(0.0f, 0.0f, 1.0f);
	horizontalAngle = 0;
	verticalAngle = 0;
	fov = 50.0f;
	nearPlane = 0.01f;
	farPlane = 10000.0f;
	aspectRatio = (4.0f / 3.0f);
	speed = 20.0f;
	lastTime = 0;
}

glm::vec3 Camera::getPosition()
{
	return position;
}

void Camera::setPosition(glm::vec3 position)
{
	this->position = position;
}

void Camera::offsetPosition(glm::vec3 offset)
{
	position += offset;
}

float Camera::getFieldOfView()
{
	return fov;
}

void Camera::setFieldOfView(float fov)
{
	this->fov = fov;
}

float Camera::getNearPlane()
{
	return nearPlane;
}

float Camera::getFarPlane()
{
	return farPlane;
}

void Camera::setNearAndFarPlanes(float nearPlane, float farPlane)
{
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
}

glm::mat4 Camera::getOrientation()
{
	glm::mat4 orientation;
	orientation = glm::rotate(orientation, glm::radians(verticalAngle), glm::vec3(1, 0, 0));
	orientation = glm::rotate(orientation, glm::radians(horizontalAngle), glm::vec3(0, 1, 0));
	return orientation;
}

void Camera::offsetOrientation(float upAngle, float rightAngle)
{
	horizontalAngle += rightAngle;
	verticalAngle += upAngle;
	normalizeAngles();
}

void Camera::lookAt(glm::vec3 position)
{
	glm::vec3 direction = glm::normalize(position - this->position);
	verticalAngle = glm::radians(asinf(-direction.y));
	horizontalAngle = -glm::radians(atan2f(-direction.x, -direction.z));
	normalizeAngles();
}

float Camera::getAspectRatio()
{
	return aspectRatio;
}

void Camera::setAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
}

glm::vec3 Camera::getForward()
{
	glm::vec4 forward = glm::inverse(getOrientation()) * glm::vec4(0, 0, -1, 1);
	return glm::vec3(forward);
}

glm::vec3 Camera::getRight()
{
	glm::vec4 right = glm::inverse(getOrientation()) * glm::vec4(1, 0, 0, 1);
	return glm::vec3(right);
}

glm::vec3 Camera::getUp()
{
	glm::vec4 up = glm::inverse(getOrientation()) * glm::vec4(0, 1, 0, 1);
	return glm::vec3(up);
}

glm::mat4 Camera::getTransformationMatrix()
{
	return getProjectionMatrix() * getViewMatrix();
}

glm::mat4 Camera::getProjectionMatrix()
{
	return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

glm::mat4 Camera::getViewMatrix()
{
	return getOrientation() * glm::translate(glm::mat4(), -position);
}

void Camera::normalizeAngles()
{
	horizontalAngle = fmodf(horizontalAngle, 360.0f);
	if (horizontalAngle < 0.0f)
		horizontalAngle += 360.0f;

	if (verticalAngle > MAX_VERT_ANGLE)
		verticalAngle = MAX_VERT_ANGLE;
	else if (verticalAngle < -MAX_VERT_ANGLE)
		verticalAngle = -MAX_VERT_ANGLE;
}

float Camera::update(GLFWwindow *window, Scene *scene)
{
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	lastTime = glfwGetTime();

	currentFrames = 1 / deltaTime;

	// Compute time difference between current and last frame

	if (glfwGetKey(window, GLFW_KEY_UP))
	{
		offsetPosition(deltaTime * speed * getForward());
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN))
	{
		offsetPosition(deltaTime * speed * -getForward());
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT))
	{
		offsetPosition(deltaTime * speed * getRight());
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT))
	{
		offsetPosition(deltaTime * speed * -getRight());
	}
	if (glfwGetKey(window, GLFW_KEY_ENTER))
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (glfwGetKey(window, GLFW_KEY_W))
	{
		scene->lights[0]->offSetPosition(glm::vec3(0, speed * deltaTime, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		scene->lights[0]->offSetPosition(glm::vec3(0, -speed * deltaTime, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		scene->lights[0]->offSetPosition(glm::vec3(speed * deltaTime, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		scene->lights[0]->offSetPosition(glm::vec3(-speed * deltaTime, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_Q))
	{
		scene->lights[0]->offSetPosition(glm::vec3(0, 0, speed * deltaTime));
	}
	if (glfwGetKey(window, GLFW_KEY_E))
	{
		scene->lights[0]->offSetPosition(glm::vec3(0, 0, -speed * deltaTime));
	}



	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	offsetOrientation(0.1f * (float)mouseY, 0.1f * (float)mouseX);
	glfwSetCursorPos(window, 0, 0);
	return deltaTime;
}