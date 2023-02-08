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

		/*glm::vec4 LambientLight = glm::vec4(0.172, 0.172, 0.172, 0.00f);
		glm::vec4 LdiffuseLight = glm::vec4(0.637, 0.637, 0.637, 0.00f);
		glm::vec4 LspecularLight = glm::vec4(0.344, 0.975, 0.516, 0.00f);
		glm::vec4 MambientLight = glm::vec4(1.0f, 0.0f, 0.0f, 0.00f);
		glm::vec4 MdiffuseLight = glm::vec4(0.0f, 0.814, 0.187, 0.00f);
		glm::vec4 MspecularLight = glm::vec4(0.559, 1.0, 0.0, 0.00f);*/

		float shine;

		int shading;
	private:

};

