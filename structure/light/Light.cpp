#include "Light.h"

// FreeGLUT:
#include <GL/freeglut.h>
#include "../material/Material.h"
#include "../../tuna-ge.h"

using namespace tunage;

RGBColor Light::getColor() const {
	return m_color;
}

void Light::setColor(RGBColor color) {
	this->m_color = color;
}


void Light::render() {
	//TODO: implement various light modes
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(getMatrix()));

	// Draw a small emissive sphere to show light position:
	//glLoadMatrixf(glm::value_ptr(glm::mat4(1.0f)));
	Material m{};
	m.setAmbient(glm::vec3(1.0f, 1.0f, 1.0f));
	m.setSpecular(glm::vec3(0.6f, 0.6f, 0.6f));
	m.setShininess(120);
	m.setDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));
	m.setEmission(glm::vec3(1.0f, 1.0f, 1.0f));

	TunaGE::setMaterial(m);

	glutSolidSphere(0.1f, 40, 40);

	// Light position is set to object coordinates and is modified by the current OpenGL matrix (as with any other object):
	glm::vec4 ambient_wi = glm::vec4(
			lightAmbient[0] * intensity,
			lightAmbient[1] * intensity,
			lightAmbient[2] * intensity,
			1.0f);

	glm::vec4 diffuse_wi = glm::vec4(
			lightDiffuse[0] * intensity,
			lightDiffuse[1] * intensity,
			lightDiffuse[2] * intensity,
			1.0f);

	glm::vec4 specular_wi = glm::vec4(
			lightSpecular[0] * intensity,
			lightSpecular[1] * intensity,
			lightSpecular[2] * intensity,
			1.0f);

	glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_POSITION, glm::value_ptr(glm::vec4(0, 0, 0, 1.0f)));
	glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_AMBIENT, glm::value_ptr(ambient_wi));
	glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_DIFFUSE, glm::value_ptr(diffuse_wi));
	glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_SPECULAR, glm::value_ptr(specular_wi));

}

void Light::enable() {
	glEnable(static_cast<GLenum>(light + GL_LIGHT0));
}

void Light::disable() {
	glDisable(static_cast<GLenum>(light + GL_LIGHT0));
}

void Light::setIntensity(float f) {
	if(f >= 1.0f){
		intensity = 1.0f;
		return;
	}

	if(f < 0.0f){
		intensity = 0.0f;
		return;
	}

	intensity = f;
}

void Light::setLightAmbient(glm::vec3 lightAmbient) {
	this->lightAmbient = glm::vec4(lightAmbient, 1.0f);
}

void Light::setLightDiffuse(glm::vec3 lightDiffuse) {
	this->lightDiffuse = glm::vec4(lightDiffuse, 1.0f);
}

void Light::setLightSpecular(glm::vec3 lightSpecular) {
	this->lightSpecular = glm::vec4(lightSpecular, 1.0f);
}

void Light::setLightDirection(glm::vec3 lightDirection) {
	this->lightDirection = lightDirection;
}

void Light::setLightCutoff(float lightCutoff) {
	this->lightCutoff = lightCutoff;
}

void Light::setLight(int light) {
	if (light >= 0 && light <= 7) {
		this->light = light;
	}
}

void Light::setMatrix(glm::mat4 matrix) {
	Node::setMatrix(matrix);
}
