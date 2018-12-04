#include "stdafx.h"
#include "tuna-ge.h"
#include "version.hh"
#include <iostream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// FreeGLUT:
#include <GL/freeglut.h>

#include "structure/color/RGBColor.h"
#include "structure/point/Point.h"

using namespace tunage;

bool TunaGE::wireframe = false;
bool TunaGE::originMarker = false;
bool TunaGE::debug = true;
bool TunaGE::culling = true;
bool TunaGE::lighting = true;

RGBColor TunaGE::color = RGBColor(0, 0, 0);
int TunaGE::windowId = -1;

Material TunaGE::material = Material{};

glm::mat4 TunaGE::camera = glm::lookAt(
		glm::vec3(0, 3, 3), // Camera is at (0,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
);

glm::mat4 TunaGE::worldRotation = glm::mat4(1.0f);

// World Rotation in degrees, for X, Y and Z
float TunaGE::wr_x = 0.0;
float TunaGE::wr_y = 0.0;
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
	glutInitWindowPosition(100, 100);

	// FreeGLUT can parse command-line params, in case:
	int *argc = new int{0};
	glutInit(argc, nullptr);

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

void TunaGE::initGlut() {
	// The OpenGL context is now initialized...
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);


	// Set callback functions:
	/*glutDisplayFunc(displayCallback);
	glutReshapeFunc(reshapeCallback);
	glutSpecialFunc(specialCallback);
	glutKeyboardFunc(keyboardCallback);*/
	glutDisplayFunc(TunaGE::displayCB);
	glutReshapeFunc(TunaGE::reshapeCB);
	glutSpecialFunc(TunaGE::specialFuncCB);
	glutKeyboardFunc(TunaGE::kbdCB);
	//glutDisplayFunc(displayCallback);

	if (TunaGE::culling) {
		glEnable(GL_CULL_FACE);
	} else {
		glDisable(GL_CULL_FACE);
	}

	if (TunaGE::lighting) {
		glEnable(GL_LIGHTING);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	}

	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
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
	// Front Face (facing the camera)
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

void TunaGE::drawMultiColorCube(float width) {
	glBegin(GL_TRIANGLE_STRIP);
	setColor(RGBColor::getColor("#FF5722")); // Deep Orange
	glVertex3f(width / 2, -width / 2, width / 2);
	glVertex3f(-width / 2, -width / 2, width / 2);
	glVertex3f(width / 2, width / 2, width / 2);
	glVertex3f(-width / 2, width / 2, width / 2);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	setColor(RGBColor::getColor("#4CAF50")); // Green
	glVertex3f(width / 2, -width / 2, -width / 2);
	glVertex3f(-width / 2, -width / 2, -width / 2);
	glVertex3f(width / 2, width / 2, -width / 2);
	glVertex3f(-width / 2, width / 2, -width / 2);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	setColor(RGBColor::getColor("#2196F3")); // Blue
	glVertex3f(width / 2, -width / 2, -width / 2);
	glVertex3f(width / 2, width / 2, -width / 2);
	glVertex3f(width / 2, -width / 2, width / 2);
	glVertex3f(width / 2, width / 2, width / 2);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	setColor(RGBColor::getColor("#FFEB3B")); // Yellow
	glVertex3f(-width / 2, -width / 2, -width / 2);
	glVertex3f(-width / 2, width / 2, -width / 2);
	glVertex3f(-width / 2, -width / 2, width / 2);
	glVertex3f(-width / 2, width / 2, width / 2);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	setColor(RGBColor::getColor("#607D8B")); // Blue Gray
	glVertex3f(-width / 2, -width / 2, -width / 2);
	glVertex3f(width / 2, -width / 2, -width / 2);
	glVertex3f(-width / 2, -width / 2, width / 2);
	glVertex3f(width / 2, -width / 2, width / 2);
	glEnd();


	glBegin(GL_TRIANGLE_STRIP);
	setColor(RGBColor::getColor("#F44336")); // Red
	glVertex3f(-width / 2, width / 2, -width / 2);
	glVertex3f(width / 2, width / 2, -width / 2);
	glVertex3f(-width / 2, width / 2, width / 2);
	glVertex3f(width / 2, width / 2, width / 2);
	glEnd();
}

