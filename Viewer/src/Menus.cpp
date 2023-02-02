#include "Menus.h"
#include <nfd.h>
#include "Utils.h"
#include <xstring>

int Menus::modelsCount = 0;
int Menus::cameraCount = 0;
int Menus::lightCount = 0;

Menus::Menus(ImGuiIO& io, Scene& scene) : my_io(io), my_scene(scene) {
	this->show_demo_window = false;
	this->show_model_window = false;
	this->activateKeyboard = false;
	this->show_camera_menu = false;
	this->orbitAroundOrigin = false;
	my_scene.showAxes = false;
	my_scene.showBoundingBox = false;
	my_scene.showFaceNormals = false;
	my_scene.showVertexNormals = false;

	this->show_triangle_fill_menu = false;
	my_scene.showBoundingRectangle = false;
	my_scene.fillMode = 0;
	my_scene.greyScaleLevel = 50;

	this->show_light_menu = false;
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
	
	ImGui::Checkbox("Demo Window   ", &show_demo_window);
	if (show_demo_window) {
		ImGui::ShowDemoWindow(&show_demo_window);
	}
	ImGui::SameLine();
	ImGui::Checkbox("Model Menu", &show_model_window);
	if (show_model_window) {
		DrawModelMenu();
	}

	ImGui::Checkbox("Camera Menu   ", &show_camera_menu);
	if (show_camera_menu) {
		DrawCameraMenu();
	}
	ImGui::SameLine();
	ImGui::Checkbox("Models and World axes", &my_scene.showAxes);

	ImGui::Checkbox("Bounding Box  ", &my_scene.showBoundingBox);
	ImGui::SameLine();
	ImGui::Checkbox("Face Normals", &my_scene.showFaceNormals);
	
	ImGui::Checkbox("Vertex Normals", &my_scene.showVertexNormals);
	ImGui::SameLine();
	ImGui::Checkbox("Triangle Fill Menu", &show_triangle_fill_menu);
	if (show_triangle_fill_menu) {
		DrawTriangleFillMenu();
	}

	ImGui::Checkbox("Lights Menu", &show_light_menu);
	if (show_light_menu) {
		DrawLightMenu();
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
					if (ImGui::SliderFloat("model scaling", &activeModel.modelScaling, -10, 10)) {
						activeModel.isChanged = true;
					}
					if (ImGui::SliderFloat("model translating x", &activeModel.modelTranslationX, (-my_io.DisplaySize.x / 2), (my_io.DisplaySize.x / 2))) {
						activeModel.isChanged = true;
					}
					if (ImGui::SliderFloat("model translating y", &activeModel.modelTranslationY, (-my_io.DisplaySize.y / 2), (my_io.DisplaySize.y / 2))) {
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
					if (ImGui::SliderFloat("world scaling", &activeModel.worldScaling, -10, 10)) {
						activeModel.isChanged = true;
					}
					if (ImGui::SliderFloat("world translating x", &activeModel.worldTranslationX, (-my_io.DisplaySize.x / 2), (my_io.DisplaySize.x / 2))) {
						activeModel.isChanged = true;
					}
					if (ImGui::SliderFloat("world translating y", &activeModel.worldTranslationY, (-my_io.DisplaySize.y / 2), (my_io.DisplaySize.y / 2))) {
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

	if (activeModel.modelTranslationX > (my_io.DisplaySize.x / 2)) {
		activeModel.modelTranslationX = (my_io.DisplaySize.x / 2);
	}
	if (activeModel.modelTranslationX < (-my_io.DisplaySize.x / 2)) {
		activeModel.modelTranslationX = (-my_io.DisplaySize.x / 2);
	}
	if (activeModel.worldTranslationX > (my_io.DisplaySize.x / 2)) {
		activeModel.worldTranslationX = (my_io.DisplaySize.x / 2);
	}
	if (activeModel.worldTranslationX < (-my_io.DisplaySize.x / 2)) {
		activeModel.worldTranslationX = (-my_io.DisplaySize.x / 2);
	}
	if (activeModel.modelTranslationY > (my_io.DisplaySize.y / 2)) {
		activeModel.modelTranslationY = (my_io.DisplaySize.y / 2);
	}
	if (activeModel.modelTranslationY < (-my_io.DisplaySize.y / 2)) {
		activeModel.modelTranslationY = (-my_io.DisplaySize.y / 2);
	}
	if (activeModel.worldTranslationY > (my_io.DisplaySize.y / 2)) {
		activeModel.worldTranslationY = (my_io.DisplaySize.y / 2);
	}
	if (activeModel.worldTranslationY < (-my_io.DisplaySize.y / 2)) {
		activeModel.worldTranslationY = (-my_io.DisplaySize.y / 2);
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

void Menus::DrawCameraMenu() {
	ImGui::Begin("Camera Menu");

	if (ImGui::Button("Add Another Camera")) {
		std::shared_ptr<Camera> camera2(new Camera());
		my_scene.AddCamera(camera2);
		my_scene.SetActiveCameraIndex(my_scene.GetCameraCount() - 1);
	}
	
	CameraPosition();
		
	ImGui::End();
}

void Menus::CameraPosition() {
	ImGui::Text("Change Camera Position");	
	cameraCount = my_scene.GetCameraCount();

	for (int i = 0; i < cameraCount; i++) {
		ImGui::PushID(i);
		string temp = "camera " + to_string(i + 1);
		const char* name = temp.c_str();
		
		if (ImGui::CollapsingHeader(name)) {
			if (my_scene.GetActiveCameraIndex() != i) {
				if (ImGui::Button("Activate")) {
					my_scene.SetActiveCameraIndex(i);
				}
			}
			else {
				auto& activeCamera = my_scene.GetActiveCamera();
				if (ImGui::Button("Reset Camera Position")) {
					activeCamera.Reset();
				}

				ImGui::Checkbox("Orbit Around Origin", &orbitAroundOrigin);
				if (orbitAroundOrigin) {
					activeCamera.Orbit();
				}
				ImGui::PushItemWidth(50);
				if (ImGui::SliderFloat("up x", &activeCamera.upX, -5, 5)) {
					activeCamera.isChanged = true;
				}
				ImGui::SameLine();
				if (ImGui::SliderFloat("up y", &activeCamera.upY, -5, 5)) {
					activeCamera.isChanged = true;
				}
				ImGui::SameLine();
				if (ImGui::SliderFloat("up z", &activeCamera.upZ, -5, 5)) {
					activeCamera.isChanged = true;
				}
				
				if (ImGui::SliderFloat("eye x", &activeCamera.eyeX, -5, 5)) {
					activeCamera.isChanged = true;
				}
				ImGui::SameLine();
				if (ImGui::SliderFloat("eye y", &activeCamera.eyeY, -5, 5)) {
					activeCamera.isChanged = true;
				}
				ImGui::SameLine();
				if (ImGui::SliderFloat("eye z", &activeCamera.eyeZ, -5, 5)) {
					activeCamera.isChanged = true;
				}

				if (ImGui::SliderFloat("at x", &activeCamera.atX, -5, 5)) {
					activeCamera.isChanged = true;
				}
				ImGui::SameLine();
				if (ImGui::SliderFloat("at y", &activeCamera.atY, -5, 5)) {
					activeCamera.isChanged = true;
				}
				ImGui::SameLine();
				if (ImGui::SliderFloat("at z", &activeCamera.atZ, -5, 5)) {
					activeCamera.isChanged = true;
				}
				if (activeCamera.isChanged) {
					activeCamera.CallLookAt();
				}
				ImGui::PopItemWidth();
				ChangeProjection();
			}
		}
		ImGui::PopID();
	}
}

void Menus::ChangeProjection() {
	ImGui::Text("Change Projection Parameters");
	auto& activeCamera = my_scene.GetActiveCamera();
	if (ImGui::Button("Reset Camera Projection")) {
		activeCamera.Reset1();
	}

	static int e = 0;
	ImGui::RadioButton("orthographic", &e, 0);
	ImGui::SameLine();
	ImGui::RadioButton("perspective", &e, 1);

	if (e == 0) {
		ImGui::PushItemWidth(100);
		if (ImGui::SliderFloat("left", &activeCamera.orthoLeft, -5, 5)) {
			activeCamera.isChanged1 = true;
		}
		if (ImGui::SliderFloat("right", &activeCamera.orthoRight, -5, 5)) {
			activeCamera.isChanged1 = true;
		}
		if (ImGui::SliderFloat("bottom", &activeCamera.orthoBottom, -5, 5)) {
			activeCamera.isChanged1 = true;
		}
		if (ImGui::SliderFloat("top", &activeCamera.orthoTop, -5, 5)) {
			activeCamera.isChanged1 = true;
		}
		if (ImGui::SliderFloat("near", &activeCamera.orthoNear, -5, 5)) {
			activeCamera.isChanged1 = true;
		}
		if (ImGui::SliderFloat("far", &activeCamera.orthoFar, -5, 5)) {
			activeCamera.isChanged1 = true;
		}
		ImGui::PopItemWidth();
		if (activeCamera.isChanged1) {
			activeCamera.SetOrthographic();
		}
	}
	if (e == 1) {
		ImGui::PushItemWidth(100);
		if (ImGui::SliderFloat("fovy", &activeCamera.perFOV, 1, 179)) {
			activeCamera.isChanged1 = true;
		}
		if (ImGui::SliderFloat("width", &activeCamera.perWidth, 1, my_io.DisplaySize.x)) {
			activeCamera.isChanged1 = true;
		}
		if (ImGui::SliderFloat("height", &activeCamera.perHeight, 1, my_io.DisplaySize.y)) {
			activeCamera.isChanged1 = true;
		}
		if (ImGui::SliderFloat("far perspective", &activeCamera.perFar, 0.2, 100)) {
			activeCamera.isChanged1 = true;
		}
		if (ImGui::SliderFloat("near perspective", &activeCamera.perNear, 0.1, 100)) {
			activeCamera.isChanged1 = true;
		}
		ImGui::PopItemWidth();
		if (activeCamera.isChanged1) {
			activeCamera.SetPerspective();
		}
	}	
}

void Menus::DrawTriangleFillMenu() {
	ImGui::Begin("Triangle Fill Menu");
	
	ImGui::Checkbox("Bounding Rectangle", &my_scene.showBoundingRectangle);

	ImGui::RadioButton("no fill", &my_scene.fillMode, 0);
	ImGui::SameLine();
	ImGui::RadioButton("z buffer color", &my_scene.fillMode, 2);
	
	ImGui::RadioButton("z buffer grey", &my_scene.fillMode, 1);
	ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::SliderFloat("grey scale level", &my_scene.greyScaleLevel, 1, 50);
	ImGui::PopItemWidth();

	ImGui::RadioButton("single color", &my_scene.fillMode, 3);
	ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::ColorEdit3("clear color", (float*)&my_scene.clear_color);
	ImGui::PopItemWidth();

	ImGui::End();
}

void Menus::DrawLightMenu() {
	ImGui::Begin("Light Menu");

	if (ImGui::Button("Add Another Light")) {
		std::shared_ptr<PointLight> light2(new PointLight());
		my_scene.AddLight(light2);
		my_scene.SetActiveLightIndex(my_scene.GetLightCount() - 1);
	}

	Settings();

	ImGui::End();
}

void Menus::Settings() {
	lightCount = my_scene.GetLightCount();

	for (int i = 0; i < lightCount; i++) {
		ImGui::PushID(i);
		string temp = "light " + to_string(i + 1);
		const char* name = temp.c_str();
		
		if (ImGui::CollapsingHeader(name)) {
			if (my_scene.GetActiveLightIndex() != i) {
				if (ImGui::Button("Activate")) {
					my_scene.SetActiveLightIndex(i);
				}
			}
			else {
				auto& activeLight = my_scene.GetActiveLight();
				if (ImGui::Button("Reset Light Position")) {
					activeLight.ResetPosition();
				}
				ImGui::PushItemWidth(200);
				if (ImGui::SliderFloat("x", &activeLight.x, (-my_io.DisplaySize.x), (my_io.DisplaySize.x))) {
					activeLight.isChangedP = true;
				}
				if (ImGui::SliderFloat("y", &activeLight.y, (-my_io.DisplaySize.y), (my_io.DisplaySize.y))) {
					activeLight.isChangedP = true;
				}
				if (ImGui::SliderFloat("z", &activeLight.z, -1000, 1000)) {
					activeLight.isChangedP = true;
				}
				if (activeLight.isChangedP) {
					activeLight.ChangePosition();
				}
				ImGui::PopItemWidth();

				ImGui::RadioButton("ambient", &activeLight.lightType, 0);
				ImGui::SameLine();
				ImGui::RadioButton("diffuse", &activeLight.lightType, 1);
				ImGui::SameLine();
				ImGui::RadioButton("specular", &activeLight.lightType, 2);

				ImGui::Text("Light Colors");
				ImGui::ColorEdit3("ambient1", (float*)&activeLight.LambientLight);
				ImGui::ColorEdit3("diffuse1", (float*)&activeLight.LdiffuseLight);
				ImGui::ColorEdit3("specular1", (float*)&activeLight.LspecularLight);
				ImGui::Text("Model Colors");
				ImGui::ColorEdit3("ambient2", (float*)&activeLight.MambientLight);
				ImGui::ColorEdit3("diffuse2", (float*)&activeLight.MdiffuseLight);
				ImGui::ColorEdit3("specular2", (float*)&activeLight.MspecularLight);

				ImGui::SliderFloat("specular shine", &activeLight.shine, 0.01, 1);

				ImGui::RadioButton("flat", &activeLight.shading, 0);
				ImGui::SameLine();
				ImGui::RadioButton("gouraud", &activeLight.shading, 1);
				ImGui::SameLine();
				ImGui::RadioButton("phong", &activeLight.shading, 2);
			}
		}
		ImGui::PopID();
	}

}