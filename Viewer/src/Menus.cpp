#include "Menus.h"
#include <nfd.h>
#include "Utils.h"
#include <xstring>

int Menus::modelsCount = 0;

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

	ImGui::ColorEdit3("clear color", (float*)&clear_color);
	ImGui::Checkbox("Demo Window", &show_demo_window);
	if (show_demo_window) {
		ImGui::ShowDemoWindow(&show_demo_window);
	}
	ImGui::Checkbox("Model Menu", &show_model_window);
	if (show_model_window) {
		DrawModelMenu();
	}

	ImGui::End();
}

void Menus::DrawModelMenu() {
	ImGui::Begin("Model Menu");

	TranformationMouse();
	TranformationKeyboard();

	ImGui::End();
}

void Menus::TranformationMouse() {
	ImGui::Text("Mouse Transformations");
	modelsCount = my_scene.GetModelCount();

	for (int i = 0; i < modelsCount; i++) {
		ImGui::PushID(i);
		if (ImGui::CollapsingHeader(my_scene.GetModel(i).GetModelName().c_str())) {
			if (my_scene.GetActiveModelIndex() != i) {
				if (ImGui::Button("Activate")) {
					my_scene.SetActiveModelIndex(i);
				}
			}
			else {
				auto& activeModel = my_scene.GetActiveModel();
				static int e = 0;
				ImGui::RadioButton("model frame", &e, 0);
				ImGui::SameLine();
				ImGui::RadioButton("world frame", &e, 1);
				if (e == 0) {
					if (ImGui::SliderFloat("model scaling", &activeModel.modelScaling, 1, 1000)) {
						activeModel.isScaling = true;
					}
					if (ImGui::SliderFloat("model translating x", &activeModel.modelTranslationX, 0, my_io.DisplaySize.x)) {
						activeModel.isTranslating = true;
					}
					if (ImGui::SliderFloat("model translating y", &activeModel.modelTranslationY, 0, my_io.DisplaySize.y)) {
						activeModel.isTranslating = true;
					}
					if (ImGui::SliderFloat("model translating z", &activeModel.modelTranslationZ, 0, 1000)) {
						activeModel.isTranslating = true;
					}
					if (ImGui::SliderFloat("model rotating x", &activeModel.modelRotationX, -360, 360)) {
						activeModel.isRotating = true;
					}
					if (ImGui::SliderFloat("model rotating y", &activeModel.modelRotationY, -360, 360)) {
						activeModel.isRotating = true;
					}
					if (ImGui::SliderFloat("model rotating z", &activeModel.modelRotationZ, -360, 360)) {
						activeModel.isRotating = true;
					}
				}
				if (e == 1) {
					if (ImGui::SliderFloat("world scaling", &activeModel.worldScaling, 1, 1000)) {
						activeModel.isScaling = true;
					}
					if (ImGui::SliderFloat("world translating x", &activeModel.worldTranslationX, 0, my_io.DisplaySize.x)) {
						activeModel.isTranslating = true;
					}
					if (ImGui::SliderFloat("world translating y", &activeModel.worldTranslationY, 0, my_io.DisplaySize.y)) {
						activeModel.isTranslating = true;
					}
					if (ImGui::SliderFloat("world translating z", &activeModel.worldTranslationZ, 0, 1000)) {
						activeModel.isTranslating = true;
					}
					if (ImGui::SliderFloat("world rotating x", &activeModel.worldRotationX, -360, 360)) {
						activeModel.isRotating = true;
					}
					if (ImGui::SliderFloat("world rotating y", &activeModel.worldRotationY, -360, 360)) {
						activeModel.isRotating = true;
					}
					if (ImGui::SliderFloat("world rotating z", &activeModel.worldRotationZ, -360, 360)) {
						activeModel.isRotating = true;
					}
				}
				if (activeModel.isScaling || activeModel.isTranslating || activeModel.isRotating) {
					activeModel.ChangeModel();
				}
			}
		}
		ImGui::PopID();
	}
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