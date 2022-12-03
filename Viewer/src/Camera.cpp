#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
	eyeX = eyeY = 0;
	eyeZ = 1;
	atX = atY = atZ = 0;
	upX = upZ = 0;
	upY = 1;

	orthoLeft = orthoBottom = -1;
	orthoRight = orthoTop = 1;
	orthoNear = orthoFar = 0;

	perFOV = 45;
	perHeight = perWidth = 1;
	perNear = 0.1;
	perFar = 100;

	isChanged = false;
	isChanged1 = false;

	CallLookAt();
	SetOrthographic();
}

Camera::~Camera() {

}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up) {
	view_transformation = glm::lookAt(eye, at, up);
}

const glm::mat4x4& Camera::GetProjectionTransformation() const {
	return projection_transformation;
}

const glm::mat4x4& Camera::GetViewTransformation() const {
	return view_transformation;
}

void Camera::Reset() {
	eyeX = eyeY = 0;
	eyeZ = 1;
	atX = atY = atZ = 0;
	upX = upZ = 0;
	upY = 1;
	CallLookAt();
}

void Camera::Orbit() {

}

void Camera::CallLookAt() {
	glm::vec3 eye(eyeX, eyeY, eyeZ);
	glm::vec3 at(atX, atY, atZ);
	glm::vec3 up(upX, upY, upZ);
	SetCameraLookAt(eye, at, up);
}

void Camera::SetOrthographic() {
	projection_transformation = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, orthoNear, orthoFar);
}

void Camera::SetPerspective() {
	projection_transformation = glm::perspective(glm::radians(perFOV), perWidth / perHeight, perNear, perFar);
}

void Camera::Reset1() {
	orthoLeft = orthoBottom = -1;
	orthoRight = orthoTop = 1;
	orthoNear = orthoFar = 0;

	perFOV = 45;
	perHeight = perWidth = 1;
	perNear = 0.1;
	perFar = 100;

	SetOrthographic();
}