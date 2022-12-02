#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
	
}

Camera::~Camera() {
	
}

const glm::mat4x4& Camera::GetProjectionTransformation() const {
	return projection_transformation;
}

const glm::mat4x4& Camera::GetViewTransformation() const {
	return view_transformation;
}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up) {
	view_transformation = glm::lookAt(eye, at, up);
}

void Camera::SetOrthographic() {
	projection_transformation = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, orthoNear, orthoFar);
}

void Camera::SetPerspective() {
	projection_transformation = glm::perspective(glm::radians(perFOV), perWidth / perHeight, perNear, perFar);
}