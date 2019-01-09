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
        static bool free();

        static std::string version();

		static List renderList;

		static Camera* getCurrentCamera();

		// Setting Callbacks
		static void setMotionCallback(void (* motion_callback)( int, int ));
		static void setMouseCallback(void (* mouse_callback)( int, int, int, int ));
		static void setKeyboardCallback(void (* keyboard_callback)( unsigned char, int, int ));

        // Draw Functions
        static void drawCube(float width);
        static void drawPlane(float width);
        static void drawOriginMarkers(float width);
        static void setColor(RGBColor color);
        static void enableOriginMarker();
        static void setWorldRotation(glm::mat4 worldRotation);
		static void setMaterial(Material material);

        // Rendering Methods
		static void render(glm::mat4 camera, List& list);
        static void renderString(float x, float y, void* font, const char* string);

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
        static void set2DTextProjectionMatrix();
        static void drawLight();

        // Auxiliary Functions
		static void c_PA(float w);
		static void c_PB(float w);
		static void c_PC(float w);
		static void c_PD(float w);
		static void c_PE(float w);
		static void c_PF(float w);
		static void c_PG(float w);
		static void c_PH(float w);

		//callbacks
		static void (* motion_callback)( int, int );
		static void (* mouse_callback)( int, int, int, int );
		static void (* special_callback)( int, int, int );
		static void (* keyboard_callback)( unsigned char, int, int );

		static bool wireframe;
        static bool originMarker;
        static bool debug;
        static bool culling;
        static bool lighting;

        static RGBColor color;
        static Material material;
		static Light light;
		static Texture tex;

        // Lights
        static Light ambient_light;
        static glm::mat4 worldRotation;

        static float wr_x;
        static float wr_y;
        static float wr_z;

        static int screen_w;
        static int screen_h;

        TunaGE(){};
    };
}