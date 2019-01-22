#pragma once


// Generic info:

#define LIB_NAME	"TunaGE"
#define LIB_MAJOR	2
#define LIB_MINOR	1
#define LIB_PATCH	0

#define FPS_COUNTER_SIZE 10


#include "libapi.h"

#include <string>
#include <glm/glm.hpp>

#if DEBUG
// GL Vector => String casting
	#include <glm/ext.hpp>
#endif

#include "structure/light/Light.h"
#include "structure/color/RGBColor.h"
#include "structure/camera/Camera.h"
#include "structure/mesh/Mesh.h"
#include "structure/list/List.h"
#include "structure/element/Element.h"
#include "structure/string/String.h"
#include "structure/font/Font.h"
#include "structure/mouse/Mouse.h"
#include "structure/button/Button.h"
#include "structure/keyboard/Keyboard.h"
#include "structure/ovoreader/OvoReader.h"

namespace tunage {
    class LIB_API TunaGE {
    public:
		
		//Methods//
		//Main methods
		static void init();
		static Node* loadOVO(const char* path);
		static void loop();
		static bool free();

		// Setting Callbacks
		static void setMotionCallback(void (* motion_callback)( int, int ));
		static void setMouseCallback(void (* mouse_callback)( Mouse::Button, Button::State, int, int ));
		static void setKeyboardCallback(void (* keyboard_callback)(unsigned char c, int x, int y));
		static void setSpecialCallback(void (* special_callback)( Keyboard::Key k, int x, int y));
		static void setLoopCallback(void (* loop_callback)());

		// Window Settings
		static void setWindowSize(int width, int height);

		// Rendering Methods
        static void renderString(float x, float y, FontType ft, RGBColor& color, String string);
		static void redisplay();

        // Get Parameters
        static int getScreenW();
        static int getScreenH();
		static Camera* getCurrentCamera();

		//Set Flags
		static void enableCulling(bool enabled);
		static void setWireframe(bool enabled);
		static void setLightning(bool enabled);
        static void setDisplayWindow(bool enabled);		// Flag to display a window. When false, only the framebuffer is used (e.g: for tests)
		static void setFPSCounter(bool enbabled);
		static void setDebug(bool enabled);

		//Debugging and Test methods
		static void* renderSingleFrame(int& width, int& height);
		static String version();

		//Fields//
		static List renderList;

    private:

		TunaGE() {};

		//Methods//
        static void initGlut();
        static void displayCB();
		static void reshapeCB(int w, int h);
		static void mouseCB(int button, int state, int x, int y);
		static void specialKeyCB(int button, int x, int y);
		static void loopEvent();
		static void loop_inner(bool swapBuffers);
		static void closeFunc();

		static std::vector<Object*>& getAllocatedObjects();

		friend Node* OvoReader::parse(const char* path);

		//Fields//
		static int windowId;

		//Callbacks
		static void (* motion_callback)( int, int );
		static void (* mouse_callback)( Mouse::Button, Button::State, int, int );
		static void (* special_callback)( Keyboard::Key k, int x, int y);
		static void (* keyboard_callback)( unsigned char, int, int );
		static void (* loop_callback)();

		//Flags
		static bool wireframe;
		static bool debug;
        static bool culling;
        static bool lighting;
        static bool displayWindow;
		static bool reshapeAlreadyCalled;
		static bool framerateVisible;
		static bool closeAlreadyCalled;

        static int screen_w;
        static int screen_h;

        static double lastFPS;
        static double lastFPSArr[FPS_COUNTER_SIZE];
        static int lastFPS_idx;

        static bool stopRendering;

        static bool glutInitAlreadyCalled;
        static bool freeAlreadyCalled;

        static std::vector<Object*> allocatedObjects;

    };
}