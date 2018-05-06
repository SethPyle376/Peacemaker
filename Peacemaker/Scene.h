#pragma once
#include <vector>

#include "Actor.h"
#include "Camera.h"
#include "Light.h"
#include "Text.h"
#include "Skybox.h"
#include "Terrain.h"
#include "Frame.h"

#include "DemoUI.h"

//Forward definition of cross referenced class
class Actor;
class Skybox;
class Camera;
class Terrain;
class DemoUI;

class Scene
{
private:
	float totalTime;
	float deltaTime;

	void debugInput();

	DemoUI *controlPanel;

	

	Rectangle *rectangle;

	Frame *frame;

	glm::vec2 resolution;


public:
	Scene(glm::vec2 resolution, GLFWwindow *window);
	void update(GLFWwindow *window, GLuint shadowMap);

	void drawShadows();

	Terrain *terrain;

	std::vector<Actor*> actors;
	std::vector<Light*> lights;

	Skybox *skybox;

	Camera *camera;
};