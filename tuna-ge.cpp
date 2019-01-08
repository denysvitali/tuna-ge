#include "stdafx.h"
#include "tuna-ge.h"
#include "version.hh"
#include <iostream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/freeglut.h>

#include "structure/color/RGBColor.h"
#include "structure/vertex/Vertex.h"
#include "structure/mesh/Mesh.h"

#include <FreeImage.h>

using namespace tunage;

void (* TunaGE::motion_callback)(int, int) = nullptr;

void (* TunaGE::mouse_callback)(int, int, int, int) = nullptr;

void (* TunaGE::special_callback)(int, int, int) = nullptr;

void (* TunaGE::keyboard_callback)(unsigned char, int, int) = nullptr;

bool TunaGE::wireframe = false;
bool TunaGE::originMarker = false;
bool TunaGE::debug = true;
bool TunaGE::culling = true;
bool TunaGE::lighting = true;

RGBColor TunaGE::color = RGBColor(0, 0, 0);
int TunaGE::windowId = -1;

Material TunaGE::material = Material{};

//Camera TunaGE::getCurrentCamera() = Camera{"default getCurrentCamera()"};

List TunaGE::renderList = List{"render list"};

Texture TunaGE::tex = Texture{"texture 1"};

glm::mat4 TunaGE::worldRotation = glm::mat4(1.0f);
Light TunaGE::light = Light{RGBColor{100, 255, 255}, "Light 1"};
// World Rotation in degrees, for X, Y and Z
float TunaGE::wr_x = 0.0;
float TunaGE::wr_y = 0.0;
// FreeGLUT:

float TunaGE::wr_z = 0.0;
// Screen size
int TunaGE::screen_w = 0;

int TunaGE::screen_h = 0;

// Lights
Light TunaGE::ambient_light = Light{RGBColor{255, 255, 255}, " "};

TunaGE TunaGE::init() {


	TunaGE engine{};
	std::cout << "TunaGE::init()" << std::endl;

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	TunaGE::screen_h = 100;
	TunaGE::screen_w = 100;
	glutInitWindowPosition(TunaGE::screen_w, TunaGE::screen_h);


	// FreeGLUT can parse command-line params, in case:
	int argc = 1;
	char* argv[1] = {(char*) "Tuna"};
	glutInit(&argc, argv);


	// Set some optional flags:
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	TunaGE::wireframe = false;
	TunaGE::color = RGBColor(0, 0, 0);
	TunaGE::windowId = glutCreateWindow("Tuna");
	TunaGE::initGlut();


	return engine;
}

void TunaGE::loop() {
	// Enter the main FreeGLUT processing loop:
	glutMainLoop();
}

int TunaGE::getScreenH() {
	return TunaGE::screen_h;
}

int TunaGE::getScreenW() {
	return TunaGE::screen_w;
}

void TunaGE::renderSingleFrame(unsigned char*&p, int &width, int &height) {
	TunaGE::displayCB();

	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);

	glReadBuffer(GL_BACK);
	GLubyte seed[width * height * 3];
	glReadPixels(0, height, width, height, GL_RGB, GL_UNSIGNED_BYTE, &seed);

	FIBITMAP* dib = FreeImage_Allocate(width, height, 24);
	int bytespp = FreeImage_GetLine(dib) / FreeImage_GetWidth(dib);
	for (unsigned y = 0; y < FreeImage_GetHeight(dib); y++) {
		BYTE* bits = FreeImage_GetScanLine(dib, y);
		for (unsigned x = 0; x < FreeImage_GetWidth(dib); x++) {
			bits[x*3 + 2] = seed[y * 3 * width + x * 3];
			bits[x*3 + 1] = seed[y * 3 * width + x * 3 + 1];
			bits[x*3 + 0] = seed[y * 3 * width + x * 3 + 2];
			//printf("%d %d %d\n", bits[FI_RGBA_RED], bits[FI_RGBA_GREEN], bits[FI_RGBA_BLUE]);
			//bits[FI_RGBA_ALPHA] = 255;
		}
	}

	FreeImage_Save(FIF_JPEG, dib, "/tmp/out.jpg");


	auto err = glGetError();
	if (err) {
		std::cerr << glGetString(err) << std::endl;
	}
}

bool TunaGE::free() {
	return true;
}

