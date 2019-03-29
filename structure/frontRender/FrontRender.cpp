#include <GL/glew.h>
#include <GL/freeglut.h>
#include "FrontRender.h"
#include "../utils/CurrentDir.h"





using namespace tunage;

FrontRender::FrontRender(int windowsX, int windowsY) : windowsSizeX(windowsX), windowsSizeY(windowsY), fboSizeX(windowsX/2), fboSizeY(windowsY) {

	char dir[FILENAME_MAX];
	GetCurrentDir(dir, FILENAME_MAX);
	char* passVsPath = new char[FILENAME_MAX + 50];
	char* passFsPath = new char[FILENAME_MAX + 50];
	sprintf(passVsPath, "%s%s", dir, "/assets/shaders/passShader.vert");
	sprintf(passFsPath, "%s%s", dir, "/assets/shaders/passShader.frag");

	Shader* passVs = new Shader();
	Shader::loadFromFile(Shader::TYPE_VERTEX, passVsPath, *passVs);
	Shader* passFs = new Shader();
	Shader::loadFromFile(Shader::TYPE_FRAGMENT, passFsPath, *passFs);

	delete[] passVsPath;
	delete[] passFsPath;

	passPs = new Program();
	Program::build(*passVs, *passFs, *passPs);

	passPs->render();
	passPs->bind(0, "in_Position");
	passPs->bind(2, "in_TexCoord");

	passProjLoc = passPs->getUniformLocation("projection");
	passMvLoc = passPs->getUniformLocation("modelview");
	passColorLoc = passPs->getUniformLocation("color");

	// Create a 2D box for screen rendering:
	glm::vec2 *boxPlane = new glm::vec2[4];
	boxPlane[0] = glm::vec2(0.0f, 0.0f);
	boxPlane[1] = glm::vec2(fboSizeX, 0.0f);
	boxPlane[2] = glm::vec2(0.0f, fboSizeY);
	boxPlane[3] = glm::vec2(fboSizeX, fboSizeY);

	glGenVertexArrays(1, &frontVao);
	glBindVertexArray(frontVao);

	// Copy data into VBOs:
	glGenBuffers(1, &boxVertexVbo);
	glBindBuffer(GL_ARRAY_BUFFER, boxVertexVbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), boxPlane, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	delete[] boxPlane;

	glm::vec2 *texCoord = new glm::vec2[4];
	texCoord[0] = glm::vec2(0.0f, 0.0f);
	texCoord[1] = glm::vec2(1.0f, 0.0f);
	texCoord[2] = glm::vec2(0.0f, 1.0f);
	texCoord[3] = glm::vec2(1.0f, 1.0f);
	glGenBuffers(1, &boxTexCoordVbo);
	glBindBuffer(GL_ARRAY_BUFFER, boxTexCoordVbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), texCoord, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	delete[] texCoord;

	// Load FBO and its texture:
	GLint prevViewport[4];
	glGetIntegerv(GL_VIEWPORT, prevViewport);

	for (int c = 0; c < EYE_LAST; c++)
	{
		int fboSizeX = windowsSizeX;
		int fboSizeY = windowsSizeY;
		glGenTextures(1, &fboTexId[c]);
		glBindTexture(GL_TEXTURE_2D, fboTexId[c]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, fboSizeX, fboSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		fbo[c] = new Fbo();
		fbo[c]->bindTexture(0, Fbo::BIND_COLORTEXTURE, fboTexId[c]);
		fbo[c]->bindRenderBuffer(1, Fbo::BIND_DEPTHBUFFER, fboSizeX, fboSizeY);
		if (!fbo[c]->isOk())
			std::cout << "[ERROR] Invalid FBO" << std::endl;
	}
	Fbo::disable();
	glViewport(0, 0, prevViewport[2], prevViewport[3]);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void FrontRender::render(List renderList, glm::mat4 ortho) {

	// Store the current viewport size:
	GLint prevViewport[4];
	glGetIntegerv(GL_VIEWPORT, prevViewport);

	// Render to each eye: 
	for (int c = 0; c < EYE_LAST; c++) {
		fbo[c]->render();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderList.render();
	}

	// Done with the FBO, go back to rendering into the default context buffers:
	Fbo::disable();
	glViewport(0, 0, prevViewport[2], prevViewport[3]);

	////////////////
   // 2D rendering:

   // Set a matrix for the left "eye":    
	glm::mat4 f = glm::mat4(1.0f);

	// Setup the passthrough shader:
	passPs->render();
	Program::getCurrent()->setMatrix(passProjLoc, ortho);
	Program::getCurrent()->setMatrix(passMvLoc, f);
	Program::getCurrent()->setVec(passColorLoc, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	glBindVertexArray(frontVao);

	glBindBuffer(GL_ARRAY_BUFFER, boxVertexVbo);
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glDisableVertexAttribArray(1); // We don't need normals for the 2D quad

	glBindBuffer(GL_ARRAY_BUFFER, boxTexCoordVbo);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(2);

	// Bind the FBO buffer as texture and render:
	glBindTexture(GL_TEXTURE_2D, fboTexId[EYE_LEFT]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Do the same for the right "eye": 
	f = glm::translate(glm::mat4(1.0f), glm::vec3(windowsSizeX / 2, 0.0f, 0.0f));
	Program::getCurrent()->setMatrix(passMvLoc, f);
	Program::getCurrent()->setVec(passColorLoc, glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
	glBindTexture(GL_TEXTURE_2D, fboTexId[EYE_RIGHT]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}