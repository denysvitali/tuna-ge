#include "Mesh.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace tunage;

//  DEPRECATED
//	Render method using the mesh render matrix and material
//	(Not used, because we're using the list)
void Mesh::render() {
	//	Render material
	if (material != nullptr) {
		material->render();
	}
	glm::mat4 composedMatrix = getRenderMatrix();
	glLoadMatrixf(glm::value_ptr(composedMatrix));

	if (!initialized) {
		initialized = true;
		init();
	}

	glBindBuffer(GL_ARRAY_BUFFER, vboVer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboInd);

	size_t tOffset = numVertices * 3 * sizeof(float);
	size_t nOffset = tOffset + numVertices * 2 * sizeof(float);

	// specify vertex arrays with their offsets
	glVertexPointer(3, GL_FLOAT, 0, (void*)0);
	glTexCoordPointer(2, GL_FLOAT, 0, (void*)tOffset);
	glNormalPointer(GL_FLOAT, 0, (void*)nOffset);

	glDrawElements(GL_TRIANGLES,            // primitive type
		numFaces * 3,                      // # of indices
		GL_UNSIGNED_INT,         // data type
		(void*)0);               // offset to indices

}

//	Render method using a material and render matrix passed as parameters
void Mesh::render(glm::mat4 pos, Material* mat) {
	//	Render material
	if (mat != nullptr) {
		mat->render();
	}
	glLoadMatrixf(glm::value_ptr(pos));
	if (!initialized) {
		initialized = true;
		init();
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, vboVer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboInd);

	size_t tOffset = numVertices * 3 * sizeof(float);
	size_t nOffset = tOffset + numVertices * 2 * sizeof(float);
	
	// specify vertex arrays with their offsets
	glVertexPointer(3, GL_FLOAT, 0, (void*)0);
	glTexCoordPointer(2, GL_FLOAT, 0, (void*)tOffset);
	glNormalPointer(GL_FLOAT, 0, (void*)nOffset);

	glDrawElements(GL_TRIANGLES,            // primitive type
		numFaces * 3 ,                      // # of indices
		GL_UNSIGNED_INT,         // data type
		(void*)0);               // offset to indices

}

void Mesh::addVertex(Vertex& vertex) {
	faces.push_back(std::move(vertex));
}

void Mesh::addVertexes(float* positionArr, float* texcoordArr, float* normalArr, unsigned int* facesArr, int numVertices, int numFaces)
{
	this->positionArr = positionArr;
	this->texcoordArr = texcoordArr;
	this->normalArr = normalArr;
	this->facesArr = facesArr;
	this->numVertices = numVertices;
	this->numFaces = numFaces;
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

void Mesh::init()
{
	// Generate a vertex buffer and bind it:
	glGenBuffers(1, &vboVer);
	glGenBuffers(1, &vboInd);

	glBindBuffer(GL_ARRAY_BUFFER, vboVer);
	glBufferData(GL_ARRAY_BUFFER, numVertices * (3 + 2 + 3) * sizeof(float), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * 3 * sizeof(float), positionArr);
	glBufferSubData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), numVertices * 2 * sizeof(float), texcoordArr);
	glBufferSubData(GL_ARRAY_BUFFER, numVertices * (3 + 2) * sizeof(float), numVertices * 3 * sizeof(float), normalArr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboInd);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numFaces * 3 * sizeof(unsigned int), facesArr, GL_STATIC_DRAW);
}

Mesh::~Mesh() {
	faces.clear();
	if (initialized) {
		// Dispose buffer:
		glDeleteBuffers(1, &vboVer);
		glDeleteBuffers(1, &vboInd);
	}
}