void TunaGE::drawPlane(float width) {
	// Draws a plane on XZ
	glMatrixMode(GL_MODELVIEW);
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 vp = TunaGE::camera * TunaGE::worldRotation * Model;
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
	setMaterial(m);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(width, -0.01, 0);
	glVertex3f(0, -0.01, -width);
	glVertex3f(0, -0.01, width);
	glVertex3f(-width, -0.01, 0);
	glEnd();
}

void TunaGE::renderString(float x, float y, void *font, const char *string) {
	glRasterPos2d(x, y);
	glutBitmapString(font, reinterpret_cast<const unsigned char *>(string));
}

Material generateOriginMarkerMaterial(RGBColor color){
	Material m{};
	m.setAmbient(color.vec());
	m.setSpecular(glm::vec3(0.6f, 0.6f, 0.6f));
	m.setShininess(120);
	m.setDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));
	return m;
}

void drawArrow(float x, float y, float z, float rx, float ry, float rz){
	double arrow_length = 0.3;
	glPushMatrix();
	glRotatef(rx, 1, 0, 0);
	glRotatef(ry, 0, 1, 0);
	glRotatef(rz, 0, 0, 1);
	glTranslated(x, y, z);
	glutSolidCylinder(0.02, arrow_length, 50, 50);
	glTranslated(0, 0, arrow_length);
	glutSolidCone(0.05, arrow_length*0.85, 50, 50);
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
	glm::mat4 vp = TunaGE::camera * TunaGE::worldRotation * rotationX * scale * translate;
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
	vp = TunaGE::camera * TunaGE::worldRotation * rotationY * scale * translate;
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
	vp = TunaGE::camera * TunaGE::worldRotation * rotationZ * scale * translate;
	glLoadMatrixf(glm::value_ptr(vp));
	drawCube(1.0);

}

void TunaGE::drawLight() {
	Light l{RGBColor{255, 255, 255}, "Light 0"};

	// Position
	l.setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

	l.setLight(0);
	l.setMatrix(glm::mat4(1.0f));
	l.setIntensity(0.8);
	l.setLightAmbient(glm::vec3(1.0f, 1.0f, 1.0f));
	l.setLightDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));
	l.setLightSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
	l.enable();
	l.render();
}

void TunaGE::displayCB() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

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
		void *font = GLUT_BITMAP_9_BY_15;
		for (char c : s) {
			glutBitmapCharacter(font, c);
		}

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glEnable(GL_TEXTURE_2D);
		if (TunaGE::lighting) {
			glEnable(GL_LIGHTING);
		}
	}


	if (TunaGE::wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	drawPlane(1);

	if (TunaGE::originMarker) {
		/*Material mat;
		mat.setAmbient(glm::vec3(1.0f,0.2f,0.2f));
		mat.setSpecular(glm::vec3(0.8f,0.8f,0.8f));
		mat.setDiffuse(glm::vec3(0.2f,0.2f,0.2f));
		mat.setShininess(80);
		setMaterial(mat);
		drawArrow(0, 0, 0, 0, 0, 0);*/
		drawOriginMarkers(1.0);
	}

	if(TunaGE::lighting){
		drawLight();
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

void TunaGE::kbdCB(unsigned char c, int mouseX, int mouseY) {
	switch (c) {
		case 'w':
			TunaGE::wireframe = !TunaGE::wireframe;
			break;
		case 'x':
			TunaGE::originMarker = !TunaGE::originMarker;
			break;
		case 'd':
			TunaGE::debug = !TunaGE::debug;
			break;
	}
}

void TunaGE::setProjectionMatrix() {
	glMatrixMode(GL_PROJECTION);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) TunaGE::screen_w / (float) TunaGE::screen_h,
											0.1f, 100.0f);
	glLoadMatrixf(glm::value_ptr(Projection));
	glMatrixMode(GL_MODELVIEW);
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

void TunaGE::setWorldRotation(glm::mat4 worldRotation) {
	TunaGE::worldRotation = worldRotation;
}

void TunaGE::setMaterial(Material material) {
	TunaGE::material = material;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,
				 glm::value_ptr(material.getAmbient()));
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,
				 glm::value_ptr(material.getDiffuse()));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
				 glm::value_ptr(material.getSpecular()));
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,
				material.getShininess());
}