void TunaGE::initGlut() {
	// Set callback functions:
	glutMotionFunc(motion_callback);
	glutMouseFunc(mouse_callback);
	glutDisplayFunc(TunaGE::displayCB);
	glutReshapeFunc(TunaGE::reshapeCB);
	glutSpecialFunc(TunaGE::specialFuncCB);
	glutKeyboardFunc(keyboard_callback);

	if (TunaGE::culling) {
		glEnable(GL_CULL_FACE);
	} else {
		glDisable(GL_CULL_FACE);
	}

	if (TunaGE::lighting) {
		glEnable(GL_LIGHTING);
		//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
		//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	}

	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
}

void TunaGE::enableOriginMarker() {
	TunaGE::originMarker = true;
}

std::string TunaGE::version() {
	std::stringstream ss{};
	ss << LIB_MAJOR << "." << LIB_MINOR << "." << LIB_PATCH;
	if (!Version::GIT_SHA1.empty()) {
		ss << "-" << Version::GIT_SHA1;
	}
	return ss.str();
}

void TunaGE::setColor(RGBColor color) {
	TunaGE::color = color;
	glColor3f(color.r(), color.g(), color.b());
}

void TunaGE::c_PA(float width) {
	glVertex3f(-width / 2, width / 2, width / 2);
}

void TunaGE::c_PB(float width) {
	glVertex3f(width / 2, width / 2, width / 2);
}

void TunaGE::c_PC(float width) {
	glVertex3f(-width / 2, -width / 2, width / 2);
}

void TunaGE::c_PD(float width) {
	glVertex3f(width / 2, -width / 2, width / 2);
}

void TunaGE::c_PE(float width) {
	glVertex3f(width / 2, -width / 2, -width / 2);
}

void TunaGE::c_PF(float width) {
	glVertex3f(-width / 2, -width / 2, -width / 2);
}

void TunaGE::c_PG(float width) {
	glVertex3f(width / 2, width / 2, -width / 2);
}

void TunaGE::c_PH(float width) {
	glVertex3f(-width / 2, width / 2, -width / 2);
}

void TunaGE::drawCube(float width) {
	glBegin(GL_TRIANGLE_STRIP);
	// Front Face (facing the getCurrentCamera())
	glNormal3f(0.0f, 0.0f, 1.0f);
	TunaGE::c_PB(width);
	TunaGE::c_PA(width);
	TunaGE::c_PD(width);
	TunaGE::c_PC(width);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	// Back Face
	glNormal3f(0.0f, 0.0f, -1.0f);
	TunaGE::c_PH(width);
	TunaGE::c_PG(width);
	TunaGE::c_PF(width);
	TunaGE::c_PE(width);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	// Right Face
	glNormal3f(1.0f, 0.0f, 0.0f);
	TunaGE::c_PG(width);
	TunaGE::c_PB(width);
	TunaGE::c_PE(width);
	TunaGE::c_PD(width);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	// Left Face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	TunaGE::c_PA(width);
	TunaGE::c_PH(width);
	TunaGE::c_PC(width);
	TunaGE::c_PF(width);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	// Bottom Face
	TunaGE::c_PD(width);
	TunaGE::c_PC(width);
	TunaGE::c_PE(width);
	TunaGE::c_PF(width);
	glEnd();


	glBegin(GL_TRIANGLE_STRIP);
	// Top Face
	glNormal3f(0.0f, 1.0f, 0.0f);
	c_PB(width);
	c_PG(width);
	c_PA(width);
	c_PH(width);
	glEnd();
}

void TunaGE::drawPlane(float width) {
	// Draws a plane on XZ

	Mesh mesh{"plane"};
	Material material{};

	RGBColor color = RGBColor::getColor("#1a1a1a");
	material.setAmbient(color.vec());
	material.setShininess(200);
	material.setSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
	material.setDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));

	material.setTexture(&tex);

	mesh.setMaterial(material);
	glm::mat4 model = glm::mat4(1.0f);
	mesh.setMatrix(TunaGE::getCurrentCamera().getMatrix() * TunaGE::worldRotation * model);

	Vertex v1{-width, 0, 0, 0, 1, 0, 0, 0};
	Vertex v2{width, 0, 0, 0, 1, 0, 1, 1};
	Vertex v3{0, 0, -width, 0, 1, 0, 1, 0};

	Vertex v4{0, 0, width, 0, 1, 0, 0, 1};
	Vertex v5{width, 0, 0, 0, 1, 0, 1, 1};
	Vertex v6{-width, 0, 0, 0, 1, 0, 0, 0};

	mesh.addVertex(v1);
	mesh.addVertex(v2);
	mesh.addVertex(v3);
	mesh.addVertex(v4);
	mesh.addVertex(v5);
	mesh.addVertex(v6);
	mesh.render();

	/*
	glMatrixMode(GL_MODELVIEW);
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 vp = TunaGE::getCurrentCamera() * TunaGE::worldRotation * Model;
	glLoadMatrixf(glm::value_ptr(vp));
	RGBColor color = RGBColor::getColor("#8e44ad");
	Material m{};
	m.setAmbient(color.vec());
	m.setShininess(120);
	m.setSpecular(glm::vec3(0.3f, 0.3f, 0.3f));
	m.setDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));

	glBegin(GL_TRIANGLE_STRIP);
	setMaterial(m);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(width, 0, 0);
	glVertex3f(0, 0, -width);
	glVertex3f(0, 0, width);
	glVertex3f(-width, 0, 0);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(width, -0.01, 0);
	glVertex3f(0, -0.01, -width);
	glVertex3f(0, -0.01, width);
	glVertex3f(-width, -0.01, 0);
	glEnd();*/
}

