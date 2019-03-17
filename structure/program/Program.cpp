#include <GL/glew.h>
#include "Program.h"

Program* Program::current = nullptr;

Program::Program() : glId(0) {}

Program::~Program() {
	if (glId) {
		glDeleteProgram(glId);
	}
}

GLuint Program::getId() {
	return glId;
}

Program* Program::getCurrent() {
	return Program::current;
}


void Program::setMatrix(const char* name, glm::mat4 mat) {
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr<float>(mat));
}

void Program::setMatrix(int location, glm::mat4 mat){
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

GLint Program::getUniformLocation(const char* name) {
	if (!name) return -1;
	return glGetUniformLocation(glId, name);
}

void Program::bind(GLuint location, const char* name) {
	glBindAttribLocation(glId, location, name);
}

bool Program::build(Shader &vert, Shader &frag, Program &out) {
	if (vert.getType() != Shader::TYPE_VERTEX) {
		std::cout << "[Program] ERROR: invalid vertex shader." << std::endl;
		return false;
	}
	if (frag.getType() != Shader::TYPE_FRAGMENT) {
		std::cout << " [Program] ERROR: invalid fragment shader." << std::endl;
		return false;
	}

	// Delete if already used:
	if (out.glId) {
		glDeleteProgram(out.glId);
	}

	// Create program:
	out.glId = glCreateProgram();
	if (out.glId == 0) {
		std::cout << "[Program] ERROR: unable to create pipeline." << std::endl;
		return false;
	}

	// Bind vertex shader:
	glAttachShader(out.glId, vert.getId());

	// Bind fragment shader:
	glAttachShader(out.glId, frag.getId());

	// Link program:
	glLinkProgram(out.glId);

	// Verify program:
	int status;
	char buffer[MAX_LOGSIZE];
	int length = 0;
	memset(buffer, 0, MAX_LOGSIZE);

	glGetProgramiv(out.glId, GL_LINK_STATUS, &status);
	glGetProgramInfoLog(out.glId, MAX_LOGSIZE, &length, buffer);
	if (status == false) {
		std::cout << "[Program] ERROR: " << buffer << std::endl;
		return false;
	}
	glValidateProgram(out.glId);
	glGetProgramiv(out.glId, GL_VALIDATE_STATUS, &status);
	if (status == GL_FALSE) {
		std::cout << "[Program] ERROR: Unable to validate program." << std::endl;
		return false;
	}

	// Done:
	return true;
}

bool Program::render() {
	if (glId) {
		glUseProgram(glId);
		Program::current = this;
	} else {
		std::cout << "[Program] ERROR: render() method called on invalid Program." << std::endl;
		return false;
	}
	return true;
}

void Program::setMatrix(int location, glm::mat3 mat) {
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}
