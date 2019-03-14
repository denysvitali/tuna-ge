
#include "tuna-ge.h"
#include "version.hh"
#include <sstream>
#include <chrono>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "structure/ovoreader/OvObject.h"
#include "structure/ovoreader/OvMesh.h"
#include "structure/ovoreader/OvLight.h"

#include <FreeImage.h>


// Save Image during renderSingleFrame in a temp dir (in order to generate expected test results)
#define SAVE_IMAGE 1

using namespace tunage;

//Set default values//
void (* TunaGE::motion_callback)(int, int) = nullptr;

void (* TunaGE::mouse_callback)(Mouse::Button, Button::State, int, int) = nullptr;

void (* TunaGE::special_callback)(Keyboard::Key, int x, int y) = nullptr;

void (* TunaGE::keyboard_callback)(unsigned char, int, int) = nullptr;

void (* TunaGE::loop_callback)() = nullptr;


bool TunaGE::wireframe = false;
#ifdef DEBUG
bool TunaGE::debug = true;
#else
bool TunaGE::debug = false;
#endif

bool TunaGE::stopRendering = false;
bool TunaGE::culling = true;
bool TunaGE::lighting = true;
bool TunaGE::reshapeAlreadyCalled = false;
bool TunaGE::closeAlreadyCalled = false;
bool TunaGE::freeAlreadyCalled = true;
bool TunaGE::framerateVisible = false;

//	Display a window? Used during Tests to avoid generating GL windows
bool TunaGE::displayWindow = true;
int TunaGE::windowId = -1;
List TunaGE::renderList = List{"render list"};
//	Screen size
int TunaGE::screen_w = 100;
int TunaGE::screen_h = 100;
double TunaGE::lastFPS = -1;
int TunaGE::lastFPS_idx = 0;
double TunaGE::lastFPSArr[FPS_COUNTER_SIZE] = {};

RGBColor debugColor = RGBColor::getColor("#fafafa");
RGBColor fpsColor = RGBColor::getColor("#4CAF50");

std::vector<Object*> TunaGE::allocatedObjects = std::vector<Object*>{};
bool TunaGE::glutInitAlreadyCalled = false;

