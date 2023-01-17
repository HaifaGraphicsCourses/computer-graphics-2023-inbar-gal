#include "PointLight.h"

PointLight::PointLight() {
	x = y = z = 0.0;

	lightType = 0;
	shine = 1.0f;
	shading = 0;
}

PointLight::~PointLight() {

}

void PointLight::ChangePosition() {
	position = glm::vec3(x, y, z);
}

void PointLight::ResetPosition() {
	x = y = z = 0.0;
	position = glm::vec3(0.0, 0.0, 0.0);
}

