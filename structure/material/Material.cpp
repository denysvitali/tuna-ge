#include <glm/vec4.hpp>
#include "Material.h"
#include <GL/freeglut.h>

using namespace tunage;

void tunage::Material::render()
{
	if (texture != nullptr) {
		texture->render();
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,
		glm::value_ptr(ambient));
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,
		glm::value_ptr(diffuse));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
		glm::value_ptr(specular));
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,
				 glm::value_ptr(emission));
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,
		shininess);
}

void Material::setAmbient(glm::vec3 light) {
    ambient = glm::vec4(light[0], light[1], light[2], 1.0f);
}

void Material::setDiffuse(glm::vec3 light) {
    diffuse = glm::vec4(light[0], light[1], light[2], 1.0f);
}

void Material::setSpecular(glm::vec3 light) {
    specular = glm::vec4(light[0], light[1], light[2], 1.0f);
}

void Material::setEmission(glm::vec3 light) {
    emission = glm::vec4(light[0], light[1], light[2], 1.0f);
}

void Material::setShininess(int intensity) {
    shininess = intensity;
}

glm::vec4 Material::getAmbient() const {
    return this->ambient;
}


glm::vec4 Material::getDiffuse() const {
    return this->diffuse;
}


glm::vec4 Material::getEmission() const {
    return this->emission;
}


glm::vec4 Material::getSpecular() const {
    return this->specular;
}

int Material::getShininess() const {
    return this->shininess;
}

void tunage::Material::setTexture(Texture* texture)
{
	this->texture = texture;
}
