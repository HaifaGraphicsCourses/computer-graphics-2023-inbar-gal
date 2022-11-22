#include "Menus.h"
#include <nfd.h>
#include "Utils.h"
#include <xstring>

int Menus::modelsCount = 0;

Menus::Menus(ImGuiIO& io, Scene& scene) : my_io(io), my_scene(scene) {
	this->show_demo_window = false;
	this->show_model_window = false;
	this->activateKeyboard = false;
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
	ImGui::SameLine();
	ImGui::Checkbox("Model Menu", &show_model_window);
	if (show_model_window) {
		DrawModelMenu();
	}

	ImGui::End();
}

void Menus::DrawModelMenu() {
	ImGui::Begin("Model Menu");

	auto& activeModel = my_scene.GetActiveModel();
	if (ImGui::Button("Reset All Transformations")) {
		activeModel.Reset();
	}
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
				ImGui::PushItemWidth(200);
				if (e == 0) {
					if (ImGui::SliderFloat("model scaling", &activeModel.modelScaling, 1, 1000)) {
						activeModel.isChanged = true;
					}
					if (ImGui::SliderFloat("model translating x", &activeModel.modelTranslationX, 0, my_io.DisplaySize.x)) {
						activeModel.isChanged = true;
					}
					if (ImGui::SliderFloat("model translating y", &activeModel.modelTranslationY, 0, my_io.DisplaySize.y)) {
						activeModel.isChanged = true;
					}
					if (ImGui::SliderFloat("model translating z", &activeModel.modelTranslationZ, 0, 1000)) {
						activeModel.isChanged = true;
					}
					if (ImGui::SliderFloat("model rotating x", &activeModel.modelRotationX, -360, 360)) {
						activeModel.isChanged = true;
					}
					if (ImGui::SliderFloat("model rotating y", &activeModel.modelRotationY, -360, 360)) {
						activeModel.isChanged = true;
					}
					if (ImGui::SliderFloat("model rotating z", &activeModel.modelRotationZ, -360, 360)) {
						activeModel.isChanged = true;
					}
				}
				if (e == 1) {
					if (ImGui::SliderFloat("world scaling", &activeModel.worldScaling, 1, 1000)) {
						activeModel.isChanged = true;
					}
					if (ImGui::SliderFloat("world translating x", &activeModel.worldTranslationX, 0, my_io.DisplaySize.x)) {
						activeModel.isChanged = true;
					}
					if (ImGui::SliderFloat("world translating y", &activeModel.worldTranslationY, 0, my_io.DisplaySize.y)) {
						activeModel.isChanged = true;
					}
					if (ImGui::SliderFloat("world translating z", &activeModel.worldTranslationZ, 0, 1000)) {
						activeModel.isChanged = true;
					}
					if (ImGui::SliderFloat("world rotating x", &activeModel.worldRotationX, -360, 360)) {
						activeModel.isChanged = true;
					}
					if (ImGui::SliderFloat("world rotating y", &activeModel.worldRotationY, -360, 360)) {
						activeModel.isChanged = true;
					}
					if (ImGui::SliderFloat("world rotating z", &activeModel.worldRotationZ, -360, 360)) {
						activeModel.isChanged = true;
					}
				}
				if (activeModel.isChanged) {
					activeModel.ChangeModel();
				}
				ImGui::PopItemWidth();
			}
		}
		ImGui::PopID();
	}
}

