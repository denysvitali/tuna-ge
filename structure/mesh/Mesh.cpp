#include <utility>

#include "Mesh.h"
#include <GL/freeglut.h>

void tunage::Mesh::render() {
	material.render();
	glm::mat4 composedMatrix = getRenderMatrix();
	glLoadMatrixf(glm::value_ptr(composedMatrix));

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);
	for (auto &face : faces) {
		glNormal3f(face.getNorm().x, face.getNorm().y, face.getNorm().z);
		glTexCoord2f(face.getUV().x, face.getUV().y);
		glVertex3f(face.getPos().x, face.getPos().y, face.getPos().z);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void tunage::Mesh::addVertex(Vertex vertex) {
	faces.push_back(vertex);
}

void tunage::Mesh::setMaterial(Material material) {
	this->material = std::move(material);
}
