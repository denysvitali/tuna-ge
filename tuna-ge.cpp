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

#include <FreeImage.h>

using namespace tunage;

void (* TunaGE::motion_callback)( int, int ) = nullptr;
void (* TunaGE::mouse_callback)( int, int, int, int ) = nullptr;
void (* TunaGE::special_callback)( int, int, int ) = nullptr;
void (* TunaGE::keyboard_callback)( unsigned char, int, int ) = nullptr;

bool TunaGE::wireframe = false;
bool TunaGE::originMarker = false;
bool TunaGE::debug = true;
bool TunaGE::culling = true;
bool TunaGE::lighting = true;
bool TunaGE::reshapeAlreadyCalled = false;

RGBColor TunaGE::color = RGBColor(0, 0, 0);
int TunaGE::windowId = -1;

Material TunaGE::material = Material{};

//Camera TunaGE::getCurrentCamera() = Camera{"default getCurrentCamera()"};

List TunaGE::renderList = List{ "render list" };

Texture TunaGE::tex = Texture{ "texture 1" };

glm::mat4 TunaGE::worldRotation = glm::mat4(1.0f);
Light TunaGE::light = Light{ RGBColor{100, 255, 255}, "Light 1" };
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

	int old_w = TunaGE::screen_w;
	int old_h = TunaGE::screen_h;

	TunaGE::screen_w = width;
	TunaGE::screen_h = height;

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glReadBuffer(GL_FRONT);

	bool debug = TunaGE::debug;

	TunaGE::debug = false;

	TunaGE::displayCB();

	GLubyte seed[width * height * 3];
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, &seed);

	FIBITMAP* dib = FreeImage_Allocate(width, height, 24);
	int bytespp = FreeImage_GetLine(dib) / FreeImage_GetWidth(dib);
	for (unsigned y = 0; y < FreeImage_GetHeight(dib); y++) {
		BYTE* bits = FreeImage_GetScanLine(dib, y);
		for (unsigned x = 0; x < FreeImage_GetWidth(dib); x++) {
			bits[x*3 + 2] = seed[y * 3 * width + x * 3];
			bits[x*3 + 1] = seed[y * 3 * width + x * 3 + 1];
			bits[x*3 + 0] = seed[y * 3 * width + x * 3 + 2];
		}
	}

	FreeImage_Save(FIF_JPEG, dib, "/tmp/out.bmp");

	TunaGE::debug = debug;
	TunaGE::screen_w = old_w;
	TunaGE::screen_h = old_h;

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
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    }

    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    mesh.setMatrix(TunaGE::getCurrentCamera()->getMatrix() * TunaGE::worldRotation * model);

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
    glm::mat4 vp = TunaGE::getCurrentCamera()->getMatrix() * TunaGE::worldRotation * rotationX * scale * translate;
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
    vp = TunaGE::getCurrentCamera()->getMatrix() * TunaGE::worldRotation * rotationY * scale * translate;
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
    vp = TunaGE::getCurrentCamera()->getMatrix() * TunaGE::worldRotation * rotationZ * scale * translate;
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
    glm::mat4 vp = TunaGE::getCurrentCamera()->getMatrix() * TunaGE::worldRotation * Model;
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

	if(!TunaGE::reshapeAlreadyCalled){
		TunaGE::reshapeCB(TunaGE::screen_w, TunaGE::screen_h);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (TunaGE::wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}


	TunaGE::renderList.render();

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
        glm::vec3 cp = TunaGE::getCurrentCamera()->getCameraPos();
        glm::vec3 cf = TunaGE::getCurrentCamera()->getCameraFront();
        ss << TunaGE::getCurrentCamera()->getName() << ": " << cp[0] << ", " << cp[1] << ", " << cp[2] << "    ";
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
	if(!TunaGE::reshapeAlreadyCalled){
		TunaGE::reshapeAlreadyCalled = true;
	}

	glViewport(0, 0, w, h);
	TunaGE::screen_w = w;
	TunaGE::screen_h = h;
	setProjectionMatrix();
	glutPostWindowRedisplay(windowId);
	glutSwapBuffers();
}

