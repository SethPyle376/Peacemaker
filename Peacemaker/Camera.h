#pragma once

#include <glm.hpp>
#include "GLFW\glfw3.h"

class Camera
{
	glm::vec3 position;
	float pitch;
	float yaw;
	float roll;
	float speed;
	float mouseSpeed;
	float initialFoV;


public:

	Camera(glm::vec3 start, float startingSpeed)
	{
		position = start;
		speed = startingSpeed;
		mouseSpeed = 0.005;
		initialFoV = 45.0f;
	}
	
	glm::vec3 getPosition()
	{
		return position;
	}

	float getPitch()
	{
		return pitch;
	}

	float getYaw()
	{
		return yaw;
	}

	float getRoll()
	{
		return roll;
	}

	glm::mat4 computeMVP();

	void update(GLFWwindow *window)
	{
		// glfwGetTime is called only once, the first time this function is called
		static double lastTime = glfwGetTime();

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);

		// Get mouse position
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		// Reset mouse position for next frame
		glfwSetCursorPos(window, 1024 / 2, 768 / 2);

		// Compute new orientation
		yaw += mouseSpeed * float(1024 / 2 - xpos);
		pitch += mouseSpeed * float(768 / 2 - ypos);

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		glm::vec3 direction(
			cos(pitch) * sin(yaw),
			sin(pitch),
			cos(pitch) * cos(yaw)
		);

		// Right vector
		glm::vec3 right = glm::vec3(
			sin(yaw - 3.14f / 2.0f),
			0,
			cos(yaw - 3.14f / 2.0f)
		);

		// Up vector
		glm::vec3 up = glm::cross(right, direction);

		// Move forward
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			position += direction * deltaTime * speed;
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			position -= direction * deltaTime * speed;
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			position += right * deltaTime * speed;
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			position -= right * deltaTime * speed;
		}

		float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

							   // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		//ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
		// Camera matrix
		/*ViewMatrix = glm::lookAt(
			position,           // Camera is here
			position + direction, // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
		);*/
	}
};