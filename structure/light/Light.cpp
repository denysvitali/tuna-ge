#include "Light.h"
#include "../program/Program.h"

// FreeGLUT:
#include <GL/freeglut.h>
#include "../material/Material.h"
#include "../../tuna-ge.h"

using namespace tunage;

//	Render method using the light render matrix and default material
void Light::render() {

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(getRenderMatrix()));

	Material m{};
	m.setEmission(lightAmbient);
	//	Render material
	m.render();

	// Draw a small emissive sphere to show light position:
	glutSolidSphere(radius, 40, 40);

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

	////	Sets various FreeGLUT options based on the light type (0 = DIRECTIONAL, 1 = OMNI, 2 = SPOT)
	//switch (lightType){
	//	case 0:
	//		glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_POSITION, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	//		break;
	//	case 1:
	//		glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_POSITION, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	//		glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_SPOT_CUTOFF, &lightCutoff);
	//		break;
	//	case 2:
	//		glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_POSITION, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	//		glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_SPOT_CUTOFF, &lightCutoff);
	//		glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_SPOT_DIRECTION, glm::value_ptr(lightDirection));
	//		glLightf(static_cast<GLenum>(light + GL_LIGHT0), GL_SPOT_EXPONENT, spotExponent);
	//		break;
	//	default:
	//		break;
	//}
	//glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_AMBIENT, glm::value_ptr(ambient_wi));
	//glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_DIFFUSE, glm::value_ptr(diffuse_wi));
	//glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_SPECULAR, glm::value_ptr(specular_wi));

	
}

//	Render method using a material and render matrix passed as parameters
void tunage::Light::render(glm::mat4 pos, Material* mat)
{
	//glLoadMatrixf(glm::value_ptr(pos));

	//	Render material
	if (mat != nullptr) {
		mat->render();
	}

	//	Draw a small emissive sphere to show light position:
	//glutSolidSphere(radius, 40, 40);

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

	////	Sets various FreeGLUT options based on the light type (0 = DIRECTIONAL, 1 = OMNI, 2 = SPOT)
	//switch (lightType){
	//	case 0:
	//		glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_POSITION, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	//		break;
	//	case 1:
	//		glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_POSITION, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	//		glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_SPOT_CUTOFF, &lightCutoff);
	//		break;
	//	case 2:
	//		glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_POSITION, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	//		glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_SPOT_CUTOFF, &lightCutoff);
	//		glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_SPOT_DIRECTION, glm::value_ptr(lightDirection));
	//		break;
	//	default:
	//		break;
	//}
	////glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_CONSTANT_ATTENUATION, &attenuation);
	//glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_AMBIENT, glm::value_ptr(ambient_wi));
	//glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_DIFFUSE, glm::value_ptr(diffuse_wi));
	//glLightfv(static_cast<GLenum>(light + GL_LIGHT0), GL_SPECULAR, glm::value_ptr(specular_wi));

	Program::getCurrent()->setLight(this);
}

void Light::enable() {
	enabled = true;
	//glEnable(static_cast<GLenum>(light + GL_LIGHT0));
}
void Light::disable() {
	enabled = false;
	//glDisable(static_cast<GLenum>(light + GL_LIGHT0));
}

bool tunage::Light::isEnabled()
{
	return enabled;
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

//	Set light number used by FreeGLUT (can only be between 0 and 7; maximum of 8 lights rendered at the same time)
void Light::setLight(int light) {
	if (light >= 0 && light <= 7) {
		this->light = light;
	}
}

glm::vec3 Light::getLightAmbient() const {
    return lightAmbient;
}

glm::vec3 Light::getLightDiffuse() const {
	return lightDiffuse;
}

glm::vec3 Light::getLightSpecular() const {
	return lightSpecular;
}

void Light::setRadius(float radius) {
	this->radius = radius;
}

void Light::setType(unsigned int lightType) {
	this->lightType = lightType;
}

void Light::setSpotExponent(int val) {
	spotExponent = val;
}

int Light::getSpotExponent() {
	return spotExponent;
}

void Light::setInfluenceRadius(float f) {
	influence = f;
}

float Light::getInfluenceRadius() {
	return influence;
}

void tunage::Light::setReferenceLight(Light * referenceLight)
{
	this->referenceLight = referenceLight;
}

Light * tunage::Light::getReferenceLight() const
{
	return referenceLight;
}
