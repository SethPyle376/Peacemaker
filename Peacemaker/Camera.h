#pragma once

#include <glm.hpp>
#include <GLFW\glfw3.h>
#include <iostream>

class Camera
{
public:
	Camera();

	glm::vec3 getPosition();

	void setPosition(glm::vec3 position);

	void offsetPosition(glm::vec3 offset);

	float getFieldOfView();
	void setFieldOfView(float fov);

	float getNearPlane();
	float getFarPlane();

	void setNearAndFarPlanes(float nearPlane, float farPlane);

	glm::mat4 getOrientation();

	void offsetOrientation(float upAngle, float rightAngle);

	void lookAt(glm::vec3 position);

	float getAspectRatio();
	void setAspectRatio(float aspectRatio);

	float update(GLFWwindow *window);

	glm::vec3 getForward();
	glm::vec3 getRight();
	glm::vec3 getUp();

	glm::mat4 getTransformationMatrix();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();

private:
	glm::vec3 position;
	float horizontalAngle;
	float verticalAngle;
	float fov;
	float nearPlane;
	float farPlane;
	float aspectRatio;

	float speed;

	void normalizeAngles();
	double lastTime;
};