#include "Menus.h"
#include <nfd.h>
#include "Utils.h"

Menus::Menus(ImGuiIO& io, Scene& scene, glm::vec4 clear_color) : my_io(io), my_scene(scene) {
	this->clear_color = clear_color;
	this->show_demo_window = false;
	this->show_model_window = false;
}

void Menus::DrawImguiMenus() {
	// Menu Bar
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open", "CTRL+O")) {
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY) {
					my_scene.AddModel(Utils::LoadMeshModel(outPath));
					my_scene.SetActiveModelIndex(my_scene.GetModelCount() - 1);
					free(outPath);
				}
				else if (result == NFD_CANCEL) {

				}
				else {

				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	DrawMainMenu();
}

void Menus::DrawMainMenu() {
	ImGui::Begin("Main Menu");
	ImGui::SetWindowPos(ImVec2(500, 0));
	ImGui::SetWindowSize(ImVec2(280, 300));

	ImGui::Checkbox("Demo Window", &show_demo_window);
	if (show_demo_window) {
		ImGui::ShowDemoWindow(&show_demo_window);
	}

	ImGui::ColorEdit3("clear color", (float*)&clear_color);

	ImGui::Checkbox("Model Menu", &show_model_window);
	if (show_model_window) {
		DrawModelMenu();
	}

	ImGui::End();
}

void Menus::DrawModelMenu() {
	ImGui::Begin("Model Menu");
	ImGui::SetWindowSize(ImVec2(300, 300));

	if (ImGui::CollapsingHeader("Tranformation With Mouse Movements")) {
		TranformationMouse();
	}
	if (ImGui::CollapsingHeader("Tranformation With Keyboard Movements")) {
		TranformationKeyboard();
	}

	ImGui::End();
}

void Menus::TranformationMouse() {
	ImGui::TextColored(ImVec4(255.0, 0.0, 0.0, 1.0), "To allow mouse directed transformation, change slider");


}

void Menus::TranformationKeyboard() {
	ImGui::TextColored(ImVec4(255.0, 0.0, 0.0, 1.0), "Chose frame");
	static int e = 0;
	ImGui::RadioButton("model frame", &e, 0);
	ImGui::SameLine();
	ImGui::RadioButton("world frame", &e, 1);

	if (e == 0) {
		ImGui::Text("0");
	}
	if (e == 1) {
		ImGui::Text("1");
	}

	ImGui::TextColored(ImVec4(255.0, 0.0, 0.0, 1.0), "Keyboard instructions:");
	ImGui::Text("For scaling:");
	ImGui::BulletText("press B to increas size");
	ImGui::BulletText("press S to reduce size");

	ImGui::Text("For translation:");
	ImGui::BulletText("X axis: press 1 for X+, 2 for X-");
	ImGui::BulletText("Y axis: press 3 for Y+, 4 for Y-");
	ImGui::BulletText("Z axis: press 5 for Z+, 6 for Z-");

	ImGui::Text("For rotation:");
	ImGui::BulletText("X axis: press Z for X+, Y for X-");
	ImGui::BulletText("Y axis: press X for Y+, W for Y-");
	ImGui::BulletText("Z axis: press V for Z+, U for Z-");
}
