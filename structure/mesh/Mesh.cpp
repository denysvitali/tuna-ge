#include "Mesh.h"

// FreeGLUT:
#include <GL/freeglut.h>

void tunage::Mesh::render()
{
	material.render();
	glm::mat4 composedMatrix = getRenderMatrix();
	glLoadMatrixf(glm::value_ptr(composedMatrix));

	glBegin(GL_TRIANGLES);
	for (auto i = faces.begin(); i != faces.end(); ++i) {
		
		glNormal3f(i->getNorm().x, i->getNorm().y, i->getNorm().z);
		glTexCoord2f(i->getUV().x, i->getUV().y);
		glVertex3f(i->getPos().x, i->getPos().y, i->getPos().z);
	}
	glEnd();
}

void tunage::Mesh::addVertex(Vertex vertex)
{
	faces.push_back(vertex);
}

void tunage::Mesh::setMaterial(Material material)
{
	this->material = material;
}
