#pragma once
#include <glm/glm.hpp>

class PointLight {
	public:
		PointLight();
		virtual ~PointLight();

		glm::vec3 position;
		float x, y, z;
		bool isChangedP;
		void ChangePosition();
		void ResetPosition();

		int lightType;
		glm::vec4 ambientLight = glm::vec4(0.0f, 0.0f, 0.0f, 0.00f);
		glm::vec4 diffuseLight = glm::vec4(0.0f, 0.0f, 0.0f, 0.00f);
		glm::vec4 specularLight = glm::vec4(0.0f, 0.0f, 0.0f, 0.00f);

		int shading;

	private:

};

