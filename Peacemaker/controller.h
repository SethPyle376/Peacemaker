#pragma once

#include <glm.hpp>
#include <GLFW\glfw3.h>

float computeMatricesFromInputs(GLFWwindow* window, bool inverted);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
glm::mat4 computeMVP();
glm::vec3 getPosition();
void setPosition(float updatedY);
void invertPitch();
glm::vec2 getRotation();
void setRotation(glm::vec2 newRotation);