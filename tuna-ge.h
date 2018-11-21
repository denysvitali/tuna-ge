#pragma once


// Generic info:

#define LIB_NAME	"TunaGE"
#define LIB_MAJOR	1
#define LIB_MINOR	0
#define LIB_PATCH	0


#ifdef _WINDOWS 	
	#ifdef TUNAGE_EXPORTS
		#define LIB_API __declspec(dllexport)
	#else
		#define LIB_API __declspec(dllimport)
  #endif
#else // Under Linux
	#define LIB_API  // Dummy declaration
#endif

#include <string>
#include <glm/glm.hpp>
#include <structure/material/Material.h>
#include "RGBColor.h"

namespace tunage {
    class LIB_API TunaGE {
    public:
        static TunaGE init();
        static void loop();
        static bool free();

        static std::string version();


        // Draw Functions
        static void drawCube(float width);
        static void drawMultiColorCube(float width);
        static void drawPlane(float width);
        static void drawOriginMarkers(float width);
        static void setColor(RGBColor color);
        static void setMaterial(Material material);
        static void enableOriginMarker();
        static void setCameraMatrix(glm::mat4 view);
        static void setWorldRotation(glm::mat4 worldRotation);

        // Rendering Methods
        static void renderString(float x, float y, void* font, const char* string);

        ~TunaGE() {}
    private:

        static int windowId;
        static void initGlut();
        static void displayCB();
        static void reshapeCB(int w, int h);
        static void specialFuncCB(int key, int mouseX, int mouseY);
        static void kbdCB(unsigned char c, int mouseX, int mouseY);
        static void computeRotationMatrix();
        static void setProjectionMatrix();
        static void set2DTextProjectionMatrix();

        static bool wireframe;
        static bool originMarker;
        static bool debug;
        static bool culling;
        static bool lighting;

        static RGBColor color;

        static Material material;

        static glm::mat4 camera;
        static glm::mat4 worldRotation;

        static float wr_x;
        static float wr_y;
        static float wr_z;

        static int screen_w;
        static int screen_h;

        TunaGE(){};
    };
}