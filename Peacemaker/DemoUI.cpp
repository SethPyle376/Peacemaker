#include "DemoUI.h"

DemoUI::DemoUI(Scene *scene, GLFWwindow *window)
{
	this->scene = scene;
	this->window = window;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();
}

void DemoUI::render()
{
	ImGui_ImplGlfwGL3_NewFrame();

	ImGui::Text("Control Panel");

	ImGui::SliderFloat("Seed", &(scene->terrain->seed), 0.0f, 100.0f);

	if (ImGui::Button("Button"))
	{
		std::cout << "Button Clicked" << std::endl;
		scene->terrain->reload();
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}