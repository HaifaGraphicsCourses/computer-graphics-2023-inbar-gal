#include "Menus.h"
#include <nfd.h>
#include "Utils.h"

Menus::Menus(ImGuiIO& io, Scene& scene) : my_io(io), my_scene(scene) {
	this->show_demo_window = false;
	this->show_model_window = false;
}

void Menus::DrawImguiMenus(glm::vec4& clear_color) {
	// Menu Bar
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open", "CTRL+O")) {
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY) {
					// adding model to model vector in scene
					my_scene.AddModel(Utils::LoadMeshModel(outPath));
					// setting index of active model to the last model in the vector - that's the one we draw now
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

	DrawMainMenu(clear_color);
}

void Menus::DrawMainMenu(glm::vec4& clear_color) {
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
	ImGui::SetWindowSize(ImVec2(360, 300));

	if (ImGui::CollapsingHeader("Mouse Directed Transformations")) {
		TranformationMouse();
	}
	if (ImGui::CollapsingHeader("Keyboard Directed Transformations")) {
		TranformationKeyboard();
	}

	ImGui::End();
}

void Menus::TranformationMouse() {
	ImGui::PushItemWidth(100);

	ImGui::Text("Model Scaling");
	ImGui::InputInt("scaling", &my_scene.GetActiveModel().modelScaling);
	ImGui::Text("Model Translation (1280 width, 720 height)");
	ImGui::InputInt("x", &my_scene.GetActiveModel().modelTranslationX);
	ImGui::SameLine();
	ImGui::InputInt("y", &my_scene.GetActiveModel().modelTranslationY);
	ImGui::SameLine();
	ImGui::InputInt("z", &my_scene.GetActiveModel().modelTranslationZ);
	ImGui::Text("Model Rotation (0 <= degree <= 360)");
	ImGui::InputInt("x", &my_scene.GetActiveModel().modelRotationX);
	ImGui::SameLine();
	ImGui::InputInt("y", &my_scene.GetActiveModel().modelRotationY);
	ImGui::SameLine();
	ImGui::InputInt("z", &my_scene.GetActiveModel().modelRotationZ);

	ImGui::Text("World Scaling");
	ImGui::InputInt("scaling", &my_scene.GetActiveModel().worldScaling);
	ImGui::Text("World Translation (1280 width, 720 height)");
	ImGui::InputInt("x", &my_scene.GetActiveModel().worldTranslationX);
	ImGui::SameLine();
	ImGui::InputInt("y", &my_scene.GetActiveModel().worldTranslationY);
	ImGui::SameLine();
	ImGui::InputInt("z", &my_scene.GetActiveModel().worldTranslationZ);
	ImGui::Text("World Rotation (0 <= degree <= 360)");
	ImGui::InputInt("x", &my_scene.GetActiveModel().worldRotationX);
	ImGui::SameLine();
	ImGui::InputInt("y", &my_scene.GetActiveModel().worldRotationY);
	ImGui::SameLine();
	ImGui::InputInt("z", &my_scene.GetActiveModel().worldRotationZ);
}

void Menus::TranformationKeyboard() {
	ImGui::Text("Choose frame");
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

	ImGui::Text("Keyboard instructions:");
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