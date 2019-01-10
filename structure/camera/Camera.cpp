#include "Camera.h"
#include <GL/freeglut.h>

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

glm::mat4 Camera::getProjectionMatrix() const {
	switch(getProjMode()){
		case PERSPECTIVE:
			return glm::perspective(glm::radians(FOVangle), (float) (screen_w) / (float) (screen_h), nearPlane, farPlane);
		case ORTHOGRAPHIC:
			// TODO: Test
			return glm::ortho(0.0f, (float) screen_w, (float) screen_h, 0.0f, nearPlane, farPlane);
	}
}

void Camera::loadProjectionMatrix() {
	glMatrixMode(GL_PROJECTION);
	glm::mat4 Projection = getProjectionMatrix();
	glLoadMatrixf(glm::value_ptr(Projection));
	glMatrixMode(GL_MODELVIEW);
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

glm::vec3 Camera::getFront() const {
	return front;
}

void Camera::setUp(glm::vec3 cameraUp) {
	this->up = cameraUp;
}

glm::vec3 Camera::getUp() const {
	return up;
}
void Camera::update() {
	glm::mat4 mat;
	switch(mode){
		case LOOK_AT_POINT:
			mat = glm::lookAt(position, point, up);
			break;
		case LOOK_TOWARDS_VECTOR:
			mat = glm::lookAt(position, position + point, up);
			break;
		default:
			throw std::runtime_error("Invalid Camera Mode!");
	}

	this->setMatrix(mat);
}

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
