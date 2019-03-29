#include <GL/glew.h>
#include <tuna-ge.h>
#include "Mesh.h"
#include "../program/Program.h"

using namespace tunage;

void Mesh::render(){
	// TODO: Remove
}

//	Render method using a material and render matrix passed as parameters
void Mesh::render(glm::mat4 pos, Material* mat) {
	//	Render material
	if (mat != nullptr) {
		mat->render();
	}

	Program::getCurrent()->setMatrix(TunaGE::getMvLoc(), pos);

	glm::mat3 normal_matrix = glm::inverseTranspose(glm::mat3(pos));
	Program::getCurrent()->setMatrix(TunaGE::getNormMatLoc(), normal_matrix);

	glBindVertexArray(vaoId);

	glDrawElements(GL_TRIANGLES,            // primitive type
	numFaces * 3,                      // # of indices
	GL_UNSIGNED_INT,         // data type
	(void*) nullptr);               // offset to indices

	glBindVertexArray(0);
}

void Mesh::init(float* positionArr,
				float* texcoordArr,
				float* normalArr,
				unsigned int* facesArr,
				int numVertices,
				int numFaces) {
	this->numVertices = numVertices;
	this->numFaces = numFaces;

	glBindVertexArray(vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * (3 + 2 + 3) * sizeof(float), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * 3 * sizeof(float), positionArr);
	glBufferSubData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), numVertices * 2 * sizeof(float), texcoordArr);
	glBufferSubData(GL_ARRAY_BUFFER, numVertices * (3 + 2) * sizeof(float), numVertices * 3 * sizeof(float), normalArr);

	size_t tOffset = numVertices * 3 * sizeof(float);
	size_t nOffset = tOffset + numVertices * 2 * sizeof(float);

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, (void*)tOffset);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, (void*)nOffset);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numFaces * 3 * sizeof(unsigned int), facesArr, GL_STATIC_DRAW);

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
	glGenBuffers(2, vboId);
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &vaoId);
	glDeleteBuffers(2, vboId);
	//faces.clear();
	//if (initialized) {
	//	// Dispose buffer:
	//	glDeleteBuffers(1, &vertVboId);
	//	glDeleteBuffers(1, &faceVboId);
	//}
}

