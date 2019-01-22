#include "OvoReader.h"

#include <stack>
#include <string>
#include <sstream>
#include <glm/gtc/packing.hpp>

#include "../../tuna-ge.h"
#include "../utils/WindowsFunctions.h"
#include "../ovoreader/OvObject.h"
#include "../ovoreader/OvMesh.h"
#include "../ovoreader/OvLight.h"

using namespace tunage;

Node* OvoReader::parse(const char* path) {
	FILE* dat;
	fopen_s(&dat, path, "rb");
	if (dat == nullptr) {
		std::cerr << "ERROR: unable to open file '" << path << "'" << std::endl;
		return nullptr;
	}

	std::vector<Object*>& allocatedObjects = TunaGE::getAllocatedObjects();

	// Configure stream:
	std::cout.precision(2);  // 2 decimals are enough
	std::cout << std::fixed; // Avoid scientific notation

	std::map<std::string, Material*> mats;
	Node* root = nullptr;
	std::stack<Node*> nodeStack;
	std::map<Node*, int> nodeChildrenCount;
	unsigned int lightCount = 0;

	// Create default materials

	Material* nullMaterial = new Material{};
	allocatedObjects.push_back(nullMaterial);
	nullMaterial->setName("[none]");
	mats["[none]"] = nullMaterial;

	/////////////////
	// Parse chunks:
	unsigned int chunkId, chunkSize;
	while (true) {
		fread(&chunkId, sizeof(unsigned int), 1, dat);
		if (feof(dat))
			break;
		fread(&chunkSize, sizeof(unsigned int), 1, dat);


		// Load whole chunk into memory:
		char* data = new char[chunkSize];
		if (fread(data, sizeof(char), chunkSize, dat) != chunkSize) {
			std::cout << "ERROR: unable to read from file '" << path << "'" << std::endl;
			fclose(dat);
			delete[] data;
			return nullptr;
		}

		unsigned int position = 0;
		switch ((OvObject::Type) chunkId) {
			case OvObject::Type::OBJECT: {

				// OVO revision number:
				unsigned int versionId;
				memcpy(&versionId, data + position, sizeof(unsigned int));
				position += sizeof(unsigned int);
			}
				break;


			case OvObject::Type::NODE: {
				Node* node = new Node();
				allocatedObjects.push_back(node);

				if (root == nullptr) root = node;

				while (!nodeStack.empty()) {
					Node* parent = nodeStack.top();
					if (nodeChildrenCount[parent] > 0) {
						parent->link(node);
						nodeChildrenCount[parent]--;
						break;
					}
					if (nodeChildrenCount[parent] <= 0) {
						nodeStack.pop();
					}
				}

				// Node name:
				char nodeName[FILENAME_MAX];
				strcpy_s(nodeName, FILENAME_MAX, data + position);
				node->setName(nodeName);
				position += (unsigned int) strlen(nodeName) + 1;

				// Node matrix:
				glm::mat4 matrix;
				memcpy(&matrix, data + position, sizeof(glm::mat4));
				node->setMatrix(matrix);
				position += sizeof(glm::mat4);

				// Nr. of children nodes:
				unsigned int children;
				memcpy(&children, data + position, sizeof(unsigned int));
				if (children) {
					nodeStack.push(node);
					nodeChildrenCount[node] = children;
				}
				position += sizeof(unsigned int);

				// Optional target node, [none] if not used:
				char targetName[FILENAME_MAX];
				strcpy_s(targetName, FILENAME_MAX, data + position);
				position += (unsigned int) strlen(targetName) + 1;
			}
				break;


			case OvObject::Type::MATERIAL: {

				Material* mat = new Material();
				allocatedObjects.push_back(mat);

				// Material name:
				char materialName[FILENAME_MAX];
				strcpy_s(materialName, FILENAME_MAX, data + position);
				mat->setName(materialName);
				position += (unsigned int) strlen(materialName) + 1;

				// Material term colors, starting with emissive:
				glm::vec3 emission, albedo;
				memcpy(&emission, data + position, sizeof(glm::vec3));
				mat->setEmission(emission);
				position += sizeof(glm::vec3);

				// Albedo:
				memcpy(&albedo, data + position, sizeof(glm::vec3));
				mat->setAmbient(albedo * 0.2f);
				mat->setSpecular(albedo * 0.4f);
				mat->setDiffuse(albedo * 0.6f);
				position += sizeof(glm::vec3);

				// Roughness factor:
				float roughness;
				memcpy(&roughness, data + position, sizeof(float));
				mat->setShininess((1 - sqrt(roughness)) * 128);
				position += sizeof(float);

				// Metalness factor:
				float metalness;
				memcpy(&metalness, data + position, sizeof(float));
				position += sizeof(float);

				// Transparency factor:
				float alpha;
				memcpy(&alpha, data + position, sizeof(float));
				mat->setAlpha(alpha);
				position += sizeof(float);

				// Albedo texture filename, or [none] if not used:
				char textureName[FILENAME_MAX];
				strcpy_s(textureName, FILENAME_MAX, data + position);
				if (std::string(textureName) != "[none]") {
					Texture* texture = new Texture(textureName);
					allocatedObjects.push_back(texture);
#if _WINDOWS
					std::stringstream ss;
					ss << "../../tuna-ge/assets/textures/" << textureName;
					texture->loadFromFile(ss.str().data());
#else
					std::stringstream ss;
					ss << "../../tuna-ge/assets/textures/" << textureName;
					texture->loadFromFile(ss.str().data());
#endif
					mat->setTexture(texture);
				}
				position += (unsigned int) strlen(textureName) + 1;

				// Normal map filename, or [none] if not used:
				char normalMapName[FILENAME_MAX];
				strcpy_s(normalMapName, FILENAME_MAX, data + position);
				position += (unsigned int) strlen(normalMapName) + 1;

				// Height map filename, or [none] if not used:
				char heightMapName[FILENAME_MAX];
				strcpy_s(heightMapName, FILENAME_MAX, data + position);
				position += (unsigned int) strlen(heightMapName) + 1;

				// Roughness map filename, or [none] if not used:
				char roughnessMapName[FILENAME_MAX];
				strcpy_s(roughnessMapName, FILENAME_MAX, data + position);
				position += (unsigned int) strlen(roughnessMapName) + 1;

				// Metalness map filename, or [none] if not used:
				char metalnessMapName[FILENAME_MAX];
				strcpy_s(metalnessMapName, FILENAME_MAX, data + position);
				position += (unsigned int) strlen(metalnessMapName) + 1;

				mats[std::string{mat->getName().data()}] = mat;
			}
				break;
			case OvObject::Type::MESH:
			case OvObject::Type::SKINNED: {

				Mesh* mesh = new Mesh();
				allocatedObjects.push_back(mesh);

				while (!nodeStack.empty()) {
					Node* parent = nodeStack.top();
					if (nodeChildrenCount[parent] > 0) {
						parent->link(mesh);
						nodeChildrenCount[parent]--;
						break;
					}
					if (nodeChildrenCount[parent] <= 0) {
						nodeStack.pop();
					}
				}

				bool isSkinned = false;
				if ((OvObject::Type) chunkId == OvObject::Type::SKINNED) {
					isSkinned = true;
				}

				// Mesh name:
				char meshName[FILENAME_MAX];
				strcpy_s(meshName, FILENAME_MAX, data + position);
				mesh->setName(meshName);
				position += (unsigned int) strlen(meshName) + 1;

				// Mesh matrix:
				glm::mat4 matrix;
				memcpy(&matrix, data + position, sizeof(glm::mat4));
				mesh->setMatrix(matrix);
				position += sizeof(glm::mat4);

				// Mesh nr. of children nodes:
				unsigned int children;
				memcpy(&children, data + position, sizeof(unsigned int));
				if (children) {
					nodeStack.push(mesh);
					nodeChildrenCount[mesh] = children;
				}
				position += sizeof(unsigned int);

				// Optional target node, or [none] if not used:
				char targetName[FILENAME_MAX];
				strcpy_s(targetName, FILENAME_MAX, data + position);
				position += (unsigned int) strlen(targetName) + 1;

				// Mesh subtype (see OvMesh SUBTYPE enum):
				unsigned char subtype;
				memcpy(&subtype, data + position, sizeof(unsigned char));
				char subtypeName[FILENAME_MAX];
				switch ((OvMesh::Subtype) subtype) {
					case OvMesh::Subtype::DEFAULT:
						strcpy_s(subtypeName, FILENAME_MAX, "standard");
						break;
					case OvMesh::Subtype::NORMALMAPPED:
						strcpy_s(subtypeName, FILENAME_MAX, "normal-mapped");
						break;
					case OvMesh::Subtype::TESSELLATED:
						strcpy_s(subtypeName, FILENAME_MAX, "tessellated");
						break;
					default:
						strcpy_s(subtypeName, FILENAME_MAX, "UNDEFINED");
				}
				position += sizeof(unsigned char);

				// Nr. of vertices:
				unsigned int vertices, faces;
				memcpy(&vertices, data + position, sizeof(unsigned int));
				position += sizeof(unsigned int);

				// ...and faces:
				memcpy(&faces, data + position, sizeof(unsigned int));
				position += sizeof(unsigned int);

				// Material name, or [none] if not used:
				char materialName[FILENAME_MAX];
				strcpy_s(materialName, FILENAME_MAX, data + position);
				std::string matName = std::string(materialName);
				mesh->setMaterial(mats.find(matName)->second);
				position += (unsigned int) strlen(materialName) + 1;

				// Mesh bounding sphere radius:
				float radius;
				memcpy(&radius, data + position, sizeof(float));
				position += sizeof(float);

				// Mesh bounding box minimum corner:
				glm::vec3 bBoxMin;
				memcpy(&bBoxMin, data + position, sizeof(glm::vec3));
				position += sizeof(glm::vec3);

				// Mesh bounding box maximum corner:
				glm::vec3 bBoxMax;
				memcpy(&bBoxMax, data + position, sizeof(glm::vec3));
				position += sizeof(glm::vec3);

				// Optional physics properties:
				unsigned char hasPhysics;
				memcpy(&hasPhysics, data + position, sizeof(unsigned char));
				position += sizeof(unsigned char);
				if (hasPhysics) {
					/**
					 * Mesh physics properties.
					 */
					struct PhysProps {
						// Pay attention to 16 byte alignement (use padding):
						unsigned char type;
						unsigned char contCollisionDetection;
						unsigned char collideWithRBodies;
						unsigned char hullType;

						// Vector data:
						glm::vec3 massCenter;

						// Mesh properties:
						float mass;
						float staticFriction;
						float dynamicFriction;
						float bounciness;
						float linearDamping;
						float angularDamping;
						void* physObj;
					};

					PhysProps mp;
					memcpy(&mp, data + position, sizeof(PhysProps));
					position += sizeof(PhysProps);
				}

				std::vector<Vertex> v;

				// Interleaved and compressed vertex/normal/UV/tangent data:
				for (unsigned int c = 0; c < vertices; c++) {

					// Vertex coords:
					glm::vec3 vertex;
					memcpy(&vertex, data + position, sizeof(glm::vec3));
					position += sizeof(glm::vec3);

					// Vertex normal:
					unsigned int normalData;
					memcpy(&normalData, data + position, sizeof(unsigned int));
					position += sizeof(unsigned int);

					// Texture coordinates:
					unsigned short textureData[2];
					memcpy(textureData, data + position, sizeof(unsigned short) * 2);
					position += sizeof(unsigned short) * 2;

					// Tangent vector:
					unsigned int tangentData;
					memcpy(&tangentData, data + position, sizeof(unsigned int));
					position += sizeof(unsigned int);

					glm::vec3 normal = glm::unpackSnorm3x10_1x2(normalData);
					glm::vec2 texture;
					texture.x = glm::unpackHalf1x16(textureData[0]);
					texture.y = glm::unpackHalf1x16(textureData[1]);
					v.emplace_back(
							Vertex{vertex.x, vertex.y, vertex.z, normal.x, normal.y, normal.z, texture.x, texture.y});
				}

				// Faces:
				for (unsigned int c = 0; c < faces; c++) {
					// Face indexes:
					unsigned int face[3];
					memcpy(face, data + position, sizeof(unsigned int) * 3);
					for (auto i : face) mesh->addVertex(v[i]);
					position += sizeof(unsigned int) * 3;
				}

				// Extra information for skinned meshes:
				if (isSkinned) {
					// Initial mesh pose matrix:
					glm::mat4 poseMatrix;
					memcpy(&poseMatrix, data + position, sizeof(glm::mat4));
					position += sizeof(glm::vec4);

					// Bone list:
					unsigned int nrOfBones;
					memcpy(&nrOfBones, data + position, sizeof(unsigned int));
					position += sizeof(unsigned int);

					for (unsigned int c = 0; c < nrOfBones; c++) {
						// Bone name:
						char boneName[FILENAME_MAX];
						strcpy_s(boneName, FILENAME_MAX, data + position);
						position += (unsigned int) strlen(boneName) + 1;

						// Initial bone pose matrix (already inverted):
						glm::mat4 boneMatrix;
						memcpy(&boneMatrix, data + position, sizeof(glm::mat4));
						position += sizeof(glm::mat4);
					}

					// Per vertex bone weights and indexes:
					for (unsigned int c = 0; c < vertices; c++) {

						// Bone indexes:
						unsigned int boneIndex[4];
						memcpy(boneIndex, data + position, sizeof(unsigned int) * 4);
						position += sizeof(unsigned int) * 4;

						// Bone weights:
						unsigned short boneWeightData[4];
						memcpy(boneWeightData, data + position, sizeof(unsigned short) * 4);
						position += sizeof(unsigned short) * 4;
					}
				}
			}
				break;

			case OvObject::Type::LIGHT: {

				Light* light = new Light();
				allocatedObjects.push_back(light);

				while (!nodeStack.empty()) {
					Node* parent = nodeStack.top();
					if (nodeChildrenCount[parent] > 0) {
						parent->link(light);
						nodeChildrenCount[parent]--;
						break;
					}
					if (nodeChildrenCount[parent] <= 0) {
						nodeStack.pop();
					}
				}

				// Light name:
				char lightName[FILENAME_MAX];
				strcpy_s(lightName, FILENAME_MAX, data + position);
				light->setName(lightName);
				position += (unsigned int) strlen(lightName) + 1;

				// Light matrix:
				glm::mat4 matrix;
				memcpy(&matrix, data + position, sizeof(glm::mat4));
				light->setMatrix(matrix);
				position += sizeof(glm::mat4);

				// Nr. of children nodes:
				unsigned int children;
				memcpy(&children, data + position, sizeof(unsigned int));
				if (children) {
					nodeStack.push(light);
					nodeChildrenCount[light] = children;
				}
				position += sizeof(unsigned int);

				// Optional target node name, or [none] if not used:
				char targetName[FILENAME_MAX];
				strcpy_s(targetName, FILENAME_MAX, data + position);
				position += (unsigned int) strlen(targetName) + 1;

				// Light subtype (see OvLight SUBTYPE enum):
				unsigned char subtype;
				memcpy(&subtype, data + position, sizeof(unsigned char));
				char subtypeName[FILENAME_MAX];
				switch ((OvLight::Subtype) subtype) {
					case OvLight::Subtype::DIRECTIONAL:
						strcpy_s(subtypeName, FILENAME_MAX, "directional");
						light->setType(0);
						break;
					case OvLight::Subtype::OMNI:
						strcpy_s(subtypeName, FILENAME_MAX, "omni");
						light->setType(1);
						break;
					case OvLight::Subtype::SPOT:
						strcpy_s(subtypeName, FILENAME_MAX, "spot");
						light->setType(2);
						break;
					default:
						strcpy_s(subtypeName, FILENAME_MAX, "UNDEFINED");
						light->setType(0);
				}
				position += sizeof(unsigned char);

				// Light color:
				glm::vec3 color;
				memcpy(&color, data + position, sizeof(glm::vec3));
				light->setLightAmbient(color);
				light->setLightDiffuse(color);
				light->setLightSpecular(color);
				position += sizeof(glm::vec3);

				// Influence radius:
				float radius;
				memcpy(&radius, data + position, sizeof(float));
				//  light->setRadius(radius);
				//TODO: to implement
				light->setInfluenceRadius(radius);
				position += sizeof(float);

				// Direction:
				glm::vec3 direction;
				memcpy(&direction, data + position, sizeof(glm::vec3));
				light->setLightDirection(direction);
				position += sizeof(glm::vec3);

				// Cutoff:
				float cutoff;
				memcpy(&cutoff, data + position, sizeof(float));
				light->setLightCutoff(cutoff);
				position += sizeof(float);

				// Exponent:
				float spotExponent;
				memcpy(&spotExponent, data + position, sizeof(float));
				light->setSpotExponent(spotExponent);
				position += sizeof(float);

				// Cast shadow flag:
				unsigned char castShadows;
				memcpy(&castShadows, data + position, sizeof(unsigned char));
				position += sizeof(unsigned char);

				// Volumetric lighting flag:
				unsigned char isVolumetric;
				memcpy(&isVolumetric, data + position, sizeof(unsigned char));
				position += sizeof(unsigned char);


				light->setLight(lightCount++);
				light->setIntensity(1.0f);
				light->enable();
			}
				break;

			case OvObject::Type::BONE: {

				// Bone name:
				char boneName[FILENAME_MAX];
				strcpy_s(boneName, FILENAME_MAX, data + position);
				position += (unsigned int) strlen(boneName) + 1;

				// Bone matrix:
				glm::mat4 matrix;
				memcpy(&matrix, data + position, sizeof(glm::mat4));
				position += sizeof(glm::mat4);

				// Nr. of children nodes:
				unsigned int children;
				memcpy(&children, data + position, sizeof(unsigned int));
				position += sizeof(unsigned int);

				// Optional target node, or [none] if not used:
				char targetName[FILENAME_MAX];
				strcpy_s(targetName, FILENAME_MAX, data + position);
				position += (unsigned int) strlen(targetName) + 1;

				// Mesh bounding box minimum corner:
				glm::vec3 bBoxMin;
				memcpy(&bBoxMin, data + position, sizeof(glm::vec3));
				position += sizeof(glm::vec3);

				// Mesh bounding box maximum corner:
				glm::vec3 bBoxMax;
				memcpy(&bBoxMax, data + position, sizeof(glm::vec3));
				position += sizeof(glm::vec3);
			}
				break;


			default:
				std::cout << "UNKNOWN]" << std::endl;
				std::cout << "ERROR: corrupted or bad data in file " << path << std::endl;
				fclose(dat);
				delete[] data;
				return nullptr;
		}
		// Release chunk memory:
		delete[] data;
	}

	// Done:
	fclose(dat);

	std::vector<Material*> materials{};
	materials.reserve(mats.size());

	for (const auto &mat : mats) {
		mat.second->setName(mat.first.data());
		materials.push_back(mat.second);
	}

	return root;
}