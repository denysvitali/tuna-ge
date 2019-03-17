#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

class Shader
{
public:

	const static int MAX_LOGSIZE = 4096;

	enum
	{
		TYPE_UNDEFINED = 0,
		TYPE_VERTEX,
		TYPE_FRAGMENT,
		TYPE_LAST
	};

	Shader();
	~Shader();

	int getType();
	GLuint getId();

	static bool loadFromFile(int type, const char* path, Shader& out);
	static bool loadFromMemory(int type, const char* data, Shader& out);


private:
	int type;
	GLuint glId;
};