#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"

class MeshModel {
	public:
		MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name);
		virtual ~MeshModel();
		MeshModel(const MeshModel& other);

		const Face& GetFace(int index) const;
		int GetFacesCount() const;

		const std::string& GetModelName() const;

		const glm::vec3& GetVertex(const int index) const;

		float modelScaling, worldScaling;
		float modelTranslationX, modelTranslationY, modelTranslationZ;
		float worldTranslationX, worldTranslationY, worldTranslationZ;
		float modelRotationX, modelRotationY, modelRotationZ;
		float worldRotationX, worldRotationY, worldRotationZ;

		bool isChanged;

		void ChangeModel();
		glm::mat4 CreateScaling(float data);
		glm::mat4 CreateTranslation(float x, float y, float z);
		glm::mat4 CreateRotation(int type, float degree);
		MeshModel GetNewModel();
		void ApplyTransformation(glm::mat4 tranformation);

		void Reset();
	private:
		std::vector<Face> faces;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::string model_name;

		glm::mat4 modelTransformation, worldTransformation;
		glm::mat4 modelRotation, worldRotation;
		glm::mat4 modelScalingMat, worldScalingMat;
		glm::mat4 modelTranslation, worldTranslation;
};