void TunaGE::setProjectionMatrix() {

    TunaGE::getCurrentCamera()->setFOV(45);
    TunaGE::getCurrentCamera()->setScreenSize(screen_w, screen_h);
    TunaGE::getCurrentCamera()->setNearPlane(0.1f);
    TunaGE::getCurrentCamera()->setFarPlane(500);
    TunaGE::getCurrentCamera()->loadProjectionMatrix();

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

Camera* TunaGE::getCurrentCamera() {
    return TunaGE::renderList.getRenderCameras().front();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////OVO READER IMPLEMENTATION////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////


// Stripped-down redefinition of OvObject (just for the chunk IDs):
class OvObject {
public:
    enum class Type : int  ///< Type of entities
    {
        // Foundation types:
                OBJECT = 0,
        NODE,
        OBJECT2D,
        OBJECT3D,
        LIST,

        // Derived classes:
                BUFFER,
        SHADER,
        TEXTURE,
        FILTER,
        MATERIAL,
        FBO,
        QUAD,
        BOX,
        SKYBOX,
        FONT,
        CAMERA,
        LIGHT,
        BONE,
        MESH,       // Keep them...
        SKINNED, // ...consecutive
        INSTANCED,
        PIPELINE,
        EMITTER,

        // Animation type
                ANIM,

        // Physics related:
                PHYSICS,

        // Terminator:
                LAST,
    };
};

// Stripped-down redefinition of OvMesh (just for the subtypes):
class OvMesh {
public:
    enum class Subtype : int ///< Kind of mesh
    {
        // Foundation types:
                DEFAULT = 0,
        NORMALMAPPED,
        TESSELLATED,

        // Terminator:
                LAST,
    };
};

// Stripped-down redefinition of OvLight (just for the subtypes):
class OvLight {
public:
    enum class Subtype : int ///< Kind of light
    {
        // Foundation types:
                OMNI = 0,
        DIRECTIONAL,
        SPOT,

        // Terminator:
                LAST,
    };
};

using namespace std;

Node* TunaGE::loadOVO(const char* path) {

    // Open file:
    FILE* dat = fopen(path, "rb");
    if (dat == nullptr) {
        cout << "ERROR: unable to open file '" << path << "'" << endl;
        return nullptr;
    }

    // Configure stream:
    cout.precision(2);  // 2 decimals are enough
    cout << fixed;      // Avoid scientific notation

    map<string, Material*> mats;
    Node* root = nullptr;
    stack<Node*> nodeStack;
    map<Node*, int> nodeChildrenCount;
    unsigned int lightCount = 0;

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
            cout << "ERROR: unable to read from file '" << path << "'" << endl;
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
                strcpy(nodeName, data + position);
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
                strcpy(targetName, data + position);
                position += (unsigned int) strlen(targetName) + 1;
            }
                break;


            case OvObject::Type::MATERIAL: {

                Material* mat = new Material();

                // Material name:
                char materialName[FILENAME_MAX];
                strcpy(materialName, data + position);
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
                mat->setShininess((int) ((1 - sqrt(roughness)) * 128));
                position += sizeof(float);

                // Metalness factor:
                float metalness;
                memcpy(&metalness, data + position, sizeof(float));
                position += sizeof(float);

                // Transparency factor:
                float alpha;
                memcpy(&alpha, data + position, sizeof(float));
                position += sizeof(float);

                // Albedo texture filename, or [none] if not used:
                char textureName[FILENAME_MAX];
                strcpy(textureName, data + position);

                Texture *texture = new Texture(textureName);
#if _WINDOWS
                stringstream ss;
                ss << "../tuna-ge/assets/textures/" << textureName;
                texture->loadFromFile(ss.str());
#else
                stringstream ss;
                ss << "../../tuna-ge/assets/textures/" << textureName;
                texture->loadFromFile(ss.str());
#endif
                mat->setTexture(texture);

                position += (unsigned int) strlen(textureName) + 1;

                // Normal map filename, or [none] if not used:
                char normalMapName[FILENAME_MAX];
                strcpy(normalMapName, data + position);
                position += (unsigned int) strlen(normalMapName) + 1;

                // Height map filename, or [none] if not used:
                char heightMapName[FILENAME_MAX];
                strcpy(heightMapName, data + position);
                position += (unsigned int) strlen(heightMapName) + 1;

                // Roughness map filename, or [none] if not used:
                char roughnessMapName[FILENAME_MAX];
                strcpy(roughnessMapName, data + position);
                position += (unsigned int) strlen(roughnessMapName) + 1;

                // Metalness map filename, or [none] if not used:
                char metalnessMapName[FILENAME_MAX];
                strcpy(metalnessMapName, data + position);
                position += (unsigned int) strlen(metalnessMapName) + 1;

                mats[mat->getName()] = mat;
            }
                break;


            case OvObject::Type::MESH:
            case OvObject::Type::SKINNED: {

                Mesh* mesh = new Mesh();

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
                strcpy(meshName, data + position);
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
                strcpy(targetName, data + position);
                position += (unsigned int) strlen(targetName) + 1;

                // Mesh subtype (see OvMesh SUBTYPE enum):
                unsigned char subtype;
                memcpy(&subtype, data + position, sizeof(unsigned char));
                char subtypeName[FILENAME_MAX];
                switch ((OvMesh::Subtype) subtype) {
                    case OvMesh::Subtype::DEFAULT:
                        strcpy(subtypeName, "standard");
                        break;
                    case OvMesh::Subtype::NORMALMAPPED:
                        strcpy(subtypeName, "normal-mapped");
                        break;
                    case OvMesh::Subtype::TESSELLATED:
                        strcpy(subtypeName, "tessellated");
                        break;
                    default:
                        strcpy(subtypeName, "UNDEFINED");
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
                strcpy(materialName, data + position);
                mesh->setMaterial(*mats.find(string(materialName))->second);
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

                vector<Vertex> v;

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
                        strcpy(boneName, data + position);
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
                strcpy(lightName, data + position);
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
                strcpy(targetName, data + position);
                position += (unsigned int) strlen(targetName) + 1;

                // Light subtype (see OvLight SUBTYPE enum):
                unsigned char subtype;
                memcpy(&subtype, data + position, sizeof(unsigned char));
                char subtypeName[FILENAME_MAX];
                switch ((OvLight::Subtype) subtype) {
                    case OvLight::Subtype::DIRECTIONAL:
                        strcpy(subtypeName, "directional");
                        light->setType(0);
                        break;
                    case OvLight::Subtype::OMNI:
                        strcpy(subtypeName, "omni");
                        light->setType(1);
                        break;
                    case OvLight::Subtype::SPOT:
                        strcpy(subtypeName, "spot");
                        light->setType(2);
                        break;
                    default:
                        strcpy(subtypeName, "UNDEFINED");
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
                light->setRadius(radius);
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
                // TODO: this shit too
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
                strcpy(boneName, data + position);
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
                strcpy(targetName, data + position);
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
                cout << "UNKNOWN]" << endl;
                cout << "ERROR: corrupted or bad data in file " << path << endl;
                fclose(dat);
                delete[] data;
                return nullptr;
        }

        // Release chunk memory:
        delete[] data;
    }

    // Done:
    fclose(dat);
    return root;
}
