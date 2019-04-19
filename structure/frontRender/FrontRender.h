#pragma once

//#include "../../tuna-ge.h"
#include "../fbo/Fbo.h"
#include "../list/List.h"
#include "../ovr/ovr.h"
#include "../skybox/Skybox.h"

// Enums:
enum Eye
{
	EYE_LEFT = 0,
	EYE_RIGHT = 1,

	// Terminator:
	EYE_LAST,
};

namespace tunage {
	class FrontRender {
	public:

		FrontRender(int windowsX, int windowsY, OvVR* ovr);
		~FrontRender();

		void render(List renderList, glm::mat4 ortho);

	private:
		//OpenVR
		OvVR* ovVr;
		// FBO:      
		Fbo *fbo[EYE_LAST] = { nullptr, nullptr };
		// Textures:
		unsigned int fboTexId[EYE_LAST] = { 0, 0 };

		// Vertex buffers:
		unsigned int boxVertexVbo = 0;
		unsigned int boxTexCoordVbo = 0;
		unsigned int frontVao = 0;

		Program* passPs;

		// Window size:
		int windowsSizeX;
		int windowsSizeY;
		int fboSizeX;
		int fboSizeY;

		int passProjLoc;
		int passMvLoc;
		int passColorLoc;
	};
}