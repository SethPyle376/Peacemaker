
//extern GLFWwindow* window;

#include "controller.h"

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
using namespace glm;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
glm::vec3 position = glm::vec3(0, 15, 5);

// Initial horizontal angle : toward -Z
float horizontalAngle = glm::pi<float>();
// Initial vertical angle : none
float verticalAngle = -(glm::pi<float>() / 4.0f);

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}
glm::vec3 getPosition()
{
	return position;
}

void setPosition(float updatedY)
{
	position.y = updatedY;
}

glm::vec2 getRotation()
{
	return glm::vec2(horizontalAngle, verticalAngle);
}

void setRotation(glm::vec2 newRotation)
{
	horizontalAngle = newRotation.x;
	verticalAngle = newRotation.y;
}


// Initial position : on +Z


// Initial Field of View
float initialFoV = 45.0f;

float speed = 6.0f; // 3 units / second
float mouseSpeed = 0.005f;


void invertPitch()
{
	verticalAngle += glm::pi<float>();
	horizontalAngle += glm::pi<float>();
}

glm::mat4 computeMVP()
{
	return ProjectionMatrix * ViewMatrix * glm::mat4(1.0);
}



float computeMatricesFromInputs(GLFWwindow* window, bool inverted) {

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
	horizontalAngle += mouseSpeed * float(1024 / 2 - xpos);
	verticalAngle += mouseSpeed * float(768 / 2 - ypos);

	if (inverted)
	{
		verticalAngle += (glm::pi<float>() / 2);
	}
	

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	if (inverted)
	{
		up = glm::vec3(0, -1, 0);
		verticalAngle -= (glm::pi<float>() / 2);
	}

	if (!inverted)
	{
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
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

						   // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
	return deltaTime;
}