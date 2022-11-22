#pragma once

#include <imgui/imgui.h>
#include "Scene.h"
#include <GLFW/glfw3.h>

class Menus {
	public:
		Menus(ImGuiIO& io, Scene& scene);
		void DrawImguiMenus(glm::vec4& clear_color);
		void DrawMainMenu(glm::vec4& clear_color);
		void DrawModelMenu();
		void TranformationMouse();
		void TranformationKeyboard();

		void CheckLimits(MeshModel& activeModel);
	private:
		ImGuiIO& my_io;
		Scene& my_scene;

		bool show_demo_window;
		bool show_model_window;

		static int modelsCount;

		bool activateKeyboard;
};