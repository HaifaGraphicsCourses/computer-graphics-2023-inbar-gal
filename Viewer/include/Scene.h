#pragma once

#include <vector>
#include <memory>

#include "Camera.h"
#include "MeshModel.h"
#include "PointLight.h"

using namespace std;

class Scene {
	public:
		Scene();

		void AddModel(const shared_ptr<MeshModel>& mesh_model);
		int GetModelCount() const;
		MeshModel& GetModel(int index) const;
		MeshModel& GetActiveModel() const;
	
		void AddCamera(const shared_ptr<Camera>& camera);
		int GetCameraCount() const;
		Camera& GetCamera(int index) const;
		Camera& GetActiveCamera() const;

		void SetActiveCameraIndex(int index);
		int GetActiveCameraIndex() const;

		void SetActiveModelIndex(int index);
		int GetActiveModelIndex() const;

		int ModelVectorEmpty() const;
		bool showAxes;
		bool showBoundingBox;
		bool showFaceNormals;
		bool showVertexNormals;
		bool showBoundingRectangle;

		int fillMode;
		float greyScaleLevel;
		glm::vec4 clear_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.00f);

		void AddLight(const shared_ptr<PointLight>& light);
		int GetLightCount() const;
		PointLight& GetLight(int index) const;
		PointLight& GetActiveLight() const;
		void SetActiveLightIndex(int index);
		int GetActiveLightIndex() const;

		int textureMode;
	private:
		vector<shared_ptr<MeshModel>> mesh_models;
		vector<shared_ptr<Camera>> cameras;
		vector<shared_ptr<PointLight>> lights;

		int active_camera_index;
		int active_model_index;
		int active_light_index;
};