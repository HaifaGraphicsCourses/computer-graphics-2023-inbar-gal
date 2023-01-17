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
		glm::vec4 LambientLight = glm::vec4(1.0f, 0.0f, 0.0f, 0.00f);
		glm::vec4 LdiffuseLight = glm::vec4(0.0f, 1.0f, 0.0f, 0.00f);
		glm::vec4 LspecularLight = glm::vec4(0.0f, 0.0f, 1.0f, 0.00f);
		glm::vec4 MambientLight = glm::vec4(1.0f, 0.0f, 0.0f, 0.00f);
		glm::vec4 MdiffuseLight = glm::vec4(0.0f, 1.0f, 0.0f, 0.00f);
		glm::vec4 MspecularLight = glm::vec4(0.0f, 0.0f, 1.0f, 0.00f);

		float shine;

		int shading;
	private:

};

