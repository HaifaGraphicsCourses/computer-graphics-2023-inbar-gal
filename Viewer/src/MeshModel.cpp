#include "MeshModel.h"
#include <iostream>
#include <glad/glad.h>

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
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
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
	glm::vec4 newV = glm::vec4(vector.x, vector.y, vector.z, 1.0f);
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

void MeshModel::print(glm::mat4x4 view, glm::mat4x4 projection) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << projection[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << glm::inverse(view)[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << worldTransformation[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << modelTransformation[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, const std::string& model_name) :
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

	modelVertices.reserve(3 * faces.size());
	for (int i = 0; i < faces.size(); i++) {
		Face currentFace = faces.at(i);
		/*for (int j = 0; j < 3; j++) {
			int vertexIndex = currentFace.GetVertexIndex(j) - 1;

			Vertex vertex;
			vertex.position = vertices[vertexIndex];
			vertex.normal = normals[vertexIndex];

			if (textureCoords.size() > 0) {
				int textureCoordsIndex = currentFace.GetTextureIndex(j) - 1;
				vertex.texCoords = textureCoords[textureCoordsIndex];
			}

			modelVertices.push_back(vertex);
		}*/

		int i1 = currentFace.GetVertexIndex(0) - 1;
		int i2 = currentFace.GetVertexIndex(1) - 1;
		int i3 = currentFace.GetVertexIndex(2) - 1;

		Vertex vertex1, vertex2, vertex3;

		vertex1.position = vertices[i1];
		vertex2.position = vertices[i2];
		vertex3.position = vertices[i3];

		vertex1.normal = normals[i1];
		vertex2.normal = normals[i2];
		vertex3.normal = normals[i3];

		if (textureCoords.size() > 0) {
			int i4 = currentFace.GetTextureIndex(0) - 1;
			int i5 = currentFace.GetTextureIndex(1) - 1;
			int i6 = currentFace.GetTextureIndex(2) - 1;

			vertex1.texCoords = textureCoords[i4];
			vertex2.texCoords = textureCoords[i5];
			vertex3.texCoords = textureCoords[i6];

			glm::vec3 edge1 = vertex2.position - vertex1.position;
			glm::vec3 edge2 = vertex3.position - vertex1.position;
			glm::vec2 deltaUV1 = vertex2.texCoords - vertex1.texCoords;
			glm::vec2 deltaUV2 = vertex3.texCoords - vertex1.texCoords;

			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			glm::vec3 tangent1, bitangent1;
			
			tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

			bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

			vertex1.tangent = tangent1;
			vertex2.tangent = tangent1;
			vertex3.tangent = tangent1;

			vertex1.bitangent = bitangent1;
			vertex2.bitangent = bitangent1;
			vertex3.bitangent = bitangent1;
		}

		modelVertices.push_back(vertex1);
		modelVertices.push_back(vertex2);
		modelVertices.push_back(vertex3);
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(Vertex), &modelVertices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)nullptr);
	glEnableVertexAttribArray(0);

	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	// Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);

	// Vertex tangent
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(3);

	// Vertex bitangent
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
	glEnableVertexAttribArray(4);

	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);
}

const glm::mat4x4& MeshModel::GetWorldTransformation() const {
	return worldTransformation;
}

const glm::mat4x4& MeshModel::GetModelTransformation() const {
	return modelTransformation;
}

const std::vector<Vertex>& MeshModel::GetModelVertices() {
	return modelVertices;
}

/*GLuint MeshModel::GetVAO() const {
	return vao;
}*/