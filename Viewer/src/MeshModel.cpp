#include "MeshModel.h"

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices,
	std::vector<glm::vec3> normals, const std::string& model_name) :
	faces(faces), vertices(vertices), normals(normals),
	modelTransformation(1), worldTransformation(1),
	modelRotation(1), worldRotation(1),
	modelScalingMat(1), worldScalingMat(1),
	modelTranslation(1), worldTranslation(1) {
	// begin
	modelScaling = worldScaling = 1;
	modelTranslationX = modelTranslationY = modelTranslationZ = 0;
	worldTranslationX = worldTranslationY = worldTranslationZ = 0;
	modelRotationX = modelRotationY = modelRotationZ = 0;
	worldRotationX = worldRotationY = worldRotationZ = 0;
}

MeshModel::~MeshModel() {
}

const Face& MeshModel::GetFace(int index) const {
	return faces[index];
}

int MeshModel::GetFacesCount() const {
	return faces.size();
}

const std::string& MeshModel::GetModelName() const {
	return model_name;
}

const glm::vec3& MeshModel::GetVertex(const int index) const {
	return vertices[index];
}