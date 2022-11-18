#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"

class MeshModel {
	public:
		MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name);
		virtual ~MeshModel();
		
		const Face& GetFace(int index) const;
		int GetFacesCount() const;
		
		const std::string& GetModelName() const;

		const glm::vec3& GetVertex(const int index) const;

		int modelScaling, worldScaling;
		int modelTranslationX, modelTranslationY, modelTranslationZ;
		int worldTranslationX, worldTranslationY, worldTranslationZ;
		int modelRotationX, modelRotationY, modelRotationZ;
		int worldRotationX, worldRotationY, worldRotationZ;
	
	private:
		std::vector<Face> faces;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::string model_name;

		glm::mat4x4 modelTransformation, worldTransformation;
		glm::mat4x4 modelRotation, worldRotation;
		glm::mat4x4 modelScalingMat, worldScalingMat;
		glm::mat4x4 modelTranslation, worldTranslation;
};
