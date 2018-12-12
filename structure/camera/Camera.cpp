#include "Camera.h"
#include <GL/freeglut.h>

glm::mat4 tunage::Camera::getInverseMatrix() const
{
	return glm::inverse(getMatrix());
}

glm::mat4 tunage::Camera::getProjectionMatrix() const
{
	return glm::perspective(glm::radians(FOVangle), (float)(screen_w) / (float)(screen_h), nearPlane, farPlane);
}

void tunage::Camera::loadProjectionMatrix()
{
	glMatrixMode(GL_PROJECTION);
	glm::mat4 Projection = glm::perspective(glm::radians(FOVangle), (float)(screen_w) / (float)(screen_h), nearPlane, farPlane);
	glLoadMatrixf(glm::value_ptr(Projection));
	glMatrixMode(GL_MODELVIEW);
}

void tunage::Camera::setFOV(float FOVangle)
{
	this->FOVangle = FOVangle;
}

float tunage::Camera::getFOV() const
{
	return FOVangle;
}

void tunage::Camera::setNearPlane(float nearPlane)
{
	this->nearPlane = nearPlane;
}

float tunage::Camera::getNearPlane() const
{
	return nearPlane;
}

void tunage::Camera::setFarPlane(float farPlane)
{
	this->farPlane = farPlane;
}

float tunage::Camera::getFarPlane() const
{
	return farPlane;
}

void tunage::Camera::setScreenSize(int screen_w, int screen_h)
{
	this->screen_w = screen_w;
	this->screen_h = screen_h;
}

void tunage::Camera::setProjMode(bool projModemode)
{
	this->projMode = projMode;
}

void tunage::Camera::setCameraPos(glm::vec3 cameraPos)
{
	this->cameraPos = cameraPos;
}

void tunage::Camera::setCameraFront(glm::vec3 cameraFront)
{
	this->cameraFront = cameraFront;
}

void tunage::Camera::setCameraUp(glm::vec3 cameraUp)
{
	this->cameraUp = cameraUp;
}

void tunage::Camera::updateCamera()
{
	this->setMatrix(glm::lookAt(cameraPos, cameraFront, cameraUp));
}