void tunage::TunaGE::render(glm::mat4 camera, List &list) {
	list.setCameraMatrix(camera);
	list.render();
}

void TunaGE::renderString(float x, float y, void* font, const char* string) {
	glRasterPos2d(x, y);
	glutBitmapString(font, reinterpret_cast<const unsigned char*>(string));
}

Material generateOriginMarkerMaterial(RGBColor color) {
	Material m{};
	m.setAmbient(color.vec());
	m.setSpecular(glm::vec3(0.6f, 0.6f, 0.6f));
	m.setShininess(120);
	m.setDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));
	return m;
}

void drawArrow(float x, float y, float z, float rx, float ry, float rz) {
	double arrow_length = 0.3;
	glPushMatrix();
	glRotatef(rx, 1, 0, 0);
	glRotatef(ry, 0, 1, 0);
	glRotatef(rz, 0, 0, 1);
	glTranslated(x, y, z);
	glutSolidCylinder(0.02, arrow_length, 50, 50);
	glTranslated(0, 0, arrow_length);
	glutSolidCone(0.05, arrow_length * 0.85, 50, 50);
	glPopMatrix();
}

void TunaGE::drawOriginMarkers(float width) {
	// X arrow - Red
	//RGBColor redColor = RGBColor::getColor("#c0392b");
	RGBColor redColor = RGBColor::getColor("#ff0000");
	Material redMat = generateOriginMarkerMaterial(redColor);
	setMaterial(redMat);

	glMatrixMode(GL_MODELVIEW);
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1, 0.02, 0.02));
	glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 vp = TunaGE::getCurrentCamera().getMatrix() * TunaGE::worldRotation * rotationX * scale * translate;
	glLoadMatrixf(glm::value_ptr(vp));

	drawCube(1.0);


	// Y arrow - Green
	RGBColor greenColor = RGBColor::getColor("#27ae60");
	Material greenMat = generateOriginMarkerMaterial(greenColor);
	setMaterial(greenMat);

	glMatrixMode(GL_MODELVIEW);
	translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0));
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.02, 1, 0.02));
	glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	vp = TunaGE::getCurrentCamera().getMatrix() * TunaGE::worldRotation * rotationY * scale * translate;
	glLoadMatrixf(glm::value_ptr(vp));
	drawCube(1.0);

	// Z arrow - Blue
	RGBColor blueColor = RGBColor::getColor("#2980b9");
	Material blueMat = generateOriginMarkerMaterial(blueColor);
	setMaterial(blueMat);

	glMatrixMode(GL_MODELVIEW);
	translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0, 0.5f));
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.02, 0.02, 1));
	glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	vp = TunaGE::getCurrentCamera().getMatrix() * TunaGE::worldRotation * rotationZ * scale * translate;
	glLoadMatrixf(glm::value_ptr(vp));
	drawCube(1.0);

}


void drawGrid(float size, int tesselation) {
	// Compute starting coordinates and step size:
	float startX = -size / 2.0f;
	float startZ = size / 2.0f;
	float triangleSize = size / (float) tesselation;

	// Normal is just one, set it now:
	glNormal3f(0.0f, 1.0f, 0.0f);

	// Go over XZ and draw triangles:
	for (int curZ = 0; curZ < tesselation; curZ++) {
		for (int curX = 0; curX < tesselation; curX++) {
			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(startX, 0.0f, startZ);
			glVertex3f(startX + triangleSize, 0.0f, startZ);
			glVertex3f(startX, 0.0f, startZ - triangleSize);
			glVertex3f(startX + triangleSize, 0.0f, startZ - triangleSize);
			glEnd();

			startX += triangleSize;
		}
		startX = -size / 2.0f;
		startZ -= triangleSize;
	}
}

void TunaGE::drawLight() {
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 vp = TunaGE::getCurrentCamera().getMatrix() * TunaGE::worldRotation * Model;
	vp = glm::translate(vp, glm::vec3(1.0f, 1.0f, 0.0f));
	light.setMatrix(vp);
	light.setLight(1);
	light.setIntensity(1.0);
	light.setLightAmbient(glm::vec3(1.0f, 1.0f, 1.0f));
	light.setLightDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));
	light.setLightSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
	light.enable();
	light.render();
}

