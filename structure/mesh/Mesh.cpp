#include "Mesh.h"
#include <GL/glew.h>
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
	if (!initialized) {
		initialized = true;
		init();
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, vboVer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboInd);

	// enable vertex arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	size_t tOffset = numVertices * 3 * sizeof(float);
	size_t nOffset = tOffset + numVertices * 2 * sizeof(float);
	
	// specify vertex arrays with their offsets
	glVertexPointer(3, GL_FLOAT, 0, (void*)0);
	glTexCoordPointer(2, GL_FLOAT, 0, (void*)tOffset);
	glNormalPointer(GL_FLOAT, 0, (void*)nOffset);

	glDrawElements(GL_TRIANGLES,            // primitive type
		numFaces,                      // # of indices
		GL_UNSIGNED_INT,         // data type
		(void*)0);               // offset to indices

// disable vertex arrays
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	/*
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);
	glNormalPointer(GL_FLOAT, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	glDrawElements(GL_TRIANGLES, numFaces, GL_UNSIGNED_INT, 0);

	/*
	// Use it for rendering:
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexPointer(3, GL_FLOAT, 0, nullptr);
	//glDrawArrays(GL_TRIANGLES, 0, numVertices);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glTexCoordPointer(2, GL_FLOAT, 0, nullptr);
	//glDrawArrays(GL_TRIANGLES, 0, numVertices);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glNormalPointer(GL_FLOAT, 0, nullptr);

	glDrawArrays(GL_TRIANGLES, 0, numVertices);

	
	/*
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glBegin(GL_TRIANGLES);
	for (auto &face : faces) {
		glNormal3f(face.getNorm().x, face.getNorm().y, face.getNorm().z);
		glTexCoord2f(face.getUV().x, face.getUV().y);
		glVertex3f(face.getPos().x, face.getPos().y, face.getPos().z);
	}
	glEnd();*/
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
	glBufferSubData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), numVertices * sizeof(glm::vec2), texcoordArr);
	glBufferSubData(GL_ARRAY_BUFFER, numVertices * (3 + 2) * sizeof(float), numVertices * sizeof(glm::vec3), normalArr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboInd);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numFaces * sizeof(unsigned int), facesArr, GL_STATIC_DRAW);
	/*
	// Copy the vertex data from system to video memory:
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec3),
		positionArr, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	// Copy the texture data from system to video memory:
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec2),
		texcoordArr, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	// Copy the normals data from system to video memory:
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec3),
		normalArr, GL_STATIC_DRAW);
		*/
}

Mesh::~Mesh() {
	faces.clear();
	if (initialized) {
		// Dispose buffer:
		glDeleteBuffers(1, &vboVer);
		glDeleteBuffers(1, &vboInd);
	}
}

