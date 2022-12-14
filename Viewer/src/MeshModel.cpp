#include "MeshModel.h"

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, 
					 std::vector<glm::vec3> normals, const std::string& model_name) : 
	faces(faces), vertices(vertices), normals(normals),
	modelTransformation(1.0f), worldTransformation(1.0f),
	modelRotation(1.0f), worldRotation(1.0f),
	modelScalingMat(1.0f), worldScalingMat(1.0f),
	modelTranslation(1.0f), worldTranslation(1.0f) {
	// begin
	const auto index = model_name.find_last_of('.');
	if (index == std::string::npos) {
		this->model_name = model_name;
	}
	else {
		this->model_name = model_name.substr(0, index);
	}

	modelScaling = worldScaling = 1;
	modelTranslationX = modelTranslationY = modelTranslationZ = 0;
	worldTranslationX = worldTranslationY = worldTranslationZ = 0;
	modelRotationX = modelRotationY = modelRotationZ = 0;
	worldRotationX = worldRotationY = worldRotationZ = 0;
	isChanged = false;
}

MeshModel::~MeshModel() {

}

MeshModel::MeshModel(const MeshModel& other) : 
	faces(other.faces), vertices(other.vertices), normals(other.normals),
	model_name(other.model_name),
	modelTransformation(other.modelTransformation), worldTransformation(other.worldTransformation) {
	// begin
	modelScaling = worldScaling = 1;
	modelTranslationX = modelTranslationY = modelTranslationZ = 0;
	worldTranslationX = worldTranslationY = worldTranslationZ = 0;
	modelRotationX = modelRotationY = modelRotationZ = 0;
	worldRotationX = worldRotationY = worldRotationZ = 0;

	CalculateMinMaxOfModel();
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

void MeshModel::ChangeModel() {
	glm::mat4 rotationX(1.0);
	glm::mat4 rotationY(1.0);
	glm::mat4 rotationZ(1.0);

	modelScalingMat = CreateScaling(modelScaling);
	modelTranslation = CreateTranslation(modelTranslationX, modelTranslationY, modelTranslationZ);
	rotationX = CreateRotation(1, modelRotationX);
	rotationY = CreateRotation(2, modelRotationY);
	rotationZ = CreateRotation(3, modelRotationZ);
	modelRotation = rotationX * rotationY * rotationZ;
	modelTransformation = modelTranslation * modelScalingMat * modelRotation;

	worldScalingMat = CreateScaling(worldScaling);
	worldTranslation = CreateTranslation(worldTranslationX, worldTranslationY, worldTranslationZ);
	rotationX = CreateRotation(1, worldRotationX);
	rotationY = CreateRotation(2, worldRotationY);
	rotationZ = CreateRotation(3, worldRotationZ);
	worldRotation = rotationX * rotationY * rotationZ;
	worldTransformation = worldTranslation * worldScalingMat * worldRotation;
}

glm::mat4 MeshModel::CreateScaling(float data) {
	return glm::mat4(
		data, 0, 0, 0,
		0, data, 0, 0,
		0, 0, data, 0,
		0, 0, 0, 1
	);
}

glm::mat4 MeshModel::CreateTranslation(float x, float y, float z) {
	return glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1
	);
}

glm::mat4 MeshModel::CreateRotation(int type, float degree) {
	float pi = acos(-1);
	float cost = cos(degree * pi / 180.0);
	float sint = sin(degree * pi / 180.0);
	
	if (type == 1) {
		return glm::mat4(
			1, 0, 0, 0,
			0, cost, sint, 0,
			0, -sint, cost, 0,
			0, 0, 0, 1
		);
	}
	else if (type == 2) {
		return glm::mat4(
			cost, 0, sint, 0,
			0, 1, 0, 0,
			-sint, 0, cost, 0,
			0, 0, 0, 1
		);
	}
	else if (type == 3) {
		return glm::mat4(
			cost, sint, 0, 0,
			-sint, cost, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}
}

MeshModel MeshModel::GetNewModel(glm::mat4x4 view, glm::mat4x4 projection) {
	MeshModel newM(*this);
	glm::mat4 transformation = projection * glm::inverse(view) * worldTransformation * modelTransformation;
	newM.ApplyTransformation(transformation);
	return newM;
}

void MeshModel::ApplyTransformation(glm::mat4 tranformation) {
	for (int i = 0; i < vertices.size(); i++) {
		auto& vertex = vertices[i];
		glm::vec4 transformed = tranformation * glm::vec4(vertex, 1.0f);
		vertex.x = transformed.x + 960;
		vertex.y = transformed.y + 540;
		vertex.z = transformed.z;
	}
}

void MeshModel::Reset() {
	modelScaling = worldScaling = 1;
	modelTranslationX = modelTranslationY = modelTranslationZ = 0;
	worldTranslationX = worldTranslationY = worldTranslationZ = 0;
	modelRotationX = modelRotationY = modelRotationZ = 0;
	worldRotationX = worldRotationY = worldRotationZ = 0;

	modelTransformation = glm::mat4(1.0);
	worldTransformation = glm::mat4(1.0);
}

void MeshModel::CalculateMinMaxOfModel() {
	glm::vec3 temp = vertices[0];
	minX = temp.x;
	maxX = temp.x;
	minY = temp.y;
	maxY = temp.y;
	minZ = temp.z;
	maxZ = temp.z;

	for (int i = 1; i < vertices.size(); i++) {
		temp = vertices[i];
		minX = fmin(minX, temp.x);
		minY = fmin(minY, temp.y);
		minZ = fmin(minZ, temp.z);
		maxX = fmax(maxX, temp.x);
		maxY = fmax(maxY, temp.y);
		maxZ = fmax(maxZ, temp.z);
	}
}

void MeshModel::ChangeVectors(glm::vec3& vector, int type, glm::mat4x4 view, glm::mat4x4 projection) {
	glm::vec4 newV = glm::vec4(vector, 1);
	if (type == 1) {
		newV = projection * glm::inverse(view) * worldTransformation * modelTransformation * newV;
	}
	else if (type == 2) {
		newV = projection * glm::inverse(view) * worldTransformation * newV;
	}
	vector.x = newV.x + 960;
	vector.y = newV.y + 540;
	vector.z = newV.z;
}

const glm::vec3& MeshModel::GetNormal(const int index) const {
	return normals[index];
}