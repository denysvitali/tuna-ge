#include "stdafx.h"
#include "tuna-ge.h"
#include "version.hh"
#include <iostream>
#include <sstream>
#include <stack>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/packing.hpp>

#include <GL/freeglut.h>

#include "structure/color/RGBColor.h"
#include "structure/vertex/Vertex.h"
#include "structure/mesh/Mesh.h"

#include "structure/ovoreader/OvObject.h"
#include "structure/ovoreader/OvMesh.h"
#include "structure/ovoreader/OvLight.h"

#include <FreeImage.h>


// Save Image during renderSingleFrame in a temp dir (in order to generate expected test results)
#define SAVE_IMAGE 1

// Windows related shit:
/*
 *  There is nothing remotely unsafe about fopen(), but a few people at MS seem to have lost their collective marbles
 *  over functions that take null-terminated strings as parameters. – anon
 *
 */
#ifndef _WINDOWS
#define fopen_s(pointer, path, mode) *pointer = fopen(path, mode)
#define strcpy_s(dst, size, src) strncpy(dst, src, size)
#define strncpy_s(dst, size, src, size2) strncpy(dst, src, size)
#endif

using namespace tunage;

//Set default values//
void (* TunaGE::motion_callback)( int, int ) = nullptr;
void (* TunaGE::mouse_callback)( int, int, int, int ) = nullptr;
void (* TunaGE::special_callback)( int, int, int ) = nullptr;
void (* TunaGE::keyboard_callback)( unsigned char, int, int ) = nullptr;
bool TunaGE::wireframe = false;
#ifdef DEBUG
	bool TunaGE::debug = true;
#else
	bool TunaGE::debug = false;
#endif
bool TunaGE::culling = true;
bool TunaGE::lighting = true;
bool TunaGE::reshapeAlreadyCalled = false;
bool TunaGE::framerateVisible = false;
//	Display a window? Used during Tests to avoid generating GL windows
bool TunaGE::displayWindow = true;
int TunaGE::windowId = -1;
List TunaGE::renderList = List{ "render list" };
//	Screen size
int TunaGE::screen_w = 100;
int TunaGE::screen_h = 100;

std::vector<Object*> TunaGE::allocatedObjects = std::vector<Object*>{};

bool TunaGE::glutInitAlreadyCalled = false;

void TunaGE::init() {
	std::cout << "TunaGE::init()" << std::endl;

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	TunaGE::setWindowSize(TunaGE::screen_w, TunaGE::screen_h);

	if(!glutInitAlreadyCalled) {
		// FreeGLUT can parse command-line params, in case:
		int argc = 1;
		char* argv[1] = {(char*) "Tuna"};
		glutInit(&argc, argv);
		glutInitAlreadyCalled = true;
	}


	// Set some optional flags:
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	TunaGE::windowId = glutCreateWindow("Tuna");
	TunaGE::initGlut();
}