void TunaGE::displayCB() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (TunaGE::wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}


	TunaGE::getCurrentCamera().updateCamera();
	// TunaGE::renderList.setCameraMatrix(getCurrentCamera().getMatrix());
	TunaGE::renderList.render();
	/*
	if (TunaGE::lighting) {
		drawLight();
	}

	drawPlane(1);

	if (TunaGE::originMarker) {
		drawOriginMarkers(1.0);
	}
	*/

	// Keep me as last rendering item
	if (TunaGE::debug) {
		setColor(RGBColor(255, 255, 0));
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D); //added this
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0.0, TunaGE::screen_w, 0.0, TunaGE::screen_h);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glRasterPos2i(10, 10);
		std::string s = TunaGE::version();
		void* font = GLUT_BITMAP_9_BY_15;
		for (char c : s) {
			glutBitmapCharacter(font, c);
		}

		glRasterPos2i(200, 10);
		std::stringstream ss;
		glm::vec3 cp = TunaGE::getCurrentCamera().getCameraPos();
		glm::vec3 cf = TunaGE::getCurrentCamera().getCameraFront();
		ss << TunaGE::getCurrentCamera().getName() << ": " << cp[0] << ", " << cp[1] << ", " << cp[2] << "    ";
		ss << "CF: " << cf[0] << "," << cf[1] << "," << cf[2];
		std::string s2 = ss.str();
		for (char c : s2) {
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

	glutPostWindowRedisplay(windowId);
	glutSwapBuffers();
}

void TunaGE::computeRotationMatrix() {
	TunaGE::worldRotation = glm::mat4(1.0f);
	TunaGE::worldRotation *= glm::rotate(glm::mat4(1.0f), glm::radians(TunaGE::wr_x), glm::vec3(1.0f, 0, 0));
	TunaGE::worldRotation *= glm::rotate(glm::mat4(1.0f), glm::radians(TunaGE::wr_y), glm::vec3(0, 1.0f, 0));
	TunaGE::worldRotation *= glm::rotate(glm::mat4(1.0f), glm::radians(TunaGE::wr_z), glm::vec3(0, 0, 1.0f));
}

void TunaGE::specialFuncCB(int key, int mouseX, int mouseY) {
	switch (key) {
		case GLUT_KEY_LEFT:
			wr_y += 1;
			TunaGE::computeRotationMatrix();
			break;
		case GLUT_KEY_RIGHT:
			wr_y -= 1;
			TunaGE::computeRotationMatrix();
			break;
		case GLUT_KEY_UP:
			wr_x += 1;
			TunaGE::computeRotationMatrix();
			break;
		case GLUT_KEY_DOWN:
			wr_x -= 1;
			TunaGE::computeRotationMatrix();
			break;
	}
}

void TunaGE::set2DTextProjectionMatrix() {
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(glm::mat4(1.0f)));
	glMatrixMode(GL_MODELVIEW);
}

void TunaGE::reshapeCB(int w, int h) {
	glViewport(0, 0, w, h);
	TunaGE::screen_w = w;
	TunaGE::screen_h = h;
	setProjectionMatrix();
	glutPostWindowRedisplay(windowId);
	glutSwapBuffers();
}

void TunaGE::setProjectionMatrix() {

	TunaGE::getCurrentCamera().setFOV(45);
	TunaGE::getCurrentCamera().setScreenSize(screen_w, screen_h);
	TunaGE::getCurrentCamera().setNearPlane(0.1f);
	TunaGE::getCurrentCamera().setFarPlane(100);
	TunaGE::getCurrentCamera().loadProjectionMatrix();

}

void TunaGE::setWorldRotation(glm::mat4 worldRotation) {
	TunaGE::worldRotation = worldRotation;
}

void TunaGE::setMaterial(Material material) {
	TunaGE::material = material;

	TunaGE::material.render();
}

void TunaGE::setMotionCallback(void (* motion_callback)(int, int)) {
	TunaGE::motion_callback = motion_callback;
}

void TunaGE::setMouseCallback(void (* mouse_callback)(int, int, int, int)) {
	TunaGE::mouse_callback = mouse_callback;
}

void TunaGE::setKeyboardCallback(void (* keyboard_callback)(unsigned char, int, int)) {
	TunaGE::keyboard_callback = keyboard_callback;
}

Camera &TunaGE::getCurrentCamera() {
	return TunaGE::renderList.getRenderCameras().front();
}