void Menus::TranformationKeyboard() {
	ImGui::Text("Keyboard Transformations");
	ImGui::Checkbox("Activate Keyboard Transformations", &activateKeyboard);

	if (activateKeyboard) {
		my_io.WantCaptureKeyboard = true;
		auto& activeModel = my_scene.GetActiveModel();

		ImGui::Text("Choose frame");
		static int e = 0;
		ImGui::RadioButton("model frame", &e, 0);
		ImGui::SameLine();
		ImGui::RadioButton("world frame", &e, 1);

		if (e == 0) {
			if (my_io.KeysDown[66]) {
				activeModel.modelScaling++;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[83]) {
				activeModel.modelScaling--;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[49]) {
				activeModel.modelTranslationX++;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[50]) {
				activeModel.modelTranslationX--;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[51]) {
				activeModel.modelTranslationY++;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[52]) {
				activeModel.modelTranslationY--;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[53]) {
				activeModel.modelTranslationZ++;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[54]) {
				activeModel.modelTranslationZ--;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[90]) {
				activeModel.modelRotationX++;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[89]) {
				activeModel.modelRotationX--;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[88]) {
				activeModel.modelRotationY++;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[87]) {
				activeModel.modelRotationY--;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[86]) {
				activeModel.modelRotationZ++;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[85]) {
				activeModel.modelRotationZ--;
				activeModel.isChanged = true;
			}
		}
		if (e == 1) {
			if (my_io.KeysDown[66]) {
				activeModel.worldScaling++;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[83]) {
				activeModel.worldScaling--;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[49]) {
				activeModel.worldTranslationX++;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[50]) {
				activeModel.worldTranslationX--;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[51]) {
				activeModel.worldTranslationY++;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[52]) {
				activeModel.worldTranslationY--;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[53]) {
				activeModel.worldTranslationZ++;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[54]) {
				activeModel.worldTranslationZ--;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[90]) {
				activeModel.worldRotationX++;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[89]) {
				activeModel.worldRotationX--;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[88]) {
				activeModel.worldRotationY++;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[87]) {
				activeModel.worldRotationY--;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[86]) {
				activeModel.worldRotationZ++;
				activeModel.isChanged = true;
			}
			if (my_io.KeysDown[85]) {
				activeModel.worldRotationZ--;
				activeModel.isChanged = true;
			}
		}

		if (activeModel.isChanged) {
			CheckLimits(activeModel);
			activeModel.ChangeModel();
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
	else {
		my_io.WantCaptureKeyboard = false;
	}
}

void Menus::CheckLimits(MeshModel& activeModel) {
	if (activeModel.modelScaling > 1000) {
		activeModel.modelScaling = 1000;
	}
	if (activeModel.modelScaling < 1) {
		activeModel.modelScaling = 1;
	}
	if (activeModel.worldScaling > 1000) {
		activeModel.worldScaling = 1000;
	}
	if (activeModel.worldScaling < 1) {
		activeModel.worldScaling = 1;
	}

	if (activeModel.modelTranslationX > my_io.DisplaySize.x) {
		activeModel.modelTranslationX = my_io.DisplaySize.x;
	}
	if (activeModel.modelTranslationX < 0) {
		activeModel.modelTranslationX = 0;
	}
	if (activeModel.worldTranslationX > my_io.DisplaySize.x) {
		activeModel.worldTranslationX = my_io.DisplaySize.x;
	}
	if (activeModel.worldTranslationX < 0) {
		activeModel.worldTranslationX = 0;
	}
	if (activeModel.modelTranslationY > my_io.DisplaySize.y) {
		activeModel.modelTranslationY = my_io.DisplaySize.y;
	}
	if (activeModel.modelTranslationY < 0) {
		activeModel.modelTranslationY = 0;
	}
	if (activeModel.worldTranslationY > my_io.DisplaySize.y) {
		activeModel.worldTranslationY = my_io.DisplaySize.y;
	}
	if (activeModel.worldTranslationY < 0) {
		activeModel.worldTranslationY = 0;
	}
	if (activeModel.modelTranslationZ > 1000) {
		activeModel.modelTranslationZ = 1000;
	}
	if (activeModel.modelTranslationZ < 0) {
		activeModel.modelTranslationZ = 0;
	}
	if (activeModel.worldTranslationZ > 1000) {
		activeModel.worldTranslationZ = 1000;
	}
	if (activeModel.worldTranslationZ < 0) {
		activeModel.worldTranslationZ = 0;
	}

	if (activeModel.modelRotationX > 360) {
		activeModel.modelRotationX = 360;
	}
	if (activeModel.modelRotationX < -360) {
		activeModel.modelRotationX = -360;
	}
	if (activeModel.worldRotationX > 360) {
		activeModel.worldRotationX = 360;
	}
	if (activeModel.worldRotationX < -360) {
		activeModel.worldRotationX = -360;
	}
	if (activeModel.modelRotationY > 360) {
		activeModel.modelRotationY = 360;
	}
	if (activeModel.modelRotationY < -360) {
		activeModel.modelRotationY = -360;
	}
	if (activeModel.worldRotationY > 360) {
		activeModel.worldRotationY = 360;
	}
	if (activeModel.worldRotationY < -360) {
		activeModel.worldRotationY = -360;
	}
	if (activeModel.modelRotationZ > 360) {
		activeModel.modelRotationZ = 360;
	}
	if (activeModel.modelRotationZ < -360) {
		activeModel.modelRotationZ = -360;
	}
	if (activeModel.worldRotationZ > 360) {
		activeModel.worldRotationZ = 360;
	}
	if (activeModel.worldRotationZ < -360) {
		activeModel.worldRotationZ = -360;
	}
}