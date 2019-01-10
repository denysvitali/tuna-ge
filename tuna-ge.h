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

namespace tunage {
    class LIB_API TunaGE {
    public:
		static TunaGE init();
        static void loop();
		static void renderSingleFrame(unsigned char*& p, int& width, int& height);
		static bool free();

		static List renderList;

		static std::string version();

		static Camera* getCurrentCamera();

		// Setting Callbacks
		static void setMotionCallback(void (* motion_callback)( int, int ));
		static void setMouseCallback(void (* mouse_callback)( int, int, int, int ));
		static void setKeyboardCallback(void (* keyboard_callback)( unsigned char, int, int ));

        static void enableOriginMarker();

		// Rendering Methods
		static void render(glm::mat4 camera, List& list);
        static void renderString(float x, float y, void* font, const char* string);
		static void redisplay();


        // Get Parameters
        static int getScreenW();
        static int getScreenH();

        static Node* loadOVO(const char* path);

        ~TunaGE() {}
    private:

        static int windowId;
        static void initGlut();
        static void displayCB();
		static void reshapeCB(int w, int h);
		static void specialFuncCB(int key, int mouseX, int mouseY);
        //static void kbdCB(unsigned char c, int mouseX, int mouseY);
		//static void mouseCallback(int button, int state, int mouseX, int mouseY);
		//static void motionCallback(int mouseX, int mouseY);
		static void computeRotationMatrix();
        static void setProjectionMatrix();

		//callbacks
		static void (* motion_callback)( int, int );
		static void (* mouse_callback)( int, int, int, int );
		static void (* special_callback)( int, int, int );
		static void (* keyboard_callback)( unsigned char, int, int );

		static bool wireframe;
		static bool debug;
        static bool culling;
        static bool lighting;

        static int screen_w;
        static int screen_h;
        static bool reshapeAlreadyCalled;

        TunaGE(){};
    };
}