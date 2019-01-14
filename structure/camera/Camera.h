#pragma once

#include "../../libapi.h"
#include "../node/Node.h"

#include "CameraMode.h"
#include "ProjectionMode.h"

namespace tunage {
	class LIB_API Camera : public Node{
	public:
		explicit Camera(const char* name) : Node{name} {}
		explicit Camera(const char* name, CameraMode mode): Node{name}, mode{mode} {}

		void setMode(CameraMode mode);
		CameraMode getMode();

		void setProjMode(ProjectionMode projMode);
		ProjectionMode getProjMode() const;

		glm::mat4 getInverseMatrix() const;
		glm::mat4 getProjectionMatrix() const;

		void loadProjectionMatrix();

		void setFOV(float FOVangle);
		float getFOV() const;
		void setNearPlane(float nearPlane);
		float getNearPlane() const;
		void setFarPlane(float farPlane);
		float getFarPlane() const;
		void setScreenSize(int width, int height);

		// Camera Settings
		void setPos(glm::vec3 cameraPos);
		glm::vec3 getPos() const;

		void setFront(glm::vec3 cameraFront);
		glm::vec3 getFront() const;

		void setUp(glm::vec3 cameraUp);
		glm::vec3 getUp() const;

		void lookAt(glm::vec3 pointInSpace);
		glm::vec3 getLookAtPoint() const;

		void update();

		glm::mat4 getRenderMatrix() const override;
		

	private:
		CameraMode mode = CameraMode::LOOK_TOWARDS_VECTOR;

		float FOVangle = 45.0f;

		int screen_w = 100;
		int screen_h = 100;
		float nearPlane = 0.1f;
		float farPlane = 500.0f;

		ProjectionMode projMode = PERSPECTIVE;

		glm::vec3 position = glm::vec3(0,0,0);
		glm::vec3 front = glm::vec3(0,0,-1);
		glm::vec3 up = glm::vec3(0,1,0);
		glm::vec3 point = glm::vec3(0,0,0);

	};
}