void TunaGE::init() {
	if(!TunaGE::freeAlreadyCalled){
		throw std::runtime_error("Cannot init TunaGE without first freeing it. Use TunaGE::free()");
	}

	TunaGE::freeAlreadyCalled = false;

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	TunaGE::setWindowSize(TunaGE::screen_w, TunaGE::screen_h);

	if (!glutInitAlreadyCalled) {
		// FreeGLUT can parse command-line params, in case:
		int argc = 1;
		char* argv[1] = {(char*) "Tuna"};
		glutInit(&argc, argv);
		glutInitAlreadyCalled = true;
	}


	// Set some optional flags:
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	TunaGE::windowId = glutCreateWindow("Tuna");
	TunaGE::initGlew();
	TunaGE::initGlut();
}
void TunaGE::initGlew() {
	// Init Glew (*after* the context creation):
	glewExperimental = GL_TRUE;
	glewInit();

	// OpenGL 2.1 is required:
	if (!glewIsSupported("GL_VERSION_2_1"))
	{
		std::cout << "OpenGL 2.1 not supported" << std::endl;
		exit;
	}

	// enable vertex arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void TunaGE::initGlut() {
	glutSetWindow(TunaGE::windowId);

	// Set callback functions:
	glutMotionFunc(motion_callback);
	glutMouseFunc(TunaGE::mouseCB);
	glutDisplayFunc(TunaGE::displayCB);
	glutReshapeFunc(TunaGE::reshapeCB);
	glutSpecialFunc(TunaGE::specialKeyCB);
	glutKeyboardFunc(keyboard_callback);
	glutWMCloseFunc(TunaGE::closeFunc);

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

void TunaGE::loopEvent() {
	if (loop_callback != nullptr) {
		loop_callback();
	}
}

void TunaGE::loop_inner(bool swapBuffers){
#ifdef _WINDOWS
	std::chrono::time_point<std::chrono::steady_clock> start, end;
#else
	std::chrono::time_point<std::chrono::system_clock> start, end;
#endif
	if(TunaGE::framerateVisible) {
		start = std::chrono::high_resolution_clock::now();
	}

	if(!stopRendering) {
		glutMainLoopEvent();
		if (!stopRendering) {
		TunaGE::loopEvent();

			if(swapBuffers) {
				glutPostWindowRedisplay(windowId);
				glutSwapBuffers();
			}
		}
	}

	if(TunaGE::framerateVisible){
		end = std::chrono::high_resolution_clock::now();
		lastFPS_idx++;
		lastFPS_idx = lastFPS_idx % FPS_COUNTER_SIZE;
		lastFPSArr[lastFPS_idx] = pow(10,9)/(end - start).count();

		if(lastFPS_idx == FPS_COUNTER_SIZE - 1){
			double fpsSum = 0.0;
			for(auto d : lastFPSArr){
				fpsSum += d;
			}
			lastFPS = fpsSum / FPS_COUNTER_SIZE;
		}
	}
}

// Enter the main FreeGLUT processing loop:
void TunaGE::loop() {
	lastFPS = -1;
	while (!TunaGE::stopRendering) {
		TunaGE::loop_inner(true);
	}
}

void TunaGE::closeFunc(){
	if(!closeAlreadyCalled) {
		closeAlreadyCalled = true;
		TunaGE::stopRendering = true;
	}
}

//	Destroys all the elements of the scene (if any) and all the elements used in the list for additional features
bool TunaGE::free() {
	if(!freeAlreadyCalled){
	renderList.clearRenderElements();
	renderList.clearCameras();

	delete TunaGE::renderList.getSceneRoot();

	for (auto o : allocatedObjects) {
		if (dynamic_cast<Texture*>(o)) {
			delete (Texture*) o;
		} else if (dynamic_cast<Mesh*>(o)) {
			delete (Mesh*) o;
		} else if (dynamic_cast<Material*>(o)) {
			delete (Material*) o;
		} else if (dynamic_cast<Light*>(o)) {
			delete (Light*) o;
		} else if (dynamic_cast<Node*>(o)) {
			delete (Node*) o;
		}
	}

	TunaGE::allocatedObjects.clear();

	glutCloseFunc(nullptr);
	glutWMCloseFunc(nullptr);
	glutLeaveMainLoop();

	if (TunaGE::windowId != -1) {
		glutDestroyWindow(TunaGE::windowId);
		TunaGE::windowId = -1;
	}
	glutExit();

	TunaGE::glutInitAlreadyCalled = false;
	TunaGE::freeAlreadyCalled = true;
	return true;

	} else {
		return false;
	}

}

//	Main callback called by FreeGLUT each frame to render the scene
void TunaGE::displayCB() {
	if (!TunaGE::reshapeAlreadyCalled) {
		TunaGE::reshapeCB(TunaGE::screen_w, TunaGE::screen_h);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (TunaGE::culling) {
		glEnable(GL_CULL_FACE);
	} else {
		glDisable(GL_CULL_FACE);
	}
	if (TunaGE::lighting) {
		glEnable(GL_LIGHTING);
	} else {
		glDisable(GL_LIGHTING);
	}
	if (TunaGE::wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	Node* root = TunaGE::renderList.getSceneRoot();
	TunaGE::renderList.clearRenderElements();
	TunaGE::renderList.pass(root);
	TunaGE::renderList.render();

	if(TunaGE::framerateVisible){
		if(lastFPS != -1) {
			char output[20];
			sprintf(output, "%.0f", lastFPS);
			renderString(TunaGE::screen_w - 60, TunaGE::screen_h - 30, FontType::BITMAP_HELVETICA_18, fpsColor, String{output});
		}
	}

	// Keep me as last rendering item
	if (TunaGE::debug) {
		renderString(10, 10, FontType::BITMAP_9_BY_15, debugColor, String{TunaGE::version()});

		std::stringstream ss;

		char outputStr[200];

		Camera* cam = TunaGE::getCurrentCamera();

		glm::vec3 cp = cam->getAbsolutePosition();

		sprintf(outputStr, "%s: %.2f,%.2f,%.2f   ",
				cam->getName().data(),
				cp[0],
				cp[1],
				cp[2]);

		switch (cam->getMode()) {
			case LOOK_AT_POINT:
				{
					glm::vec3 point;
					point = cam->getLookAtPoint();
					sprintf(outputStr, "%s%s%.2f,%.2f,%.2f",
							outputStr, "LAP: ", point[0], point[1], point[2]);
				}
				break;
			case LOOK_AT_NODE:
				if(cam->getLookAtNode() != nullptr){
					sprintf(outputStr, "%s%s%s",
							outputStr, "LAN: ",
							cam->getLookAtNode()->getName().data());
				} else {
					sprintf(outputStr, "%s%s",
							outputStr, "LAN: (null)");
				}
				break;
			case LOOK_TOWARDS_VECTOR:
				{
					glm::vec3 point = cam->getFront();
					sprintf(outputStr, "%s%s%.2f,%.2f,%.2f",
							outputStr, "LTV: ", point[0], point[1], point[2]);
				}
				break;
		}

		sprintf(outputStr, "%s W: %d x %d", outputStr, TunaGE::screen_w, TunaGE::screen_h);

		renderString(200, 10, FontType::BITMAP_9_BY_15, debugColor, String{outputStr});
	}
}

void TunaGE::specialKeyCB(int button, int x, int y) {
	if(keyboard_callback != nullptr){
		keyboard_callback(Keyboard::getKey(button), x, y);
	}
}

void TunaGE::mouseCB(int button, int state, int x, int y) {
	if(mouse_callback != nullptr){
		mouse_callback(Mouse::getButton(button), Button::getState(state), x, y);
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

	if (TunaGE::getCurrentCamera() != nullptr) {
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
void TunaGE::setMotionCallback(void(* motion_callback)(int, int)) {
	TunaGE::motion_callback = motion_callback;
}

void TunaGE::setMouseCallback(void(* mouse_callback)(Mouse::Button, Button::State, int, int)) {
	TunaGE::mouse_callback = mouse_callback;
}

void TunaGE::setKeyboardCallback(void(* keyboard_callback)(unsigned char, int, int)) {
	TunaGE::keyboard_callback = keyboard_callback;
}

void TunaGE::setSpecialCallback(void(* special_callback)(Keyboard::Key k, int x, int y)) {
	TunaGE::special_callback = special_callback;
}

//	Renders a string on screen with position, color and font specified
void TunaGE::renderString(float x, float y, FontType ft, RGBColor &color, String string) {

	void* font = Font::getFont(ft);

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, TunaGE::screen_w, 0.0, TunaGE::screen_h);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(color.r(), color.g(), color.b());
	glRasterPos2f(x, y);

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
	glColor3f(255, 255, 255); // Reset to White
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
	if (renderList.getRenderCameras().size() == 0) {
		return nullptr;
	}

	return TunaGE::renderList.getRenderCameras().front();
}

//	Setters for various flags
void tunage::TunaGE::enableCulling(bool enabled) {
	TunaGE::culling = enabled;
}

void tunage::TunaGE::setWireframe(bool enabled) {
	TunaGE::wireframe = enabled;
}

void tunage::TunaGE::setLightning(bool enabled) {
	TunaGE::lighting = enabled;
}

void TunaGE::setFPSCounter(bool enabled) {
	TunaGE::framerateVisible = enabled;
	if(!framerateVisible){
		TunaGE::lastFPS = -1;
		TunaGE::lastFPS_idx = 0;
	}
}

void TunaGE::setDisplayWindow(bool enabled) {
	TunaGE::displayWindow = enabled;
}

//	Renders a single frame, used for debugging and testing
void* TunaGE::renderSingleFrame(int &width, int &height) {

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

	if (TunaGE::windowId != -1) {
		glutPostWindowRedisplay(windowId);
		glutSwapBuffers();
	}

	auto dib = FreeImage_Allocate(width, height, 24);
	int bpp = FreeImage_GetBPP(dib);
	int Bpp = bpp / 8;

	auto* seed = (GLubyte*) malloc(sizeof(GLubyte) * width * height * Bpp);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, seed);

	for (unsigned y = 0; y < height; y++) {
		BYTE* bits = FreeImage_GetScanLine(dib, y);
		for (unsigned x = 0; x < width * Bpp; x += Bpp) {
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
	return OvoReader::parse(path);
}

void TunaGE::setWindowSize(int width, int height) {
	screen_w = width;
	screen_h = height;

	glViewport(0, 0, width, height);
	if (TunaGE::windowId == -1){
		if (!glutInitAlreadyCalled) {
			return;
		} else {
			glutDestroyWindow(TunaGE::windowId);
			TunaGE::windowId = glutCreateWindow("Tuna");
			glutInitWindowSize(width, height);
			glutSetWindow(TunaGE::windowId);
		}
	} else {
		glutReshapeWindow(width, height);
	}
	TunaGE::loop_inner(false);
}

void TunaGE::setLoopCallback(void (* loop_callback)()) {
	TunaGE::loop_callback = loop_callback;
}

void TunaGE::setDebug(bool enabled){
	TunaGE::debug = enabled;
}

std::vector<Object*>& TunaGE::getAllocatedObjects() {
	return allocatedObjects;
}