void TunaGE::initGlut() {
	glutSetWindow(TunaGE::windowId);

	// Set callback functions:
	glutMotionFunc(motion_callback);
	glutMouseFunc(mouse_callback);
	glutDisplayFunc(TunaGE::displayCB);
	glutReshapeFunc(TunaGE::reshapeCB);
	glutSpecialFunc(special_callback);
	glutKeyboardFunc(keyboard_callback);

	// FreeGLUT default settings
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Enter the main FreeGLUT processing loop:
void TunaGE::loop() {
	glutMainLoop();
}

//	Destroys all the elements of the scene (if any) and all the elements used in the list for additional features
bool TunaGE::free() {
	renderList.clearRenderElements();
	renderList.clearCameras();

	delete TunaGE::renderList.getSceneRoot();

	for(auto o : allocatedObjects){
		if(dynamic_cast<Texture*>(o)){
			delete (Texture*) o;
		} else if(dynamic_cast<Mesh*>(o)){
			delete (Mesh*) o;
		} else if(dynamic_cast<Material*>(o)){
			delete (Material*) o;
		} else if(dynamic_cast<Light*>(o)){
			delete (Light*) o;
		} else if(dynamic_cast<Node*>(o)){
			delete (Node*) o;
		}
	}

	TunaGE::allocatedObjects.clear();
	if(TunaGE::windowId != -1){
		glutDestroyWindow(TunaGE::windowId);
		TunaGE::windowId = -1;
		glutExit();
	}

	TunaGE::glutInitAlreadyCalled = false;

	return true;
}

//	Main callback called by FreeGLUT each frame to render the scene
void TunaGE::displayCB() {
	if (!TunaGE::reshapeAlreadyCalled) {
		TunaGE::reshapeCB(TunaGE::screen_w, TunaGE::screen_h);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (TunaGE::culling) {
		glEnable(GL_CULL_FACE);
	}
	else {
		glDisable(GL_CULL_FACE);
	}
	if (TunaGE::lighting) {
		glEnable(GL_LIGHTING);
	}
	else {
		glDisable(GL_LIGHTING);
	}
	if (TunaGE::culling) {
		glEnable(GL_CULL_FACE);
	}
	else {
		glDisable(GL_CULL_FACE);
	}
	if (TunaGE::wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	TunaGE::renderList.render();

	// Keep me as last rendering item
	if (TunaGE::debug) {
		RGBColor color = RGBColor::getColor("#fafafa");

		renderString(10, 10, GLUT_BITMAP_9_BY_15, color, String{TunaGE::version()});

		std::stringstream ss;

		Camera* cam = TunaGE::getCurrentCamera();

		glm::vec3 cp = cam->getPos();
		ss << cam->getName().data() << ": " << cp[0] << ", " << cp[1] << ", " << cp[2] << "    ";
		glm::vec3 point;
		switch (cam->getMode()) {
		case LOOK_AT_POINT:
			point = cam->getLookAtPoint();
			ss << "LAP: " << point[0] << ", " << point[1] << ", " << point[2];
			break;
		case LOOK_TOWARDS_VECTOR:
			point = cam->getFront();
			ss << "LTV: " << point[0] << ", " << point[1] << ", " << point[2];
			break;
		}

		ss << " W: " << TunaGE::screen_w << "x" << TunaGE::screen_h;

		renderString(200, 10, GLUT_BITMAP_9_BY_15, color, String{ss.str().data()});
	}

	if (TunaGE::windowId != -1) {
		glutPostWindowRedisplay(windowId);
		glutSwapBuffers();
	}
}

//	Callback called by FreeGLUT one time before starting and everytime the window size changes.
//	It is responsible to adapt the projection of the sceen using the parameters stored in the current camera
void TunaGE::reshapeCB(int w, int h) {
	if (!TunaGE::reshapeAlreadyCalled) {
		TunaGE::reshapeAlreadyCalled = true;
	}

	glViewport(0, 0, w, h);
	TunaGE::screen_w = w;
	TunaGE::screen_h = h;

	if(TunaGE::getCurrentCamera() != nullptr){
		TunaGE::getCurrentCamera()->setScreenSize(screen_w, screen_h);
		TunaGE::getCurrentCamera()->loadProjectionMatrix();
	}

	if (windowId != -1) {
		glutPostWindowRedisplay(windowId);
	}

	if (displayWindow) {
		glutSwapBuffers();
	}
}

//	Setters of the user-defined callbacks called by FreeGLUT accordingly
void TunaGE::setMotionCallback(void(*motion_callback)(int, int)) {
	TunaGE::motion_callback = motion_callback;
}
void TunaGE::setMouseCallback(void(*mouse_callback)(int, int, int, int)) {
	TunaGE::mouse_callback = mouse_callback;
}
void TunaGE::setKeyboardCallback(void(*keyboard_callback)(unsigned char, int, int)) {
	TunaGE::keyboard_callback = keyboard_callback;
}
void TunaGE::setSpecialCallback(void(*special_callback)(int, int, int)) {
	TunaGE::special_callback = special_callback;
}

//	Renders a string on screen with position, color and font specified
void TunaGE::renderString(float x, float y, void* font, RGBColor& color, String string) {

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, TunaGE::screen_w, 0.0, TunaGE::screen_h);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glRasterPos2f(x, y);
	glColor3f(color.r(), color.g(), color.b());

	for (char c : std::string(string.data())) {
		glutBitmapCharacter(font, c);
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	if (TunaGE::lighting) {
		glEnable(GL_LIGHTING);
	}
}

// Forces a call on displayCallback, usable client-side
void TunaGE::redisplay() {
	if (windowId != -1) {
		glutPostWindowRedisplay(windowId);
	}
}

int TunaGE::getScreenH() {
	return TunaGE::screen_h;
}
int TunaGE::getScreenW() {
	return TunaGE::screen_w;
}

//	Returns the current camera used for rendering
Camera* TunaGE::getCurrentCamera() {
	if(renderList.getRenderCameras().size() == 0){
		return nullptr;
	}

	return TunaGE::renderList.getRenderCameras().front();
}

//	Setters for various flags
void tunage::TunaGE::enableCulling(bool enabled){
	TunaGE::culling = enabled;
}
void tunage::TunaGE::wireframeMode(bool enabled){
	TunaGE::wireframe = enabled;
}
void tunage::TunaGE::setLightning(bool enabled){
	TunaGE::lighting = enabled;
}
void TunaGE::displayFrameRate(bool enabled) {
	TunaGE::framerateVisible = enabled;
}
void TunaGE::setDisplayWindow(bool enabled) {
	TunaGE::displayWindow = enabled;
}

//	Renders a single frame, used for debugging and testing
void* TunaGE::renderSingleFrame(unsigned char*&p, int &width, int &height) {

	int old_w = TunaGE::screen_w;
	int old_h = TunaGE::screen_h;

	TunaGE::screen_w = width;
	TunaGE::screen_h = height;

	TunaGE::reshapeCB(width, height);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glReadBuffer(GL_FRONT);

	bool debug = TunaGE::debug;

	/*
	 * Temporarily change debug value to FALSE, because the commit ID shown on the
	 * bottom left corner, whilst running in DEV mode, might affect the test result
	 * (the resulting bitmap changes if the commit SHA1 changes)
	 */
	TunaGE::debug = false;

	TunaGE::displayCB();

	auto dib = FreeImage_Allocate(width, height, 24);
	int bpp = FreeImage_GetBPP(dib);
	int Bpp = bpp / 8;

	auto* seed = (GLubyte*) malloc(sizeof(GLubyte) * width * height * Bpp);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, seed);

	for (unsigned y = 0; y < height; y++) {
		BYTE* bits = FreeImage_GetScanLine(dib, y);
		for (unsigned x = 0; x < width * Bpp; x+=Bpp) {
			// BGR
			bits[x + 2] = seed[y * Bpp * width + x];
			bits[x + 1] = seed[y * Bpp * width + x + 1];
			bits[x + 0] = seed[y * Bpp * width + x + 2];
		}
	}

#ifdef SAVE_IMAGE
#ifdef _WINDOWS
	std::cout << getenv("TEMP") << std::endl;
	std::stringstream ss;
	ss << getenv("TEMP") << "/out.bmp";
	std::cout << "Output file: " << ss.str() << std::endl;
	FreeImage_Save(FIF_BMP, dib, ss.str().data());
#else
	FreeImage_Save(FIF_BMP, dib, "/tmp/out.bmp");
#endif

#endif
	std::free(seed);

	TunaGE::debug = debug;
	TunaGE::screen_w = old_w;
	TunaGE::screen_h = old_h;

	auto err = glGetError();
	if (err) {
		std::cerr << glGetString(err) << std::endl;
	}

	return dib;
}

//	Returns the current version
String TunaGE::version() {
	std::stringstream ss{};
	ss << LIB_MAJOR << "." << LIB_MINOR << "." << LIB_PATCH;
	if (!Version::GIT_SHA1.empty()) {
		ss << "-" << Version::GIT_SHA1;
	}

	std::string str = ss.str();
	return String{ss.str().data()};
}

//	Load a full scene from a specified file OvO, this scene is then returned via the Root Node
Node* TunaGE::loadOVO(const char* path) {

	// Open file: 
	FILE* dat;
	fopen_s(&dat, path, "rb");
	if (dat == nullptr) {
		std::cout << "ERROR: unable to open file '" << path << "'" << std::endl;
		return nullptr;
	}

	// Configure stream:
	std::cout.precision(2);  // 2 decimals are enough
	std::cout << std::fixed;      // Avoid scientific notation

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
			position += (unsigned int)strlen(nodeName) + 1;

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
			position += (unsigned int)strlen(targetName) + 1;
		}
								   break;


		case OvObject::Type::MATERIAL: {

			Material* mat = new Material();
			allocatedObjects.push_back(mat);

			// Material name:
			char materialName[FILENAME_MAX];
			strcpy_s(materialName, FILENAME_MAX, data + position);
			mat->setName(materialName);
			position += (unsigned int)strlen(materialName) + 1;

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
			mat->setShininess((int)((1 - sqrt(roughness)) * 128));
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
			position += (unsigned int)strlen(textureName) + 1;

			// Normal map filename, or [none] if not used:
			char normalMapName[FILENAME_MAX];
			strcpy_s(normalMapName, FILENAME_MAX, data + position);
			position += (unsigned int)strlen(normalMapName) + 1;

			// Height map filename, or [none] if not used:
			char heightMapName[FILENAME_MAX];
			strcpy_s(heightMapName, FILENAME_MAX, data + position);
			position += (unsigned int)strlen(heightMapName) + 1;

			// Roughness map filename, or [none] if not used:
			char roughnessMapName[FILENAME_MAX];
			strcpy_s(roughnessMapName, FILENAME_MAX, data + position);
			position += (unsigned int)strlen(roughnessMapName) + 1;

			// Metalness map filename, or [none] if not used:
			char metalnessMapName[FILENAME_MAX];
			strcpy_s(metalnessMapName, FILENAME_MAX, data + position);
			position += (unsigned int)strlen(metalnessMapName) + 1;

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
			position += (unsigned int)strlen(meshName) + 1;

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
			position += (unsigned int)strlen(targetName) + 1;

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
			position += (unsigned int)strlen(materialName) + 1;

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
					Vertex{ vertex.x, vertex.y, vertex.z, normal.x, normal.y, normal.z, texture.x, texture.y });
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
					position += (unsigned int)strlen(boneName) + 1;

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
			position += (unsigned int)strlen(lightName) + 1;

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
			position += (unsigned int)strlen(targetName) + 1;

			// Light subtype (see OvLight SUBTYPE enum):
			unsigned char subtype;
			memcpy(&subtype, data + position, sizeof(unsigned char));
			char subtypeName[FILENAME_MAX];
			switch ((OvLight::Subtype) subtype) {
			case OvLight::Subtype::DIRECTIONAL:
				strcpy_s(subtypeName,FILENAME_MAX, "directional");
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
			position += (unsigned int)strlen(boneName) + 1;

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
			position += (unsigned int)strlen(targetName) + 1;

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

	for(const auto &mat : mats){
		mat.second->setName(mat.first.data());
		materials.push_back(mat.second);
	}

	root->setAllMaterials(materials);
	return root;
}

void TunaGE::setWindowSize(int width, int height) {
	screen_w = width;
	screen_h = height;

	glViewport(0, 0, width, height);
	if(TunaGE::windowId != -1) {
		glutReshapeWindow(width, height);
		glutMainLoopEvent();
	} else {
		if(!glutInitAlreadyCalled) {
			return;
		} else {
			glutDestroyWindow(TunaGE::windowId);
			TunaGE::windowId = glutCreateWindow("Tuna");
			glutInitWindowSize(width, height);
			glutSetWindow(TunaGE::windowId);
		}
	}
}








