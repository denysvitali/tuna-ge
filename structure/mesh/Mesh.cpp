#include "Mesh.h"
#include <GL/freeglut.h>

using namespace tunage;

//	Render method using the mesh render matrix and material
//	(Not used, because we're using the list)
void Mesh::render() {
	//	Render material
	if (material != nullptr) {
		material->render();
	}
	glm::mat4 composedMatrix = getRenderMatrix();
	glLoadMatrixf(glm::value_ptr(composedMatrix));

	glBegin(GL_TRIANGLES);
	for (auto &face : faces) {
		glNormal3f(face.getNorm().x, face.getNorm().y, face.getNorm().z);
		glTexCoord2f(face.getUV().x, face.getUV().y);
		glVertex3f(face.getPos().x, face.getPos().y, face.getPos().z);
	}
	glEnd();
}

//	Render method using a material and render matrix passed as parameters
void Mesh::render(glm::mat4 pos, Material* mat) {
	//	Render material
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

void Mesh::addVertex(Vertex& vertex) {
	faces.push_back(std::move(vertex));
}

void Mesh::setMaterial(Material* material) {
	this->material = material;
}

Material* Mesh::getMaterial() const {
	return material;
}

bool Mesh::isTransparent() {
	return material->isTransparent();
}

Mesh::~Mesh() {
	faces.clear();
}

