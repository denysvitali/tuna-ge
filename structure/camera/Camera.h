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

		//	Camera render matrix methods
		glm::mat4 getRenderMatrix() const override;
		glm::mat4 getInverseMatrix() const;
		void update();
		glm::vec3 getRelativePosition() const;
		
		//	Projection methods
		glm::mat4 getProjectionMatrix() const;
		void loadProjectionMatrix();
		void setProjMode(ProjectionMode projMode);
		ProjectionMode getProjMode() const;
		
		//	Camera settings methods
		void setFOV(float FOVangle);
		float getFOV() const;
		void setNearPlane(float nearPlane);
		float getNearPlane() const;
		void setFarPlane(float farPlane);
		float getFarPlane() const;
		void setScreenSize(int width, int height);
		void setMode(CameraMode mode);
		CameraMode getMode();
		void setPos(glm::vec3 cameraPos);
		glm::vec3 getPos() const;
		void setFront(glm::vec3 cameraFront);
		glm::vec3 getFront();
		void setUp(glm::vec3 cameraUp);
		glm::vec3 getUp() const;

		void lookAt(glm::vec3 pointInSpace);
		void lookAt(Node* node);
		glm::vec3 getLookAtPoint() const;
		Node* getLookAtNode() const;

	private:
		//	Default settings
		CameraMode mode = CameraMode::LOOK_TOWARDS_VECTOR;
		ProjectionMode projMode = PERSPECTIVE;
		float FOVangle = 45.0f;
		int screen_w = 100;
		int screen_h = 100;
		float nearPlane = 0.1f;
		float farPlane = 500.0f;
		glm::vec3 position = glm::vec3(0,0,0);	//	Camera is at position (0,0,0) (From the current render matrix)
		glm::vec3 front = glm::vec3(0,0,-1);	//	Camera looks toward the specified vector
		glm::vec3 up = glm::vec3(0,1,0);		//	Camera Roll inclination (From the current render matrix)
		glm::vec3 point = glm::vec3(0,0,0);		//	Alternative to front: Camera looks toward the specified point

		Node* lookAtNode = nullptr;
	};
}