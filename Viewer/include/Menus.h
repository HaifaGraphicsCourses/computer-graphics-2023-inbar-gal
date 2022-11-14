#pragma once

#include <imgui/imgui.h>
#include "Scene.h"
#include <GLFW/glfw3.h>

class Menus {
public:
	Menus(ImGuiIO& io, Scene& scene, glm::vec4 clear_color);
	void DrawImguiMenus();
	void DrawMainMenu();
	void DrawModelMenu();
	void TranformationMouse();
	void TranformationKeyboard();
private:
	ImGuiIO& my_io;
	Scene& my_scene;
	glm::vec4 clear_color;
	bool show_demo_window;
	bool show_model_window;
};