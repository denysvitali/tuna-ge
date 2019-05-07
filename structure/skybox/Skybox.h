#pragma once
#include "../program/Program.h"
#include <string>

namespace tunage {
	class Skybox
	{
	public:

		static Skybox* getCurrent();
		static Skybox* instantiate(std::string cubemapNames[], unsigned int cubemapNamesSize, float cubeVertices[], unsigned int cubeVerticesSize, unsigned short cubeFaces[], unsigned int cubeFacesSize);
		void render(glm::mat4 proj, glm::mat4 modelView);
		static void setCurrent(Skybox* newCurrent);
	private:

		Skybox(std::string cubemapNames[], unsigned int cubemapNamesSize, float cubeVertices[], unsigned int cubeVerticesSize, unsigned short cubeFaces[], unsigned int cubeFacesSize);
		~Skybox();
		static Skybox* current;
		Program* cubePs;
		int cubeProjLoc = -1;
		int cubeMvLoc = -1;
		unsigned short *cubeFaces;
		std::string *cubemapNames;
		float *cubeVertices;
		unsigned int cubemapId;
		unsigned int cubeVao = 0;
		unsigned int cubeVboFaces = 0;
		unsigned int cubeVboVertices = 0;
		unsigned int cubemapNamesSize;
		unsigned int cubeVerticesSize;
		unsigned int cubeFacesSize;


		const char *cubeVertShader = R"(
		   #version 440 core

		   uniform mat4 projection;
		   uniform mat4 modelview;

		   layout(location = 0) in vec3 in_Position;      

		   out vec3 texCoord;

		   void main(void)
		   {
			  texCoord = in_Position;
			  gl_Position = projection * modelview * vec4(in_Position, 1.0f);            
		   }
		)";
		const char *cubeFragShader = R"(
		   #version 440 core
   
		   in vec3 texCoord;
   
		   // Texture mapping (cubemap):
		   layout(binding = 0) uniform samplerCube cubemapSampler;

		   out vec4 fragOutput;

		   void main(void)
		   {       
			  fragOutput = texture(cubemapSampler, texCoord);
		   }
		)";
	};
}