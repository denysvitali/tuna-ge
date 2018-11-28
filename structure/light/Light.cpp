#include "Light.h"

// FreeGLUT:
#include <GL/freeglut.h>

using namespace tunage;

RGBColor Light::getColor() const{
    return m_color;
}

void tunage::Light::setColor(RGBColor color)
{
	this->m_color = color;
}


void tunage::Light::render()
{
	
	//TODO: implement various light modes

	glm::mat4 composedMatrix = getRenderMatrix();
	glLoadMatrixf(glm::value_ptr(composedMatrix));

	// Draw a small emissive sphere to show light position:   
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr(glm::vec4(1.0f)));
	glutSolidSphere(0.5f, 8, 8);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr(glm::vec4(0.0f)));

	// Light position is set to object coordinates and is modified by the current OpenGL matrix (as with any other object):
	glm::vec4 objectCoordPosition(0.0f, 0.0f, 0.0f, 1.0f);
	glLightfv(light + GL_LIGHT0, GL_POSITION, glm::value_ptr(objectCoordPosition));
	glLightfv(light + GL_LIGHT0, GL_AMBIENT, glm::value_ptr(lightAmbient));
	glLightfv(light + GL_LIGHT0, GL_DIFFUSE, glm::value_ptr(lightDiffuse));
	glLightfv(light + GL_LIGHT0, GL_SPECULAR, glm::value_ptr(lightSpecular));
	
}

void tunage::Light::enableLight()
{
	glEnable(light + GL_LIGHT0);
}

void tunage::Light::setLightPosition(glm::vec3 lightPosition)
{
	this->lightPosition = lightPosition;
}

void tunage::Light::setLightAmbient(glm::vec3 lightAmbient)
{
	this->lightAmbient = glm::vec4(lightAmbient, 1.0f);
}

void tunage::Light::setLightDiffuse(glm::vec3 lightDiffuse)
{
	this->lightDiffuse = glm::vec4(lightDiffuse, 1.0f);
}

void tunage::Light::setLightSpecular(glm::vec3 lightSpecular)
{
	this->lightSpecular = glm::vec4(lightSpecular, 1.0f);
}

void tunage::Light::setLightDirection(glm::vec3 lightDirection)
{
	this->lightDirection = lightDirection;
}

void tunage::Light::setLightCutoff(float lightCutoff)
{
	this->lightCutoff = lightCutoff;
}

void tunage::Light::setLight(int light)
{
	if (light >= 0 && light <= 7) {
		this->light = light;
	}
}
