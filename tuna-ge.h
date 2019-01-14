#pragma once


// Generic info:

#define LIB_NAME	"TunaGE"
#define LIB_MAJOR	1
#define LIB_MINOR	0
#define LIB_PATCH	0


#include "libapi.h"

#include <string>
#include <glm/glm.hpp>

#include "structure/light/Light.h"
#include "structure/color/RGBColor.h"
#include "structure/camera/Camera.h"
#include "structure/mesh/Mesh.h"
#include "structure/list/List.h"
#include "structure/element/Element.h"
#include "structure/string/String.h"

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
		static void setMouseCallback(void (* mouse_callback)( int, int, int, int ));
		static void setKeyboardCallback(void (* keyboard_callback)( unsigned char, int, int ));
		static void setSpecialCallback(void (* special_callback)( int, int, int ));

		// Rendering Methods
        static void renderString(float x, float y, void* font, RGBColor& color, const std::string string);
		static void redisplay();

        // Get Parameters
        static int getScreenW();
        static int getScreenH();
		static Camera* getCurrentCamera();

		//Set Flags
		static void enableCulling(bool enabled);
		static void wireframeMode(bool enabled);
		static void setLightning(bool enabled);
        static void setDisplayWindow(bool enabled);		// Flag to display a window. When false, only the framebuffer is used (e.g: for tests)
		static void displayFrameRate(bool enbabled);

		//Debugging and Test methods
		static void* renderSingleFrame(unsigned char*& p, int& width, int& height);
		static String version();

		//Fields//
		static List renderList;

    private:

		TunaGE() {};

		//Methods//
        static void initGlut();
        static void displayCB();
		static void reshapeCB(int w, int h);

		//Fields//
		static int windowId;

		//Callbacks
		static void (* motion_callback)( int, int );
		static void (* mouse_callback)( int, int, int, int );
		static void (* special_callback)( int, int, int );
		static void (* keyboard_callback)( unsigned char, int, int );

		//Flags
		static bool wireframe;
		static bool debug;
        static bool culling;
        static bool lighting;
        static bool displayWindow;
		static bool reshapeAlreadyCalled;
		static bool framerateVisible;

        static int screen_w;
        static int screen_h;
      
    };
}