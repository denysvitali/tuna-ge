#include "Mesh.h"
#include "../program/Program.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace tunage;

//	Render method using the mesh render matrix and material
//	(Not used, because we're using the list)
void Mesh::render() {
	render(getRenderMatrix(), material);
}

////	Render method using a material and render matrix passed as parameters
//void Mesh::render(glm::mat4 pos, Material* mat) {
//	//	Render material
//	if (mat != nullptr) {
//		mat->render();
//	}
//	glLoadMatrixf(glm::value_ptr(pos));
//	if (!initialized) {
//		initialized = true;
//		init();
//	}
//	
//	glBindBuffer(GL_ARRAY_BUFFER, vertVboId);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceVboId);
//
//	// enable vertex arrays
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_NORMAL_ARRAY);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//
//	size_t tOffset = numVertices * 3 * sizeof(float);
//	size_t nOffset = tOffset + numVertices * 2 * sizeof(float);
//	
//	// specify vertex arrays with their offsets
//	glVertexPointer(3, GL_FLOAT, 0, (void*)0);
//	glTexCoordPointer(2, GL_FLOAT, 0, (void*)tOffset);
//	glNormalPointer(GL_FLOAT, 0, (void*)nOffset);
//
//	glDrawElements(GL_TRIANGLES,            // primitive type
//		numFaces * 3 ,                      // # of indices
//		GL_UNSIGNED_INT,         // data type
//		(void*)0);               // offset to indices
//
//	// disable vertex arrays
//	glDisableClientState(GL_VERTEX_ARRAY);
//	glDisableClientState(GL_NORMAL_ARRAY);
//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//}

//	Render method using a material and render matrix passed as parameters
void Mesh::render(glm::mat4 pos, Material* mat) {
	//	Render material
	if (mat != nullptr) {
		mat->render();
	}

	glLoadMatrixf(glm::value_ptr(pos));

	glBindVertexArray(vaoId);

	glDrawElements(GL_TRIANGLES,            // primitive type
		numFaces * 3,                      // # of indices
		GL_UNSIGNED_INT,         // data type
		(void*)0);               // offset to indices

	glBindVertexArray(0);
}

void Mesh::init(float* positionArr, float* texcoordArr, float* normalArr, unsigned int* facesArr, int numVertices, int numFaces)
{
	this->numVertices = numVertices;
	this->numFaces = numFaces;

	glBindVertexArray(vaoId);

	glGenBuffers(4, vboId);
	//glGenBuffers(1, &vboId[1]);

	// Vertex coordinates buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float) * 3, positionArr, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Vertex texture coordinates buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboId[1]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float) * 2, texcoordArr, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Vertex normals buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboId[2]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float) * 3, normalArr, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numFaces * sizeof(unsigned int) * 3, facesArr, GL_STATIC_DRAW);

	//glBufferData(GL_ARRAY_BUFFER, numVertices * (3 + 2 + 3) * sizeof(float), 0, GL_STATIC_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * 3 * sizeof(float), positionArr);
	//glBufferSubData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), numVertices * 2 * sizeof(float), texcoordArr);
	//glBufferSubData(GL_ARRAY_BUFFER, numVertices * (3 + 2) * sizeof(float), numVertices * 3 * sizeof(float), normalArr);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceVboId);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, numFaces * 3 * sizeof(unsigned int), facesArr, GL_STATIC_DRAW);

	glBindVertexArray(0);
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

Mesh::Mesh() : Node{} {
	glGenVertexArrays(1, &vaoId);
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &vaoId);
	glDeleteBuffers(4, vboId);
	//faces.clear();
	//if (initialized) {
	//	// Dispose buffer:
	//	glDeleteBuffers(1, &vertVboId);
	//	glDeleteBuffers(1, &faceVboId);
	//}
}

