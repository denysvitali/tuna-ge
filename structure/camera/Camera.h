#pragma once

#include "../../libapi.h"
#include "../node/Node.h"

namespace tunage {
	class LIB_API Camera : public Node{
	public:
		explicit Camera(std::string name) : Node{name} {}
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
		void setProjMode(bool projMode);
		void setCameraPos(glm::vec3 cameraPos);
		glm::vec3 getCameraPos() const;
		void setCameraFront(glm::vec3 cameraFront);
		glm::vec3 getCameraFront() const;
		void setCameraUp(glm::vec3 cameraUp);
		glm::vec3 getCameraUp() const;
		void setCameraSpeed(float cameraSpeed);
		float getCameraSpeed() const;
		
		void updateCamera();

		glm::mat4 getRenderMatrix() const override;
		

	private:
		int screen_w = 100;
		int screen_h = 100;
		float FOVangle = 45.0f;
		float nearPlane = 0.1f;
		float farPlane = 500.0f;
		bool projMode = false;
		float cameraSpeed = 0.1f;
		glm::vec3 cameraPos = glm::vec3(0,0,0);
		glm::vec3 cameraFront = glm::vec3(0,0,-1);
		glm::vec3 cameraUp = glm::vec3(0,1,0);

	};
}