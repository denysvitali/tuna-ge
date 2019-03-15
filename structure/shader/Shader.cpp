#include "Shader.h"

Shader::Shader() : type(TYPE_UNDEFINED), glId(0) {}

Shader::~Shader() {
	if (glId)
		switch (type) {
			case TYPE_VERTEX:
			case TYPE_FRAGMENT:
				glDeleteShader(glId);
				break;
			default:
				break;
		}
}

int Shader::getType() {
	return type;
}

GLuint Shader::getId() {
	return glId;
}

bool Shader::loadFromFile(ShaderType type, const char* path, Shader &out) {
	if (path == nullptr) {
		std::cerr << "[Shader] ERROR: null pointer as path." << std::endl;
		return false;
	}

	FILE* dat = fopen(path, "rt");
	if (dat == nullptr) {
		std::cout << "[Shader] ERROR: " << path << " is not a valid path." << std::endl;
		return false;
	}

	long length;
	fseek(dat, 0, SEEK_END);
	length = ftell(dat);
	fseek(dat, 0, SEEK_SET);

	char* data = nullptr;
	if (length > 0) {
		data = new char[sizeof(char) * (length + 1)];
		length = (unsigned int) fread(data, sizeof(char), static_cast<size_t>(length), dat);
		data[length] = '\0';
	}
	fclose(dat);

	// Load from memory then:
	bool result = loadFromMemory(type, data, out);

	// Clean:
	delete[] data;

	// Done:
	return result;
}

bool Shader::loadFromMemory(ShaderType type, const char* data, Shader &out) {
	if (data == nullptr) {
		std::cout << "[Shader] ERROR: null pointer as data." << std::endl;
		return false;
	}

	// Check kind:
	int glKind = 0;
	switch (type) {
		case TYPE_VERTEX:
			glKind = GL_VERTEX_SHADER;
			break;

		case TYPE_FRAGMENT:
			glKind = GL_FRAGMENT_SHADER;
			break;

		default:
			std::cout << "[Shader] ERROR: invalid shader type enum value." << std::endl;
			return false;
	}

	// Destroy if already loaded:
	if (out.glId)
		switch (type) {
			case TYPE_VERTEX:
			case TYPE_FRAGMENT:
				glDeleteShader(out.glId);
				break;

			default:
				std::cout << "[Shader] ERROR: invalid shader type enum value." << std::endl;
				return false;
		}

	// Load program:
	out.glId = glCreateShader(glKind);
	if (out.glId == 0) {
		std::cout << "[Shader] ERROR: cannot initiliase shader object." << std::endl;
		return false;
	}
	glShaderSource(out.glId, 1, &data, NULL);
	glCompileShader(out.glId);

	// Verify shader:
	int status;
	char buffer[MAX_LOGSIZE];
	int length = 0;
	memset(buffer, 0, MAX_LOGSIZE);

	glGetShaderiv(out.glId, GL_COMPILE_STATUS, &status);
	glGetShaderInfoLog(out.glId, MAX_LOGSIZE, &length, buffer);
	if (status == false) {
		std::cout << "[Shader] ERROR: could not compile shader: " << buffer << std::endl;
		return false;
	}

	// Update values:
	out.type = type;

	// Done:
	return true;
}
