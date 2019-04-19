

// GLEW:
#include <GL/glew.h>
// FreeGLUT:
#include <GL/freeglut.h>
// FreeImage:
#include <FreeImage.h>
// C/C++:
#include <iostream>
#include <stdio.h>

#include "Skybox.h"

using namespace tunage;

Skybox* Skybox::current = nullptr;

Skybox::Skybox(std::string cubemapNames[], unsigned int cubemapNamesSize, float cubeVertices[], unsigned int cubeVerticesSize, unsigned short cubeFaces[], unsigned int cubeFacesSize)
{

	Shader* cubeVs = new Shader();
	Shader::loadFromMemory(Shader::TYPE_VERTEX, cubeVertShader, *cubeVs);
	Shader* cubeFs = new Shader();
	Shader::loadFromMemory(Shader::TYPE_FRAGMENT, cubeFragShader, *cubeFs);

	cubePs = new Program();
	Program::build(*cubeVs, *cubeFs, *cubePs);

	cubePs->render();
	cubePs->bind(0, "in_Position");

	cubeProjLoc = cubePs->getUniformLocation("projection");
	cubeMvLoc = cubePs->getUniformLocation("modelview");

	this->cubemapNames = cubemapNames;
	this->cubemapNamesSize = cubemapNamesSize;
	this->cubeVertices = cubeVertices;
	this->cubeVerticesSize = cubeVerticesSize;
	this->cubeFaces = cubeFaces;
	this->cubeFacesSize = cubeFacesSize;

	glGenVertexArrays(1, &cubeVao);
	glBindVertexArray(cubeVao);

	// Load cube data into a VBO:
	glGenBuffers(1, &cubeVboVertices);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVboVertices);
	glBufferData(GL_ARRAY_BUFFER, cubeVerticesSize, cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &cubeVboFaces);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeVboFaces);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeFacesSize, cubeFaces, GL_STATIC_DRAW);


	// Create and bind cubemap:   
	glGenTextures(1, &cubemapId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);

	// Set params:
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Set filters:
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Load sides:
	for (int curSide = 0; curSide < 6; curSide++)
	{
		// Load texture:
		FIBITMAP *fBitmap = FreeImage_Load(FreeImage_GetFileType(cubemapNames[curSide].c_str(), 0), cubemapNames[curSide].c_str());
		if (fBitmap == nullptr)
			std::cout << "[ERROR] loading file '" << cubemapNames[curSide] << "'" << std::endl;
		int intFormat = GL_RGB;
		GLenum extFormat = GL_BGR;
		if (FreeImage_GetBPP(fBitmap) == 32)
		{
			intFormat = GL_RGBA;
			extFormat = GL_BGRA;
		}

		// Fix mirroring:
		FreeImage_FlipHorizontal(fBitmap);  // Correct mirroring from cube's inside
		FreeImage_FlipVertical(fBitmap);    // Correct JPG's upside-down

		// Send texture to OpenGL:
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + curSide, 0, intFormat, FreeImage_GetWidth(fBitmap), FreeImage_GetHeight(fBitmap), 0, extFormat, GL_UNSIGNED_BYTE, (void *)FreeImage_GetBits(fBitmap));

		// Free resources:
		FreeImage_Unload(fBitmap);
	}
}

Skybox* Skybox::getCurrent()
{
	return Skybox::current;
}

void Skybox::instantiate(std::string cubemapNames[], unsigned int cubemapNamesSize, float cubeVertices[], unsigned int cubeVerticesSize, unsigned short cubeFaces[], unsigned int cubeFacesSize)
{
	Skybox::current = new Skybox(cubemapNames, cubemapNamesSize, cubeVertices, cubeVerticesSize, cubeFaces, cubeFacesSize);
}

void Skybox::render(glm::mat4 proj, glm::mat4 modelView)
{
	cubePs->render();
	cubePs->setMatrix(cubeProjLoc, proj);
	cubePs->setMatrix(cubeMvLoc, modelView);
	glBindVertexArray(cubeVao);
	glDrawElements(GL_TRIANGLES, cubeFacesSize / sizeof(unsigned short), GL_UNSIGNED_SHORT, nullptr);
}
