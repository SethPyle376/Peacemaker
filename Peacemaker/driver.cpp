#pragma once
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <map>

#include "Camera.h"
#include "ShaderProgram.h"
#include "Skybox.h"
#include "Scene.h"
#include "Renderer.h"
#include "Light.h"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include <ft2build.h>
#include <freetype/freetype.h>


using namespace std;

int main()
{
	Renderer *renderer = new Renderer(1920, 1080);

	Scene *scene = new Scene(glm::vec2(renderer->width, renderer->height));

	Light *light = new Light(glm::vec3(2.0f, 3.0f, 4.0f));

	Actor *sponza = new Actor("res/models/islandsmall.obj", "res/shaders/vertex.glsl", "res/shaders/fragment.glsl", scene);

	scene->lights.push_back(light);
	scene->actors.push_back(sponza);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(renderer->getWindow(), true);
	ImGui::StyleColorsDark();

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45, 0.55f, 0.60f, 1.00f);


	while (!glfwWindowShouldClose(renderer->getWindow()))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ImGui_ImplGlfwGL3_NewFrame();
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		int display_w, display_h;
		glfwGetFramebufferSize(renderer->getWindow(), &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		renderer->update(scene);
		
		//glfwSwapBuffers(renderer->getWindow());
	}

	//Kill
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}



