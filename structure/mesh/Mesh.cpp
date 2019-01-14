

#include "Mesh.h"
#include <GL/freeglut.h>

void tunage::Mesh::render() {
	if (material != nullptr) {
		material->render();
	}
	glm::mat4 composedMatrix = getRenderMatrix();
	glLoadMatrixf(glm::value_ptr(composedMatrix));

	glBegin(GL_TRIANGLES);
	for (auto &face : faces) {
		glNormal3f(face.getNorm().x, -face.getNorm().y, face.getNorm().z);
		glTexCoord2f(face.getUV().x, -face.getUV().y);
		glVertex3f(face.getPos().x, -face.getPos().y, face.getPos().z);
	}
	glEnd();
}

void tunage::Mesh::render(glm::mat4 pos, Material* mat) {
	if (mat != nullptr) {
		mat->render();
	}
	glLoadMatrixf(glm::value_ptr(pos));

	glBegin(GL_TRIANGLES);
	for (auto &face : faces) {
		glNormal3f(face.getNorm().x, face.getNorm().y, face.getNorm().z);
		glTexCoord2f(face.getUV().x, face.getUV().y);
		glVertex3f(face.getPos().x, face.getPos().y, face.getPos().z);
	}
	glEnd();
}

void tunage::Mesh::addVertex(Vertex vertex) {
	faces.push_back(vertex);
}

void tunage::Mesh::setMaterial(Material* material) {
	this->material = material;
}

tunage::Material* tunage::Mesh::getMaterial() const {
	return material;
}

bool tunage::Mesh::isTransparent() {
	return material->isTransparent();
}

