#pragma once
#include <glm/glm.hpp>

class Camera {
	public:
		Camera();
		virtual ~Camera();

		void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);

		const glm::mat4x4& GetProjectionTransformation() const;
		const glm::mat4x4& GetViewTransformation() const;

		void Reset();
		void Orbit();
		void CallLookAt();
		void SetOrthographic();
		void SetPerspective();
		void Reset1();
		
		float eyeX, eyeY, eyeZ;
		float atX, atY, atZ;
		float upX, upY, upZ;
		
		float orthoLeft, orthoRight, orthoTop, orthoBottom, orthoNear, orthoFar;
		float perNear, perFar, perWidth, perHeight, perFOV;

		bool isChanged;
		bool isChanged1;

		glm::vec3 GetPosition();
	private:
		glm::mat4x4 view_transformation;
		glm::mat4x4 projection_transformation;
};
