#include "Camera.h"
#include <GL/freeglut.h>

#ifdef DEBUG
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>
#include <structure/program/Program.h>
#include <tuna-ge.h>

#endif

using namespace tunage;

void Camera::setMode(CameraMode mode) {
	this->mode = mode;
}

glm::mat4 Camera::getInverseMatrix() const {
	return glm::inverse(getMatrix());
}

ProjectionMode Camera::getProjMode() const {
	return projMode;
}

void Camera::setProjMode(ProjectionMode mode) {
	projMode = mode;
}

//	Generate the projection matrix using the camera settings
glm::mat4 Camera::getProjectionMatrix() const {
	switch(getProjMode()){
		case PERSPECTIVE:
			return glm::perspective(glm::radians(FOVangle), (float) (screen_w) / (float) (screen_h), nearPlane, farPlane);
		case ORTHOGRAPHIC:
			// TODO: Test
			return glm::ortho(0.0f, (float) screen_w, (float) screen_h, 0.0f, nearPlane, farPlane);
	}

	throw std::runtime_error("Camera::getProjectionMatrix() on an invalid camera (doens't have a valid Projection Mode)");
}

//	Loads the projection matrix used by FreeGLUT to render the scene
void Camera::loadProjectionMatrix() {
	//glMatrixMode(GL_PROJECTION);

	//glLoadMatrixf(glm::value_ptr(Projection));
	
	//glMatrixMode(GL_MODELVIEW);
}

void Camera::setFOV(float FOVangle) {
	this->FOVangle = FOVangle;
}

float Camera::getFOV() const {
	return FOVangle;
}

void Camera::setNearPlane(float nearPlane) {
	this->nearPlane = nearPlane;
}

float Camera::getNearPlane() const {
	return nearPlane;
}

void Camera::setFarPlane(float farPlane) {
	this->farPlane = farPlane;
}

float Camera::getFarPlane() const {
	return farPlane;
}

void Camera::setScreenSize(int screen_w, int screen_h) {
	this->screen_w = screen_w;
	this->screen_h = screen_h;
}

void Camera::setPos(glm::vec3 cameraPos) {
	this->position = cameraPos;
}

glm::vec3 Camera::getPos() const {
	return position;
}

void Camera::setFront(glm::vec3 cameraFront) {
	this->front = cameraFront;
}

glm::vec3 Camera::getFront() {
	switch (mode){
		case LOOK_AT_NODE:
			if(lookAtNode != nullptr){
				glm::vec3 lanPos(lookAtNode->getRenderMatrix() * glm::vec4(0,0,0,1));
				front = glm::normalize(lanPos - getAbsolutePosition());
			}
			break;
		case LOOK_AT_POINT:
			front = glm::normalize(point - getAbsolutePosition());
			break;
	}
	return front;
}

void Camera::setUp(glm::vec3 cameraUp) {
	this->up = cameraUp;
}

glm::vec3 Camera::getUp() const {
	return up;
}

//	Updates the current camera matrix using the glm LookAt method
void Camera::update() {
	switch(mode) {
		case LOOK_AT_POINT:
			this->setMatrix(glm::lookAt(position, point, up));
			break;
		case LOOK_AT_NODE: {
				glm::vec4 lookAtPosition =  (this->lookAtNode->getRenderMatrix()) * glm::vec4(0,0,0, 1) ;
				glm::vec3 lookAtPosition3 = glm::vec3(lookAtPosition.x, lookAtPosition.y, lookAtPosition.z);

	#ifdef DEBUG
	#if 0
			std::cout << "Looking at " << lookAtPosition3[0] << ", " << lookAtPosition3[1] << ","
					<< lookAtPosition3[2] << std::endl;
			std::cout << "-------DEBUG------------" << std::endl;
			std::cout << glm::to_string(position) << std::endl;
			std::cout << glm::to_string(lookAtPosition3) << std::endl;
			std::cout << glm::to_string(this->lookAtNode->getRenderMatrix()) << std::endl;
			std::cout << "-----------------------" << std::endl;
	#endif
	#endif
			if (this->lookAtNode != nullptr) {
					this->setMatrix(glm::lookAt(position, lookAtPosition3, up));
				}
			}
			break;
		case LOOK_TOWARDS_VECTOR:
			this->setMatrix(glm::lookAt(position, position + front, up));
			break;
		default:
			throw std::runtime_error("Invalid Camera Mode!");
	}
}

//	Return the position of the camera relative to the scene
glm::vec3 tunage::Camera::getAbsolutePosition() const
{
	glm::vec4 composedVec = glm::vec4(position,1);
	if (getParent() != nullptr) {
		composedVec = getParent()->getRenderMatrix() * composedVec;
		return composedVec;
	}
	return composedVec;
}

//	Different from the getRenderMatrix method in node because of the inversion needed for the camera
glm::mat4 Camera::getRenderMatrix() const {
	glm::mat4 composedMatrix;
	if (getParent() != nullptr) {
		composedMatrix = getMatrix() * glm::inverse(getParent()->getRenderMatrix());
		return composedMatrix;
	}
	return getMatrix();
}

void Camera::lookAt(glm::vec3 pointInSpace) {
	this->point = pointInSpace;
}

CameraMode Camera::getMode() {
	return mode;
}

glm::vec3 Camera::getLookAtPoint() const {
	return point;
}

void Camera::lookAt(Node* node){
	this->lookAtNode = node;
}

Node* Camera::getLookAtNode() const {
	return this->lookAtNode;
}
