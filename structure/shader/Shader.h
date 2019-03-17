#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/freeglut.h>
#include <iostream>

class Shader
{
public:

	const static int MAX_LOGSIZE = 4096;

	enum ShaderType
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

	static bool loadFromFile(ShaderType type, const char* path, Shader& out);
	static bool loadFromMemory(ShaderType type, const char* data, Shader& out);
	int getParamLocation(const char * param);
	void setVec3(int location, glm::vec3 vec);
	void setFloat(int location, float f);
	void setMatrix3(int location, glm::mat3 mat);
	void setMatrix4(int location, glm::mat4 mat);


private:
	int type;
	GLuint glId;
};