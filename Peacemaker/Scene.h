#pragma once
#include <vector>

#include "Actor.h"
#include "Camera.h"
#include "Light.h"
#include "Text.h"
#include "Skybox.h"

//Forward definition of cross referenced class
class Actor;
class Skybox;
class Camera;

class Scene
{
private:
	float totalTime;
	float deltaTime;

	void debugInput();


public:
	Scene();
	void update(GLFWwindow *window);

	std::vector<Actor*> actors;
	std::vector<Light*> lights;

	Skybox *skybox;

	Camera *camera;
};