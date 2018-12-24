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
#include "structure/vertex/Vertex.h"
#include "structure/mesh/Mesh.h"

using namespace tunage;

bool TunaGE::wireframe = false;
bool TunaGE::originMarker = false;
bool TunaGE::debug = true;
bool TunaGE::culling = true;
bool TunaGE::lighting = true;

RGBColor TunaGE::color = RGBColor(0, 0, 0);
int TunaGE::windowId = -1;

Material TunaGE::material = Material{};

Camera TunaGE::camera = Camera{ "camera 1" };

Texture TunaGE::tex = Texture{ "texture 1" };

glm::mat4 TunaGE::worldRotation = glm::mat4(1.0f);
Light TunaGE::light = Light{ RGBColor{100, 255, 255}, "Light 1" };
// World Rotation in degrees, for X, Y and Z
float TunaGE::wr_x = 0.0;
float TunaGE::wr_y = 0.0;
float TunaGE::wr_z = 0.0;

// Screen size
int TunaGE::screen_w = 0;
int TunaGE::screen_h = 0;

bool firstMouse = true;
int lastX = 0;
int lastY = 0;
float yaw = -90;
float pitch = 0;

// Lights
Light TunaGE::ambient_light = Light{RGBColor{255, 255, 255}, " "};


void TunaGE::mouseCallback(int button, int state, int mouseX, int mouseY) {

	switch (state)
	{

	case GLUT_UP:
		firstMouse = true;
		break;
	default:
		break;
	}
}
void TunaGE::motionCallback(int mouseX, int mouseY){

	if (firstMouse) {
		lastX = mouseX;
		lastY = mouseY;
		firstMouse = false;
	}
	
	float xoffset = mouseX - lastX;
	float yoffset = lastY - mouseY;
	lastX = mouseX;
	lastY = mouseY;

	float sensitivity = 0.1;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera.setCameraFront(glm::normalize(front));

	glutPostWindowRedisplay(windowId);
}



TunaGE TunaGE::init() {

	camera.setCameraPos(glm::vec3(0.0f, 4.0f, 10.0f));  // Camera is at (0,2,10), in World Space
	camera.setCameraFront(glm::vec3(0.0f, 0.0f, -1.0f)); // looks in the direction
	camera.setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f)); // Head is up (set to 0,-1,0 to look upside-down)
	camera.setCameraSpeed(0.1f);
	camera.updateCamera();

	TunaGE engine{};
	std::cout << "TunaGE::init()" << std::endl;

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);

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

void TunaGE::initGlut() {
	// Set callback functions:
	/*glutDisplayFunc(displayCallback);
	glutReshapeFunc(reshapeCallback);
	glutSpecialFunc(specialCallback);
	glutKeyboardFunc(keyboardCallback);*/
	glutMotionFunc(motionCallback);
	glutMouseFunc(mouseCallback);
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
		//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
		//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
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

	Mesh mesh{ "plane" };
	Material material{};
	
	RGBColor color = RGBColor::getColor("#1a1a1a");
	material.setAmbient(color.vec());
	material.setShininess(200);
	material.setSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
	material.setDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));

	material.setTexture(&tex);

	mesh.setMaterial(material);
	glm::mat4 model = glm::mat4(1.0f);
	mesh.setMatrix(TunaGE::camera.getMatrix() * TunaGE::worldRotation * model);

	Vertex v1{ -width, 0, 0, 0, 1, 0, 0, 0 };
	Vertex v2{ width, 0, 0, 0, 1, 0, 1, 1};
	Vertex v3{ 0, 0, -width, 0, 1, 0 , 1, 0};

	Vertex v4{ 0, 0, width, 0, 1, 0 , 0, 1 };
	Vertex v5{ width, 0, 0, 0, 1, 0 , 1, 1};
	Vertex v6{ -width, 0, 0, 0, 1, 0 , 0 , 0};

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
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(width, -0.01, 0);
	glVertex3f(0, -0.01, -width);
	glVertex3f(0, -0.01, width);
	glVertex3f(-width, -0.01, 0);
	glEnd();*/
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
	glm::mat4 vp = TunaGE::camera.getMatrix() * TunaGE::worldRotation * rotationX * scale * translate;
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
	vp = TunaGE::camera.getMatrix() * TunaGE::worldRotation * rotationY * scale * translate;
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
	vp = TunaGE::camera.getMatrix() * TunaGE::worldRotation * rotationZ * scale * translate;
	glLoadMatrixf(glm::value_ptr(vp));
	drawCube(1.0);

}

void drawGrid(float size, int tesselation)
{
	// Compute starting coordinates and step size:
	float startX = -size / 2.0f;
	float startZ = size / 2.0f;
	float triangleSize = size / (float) tesselation;

	// Normal is just one, set it now:
	glNormal3f(0.0f, 1.0f, 0.0f);

	// Go over XZ and draw triangles:
	for (int curZ = 0; curZ < tesselation; curZ++)
	{
		for (int curX = 0; curX < tesselation; curX++)
		{
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
	glm::mat4 vp = TunaGE::camera.getMatrix() * TunaGE::worldRotation * Model;
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
	glEnable(GL_DEPTH_TEST);

	camera.updateCamera();

	if (TunaGE::wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (TunaGE::lighting) {
		drawLight();
	}

	drawPlane(1);

	if (TunaGE::originMarker) {
		drawOriginMarkers(1.0);
	}


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
		void *font = GLUT_BITMAP_9_BY_15;
		for (char c : s) {
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

void TunaGE::kbdCB(unsigned char c, int mouseX, int mouseY) {
	switch (c) {
		case 'q':
			TunaGE::wireframe = !TunaGE::wireframe;
			break;
		case 'x':
			TunaGE::originMarker = !TunaGE::originMarker;
			break;
		case 'e':
			TunaGE::debug = !TunaGE::debug;
			break;
		case 'w':
			camera.setCameraPos(camera.getCameraPos() + (camera.getCameraSpeed() * camera.getCameraFront()));
			break;
		case 's':
			camera.setCameraPos(camera.getCameraPos() - (camera.getCameraSpeed() * camera.getCameraFront()));
			break;
		case 'd':
			camera.setCameraPos(camera.getCameraPos() + (glm::normalize(glm::cross(camera.getCameraFront(), camera.getCameraUp()))*camera.getCameraSpeed()));
			break;
		case 'a':
			camera.setCameraPos(camera.getCameraPos() - (glm::normalize(glm::cross(camera.getCameraFront(), camera.getCameraUp()))*camera.getCameraSpeed()));
			break;
	}
}


void TunaGE::setProjectionMatrix() {
	
	camera.setFOV(45);
	camera.setScreenSize(screen_w, screen_h);
	camera.setNearPlane(0.1f);
	camera.setFarPlane(100);
	camera.loadProjectionMatrix();

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

	TunaGE::material.render();
}