#include "Program.h"

using namespace tunage;

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

Program* Program::getCurrent()
{
	return Program::current;
}


void Program::setMatrix4x4(const char* name, glm::mat4 mat) {
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr<float>(mat));
}

void Program::setMatrix3x3(const char* name, glm::mat3 mat) {
	glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr<float>(mat));
}

void Program::setLight(Light * light, glm::mat4 pos) {
	glm::vec3 light_position = glm::vec3(pos[3][0], pos[3][1], pos[3][2]);
	glUniform3fv(glGetUniformLocation(glId, "light_position"), 1, glm::value_ptr<float>(light_position));
	glUniform3fv(glGetUniformLocation(glId, "light_ambient"), 1, glm::value_ptr<float>(light->getLightAmbient()));
	glUniform3fv(glGetUniformLocation(glId, "light_diffuse"), 1, glm::value_ptr<float>(light->getLightDiffuse()));
	glUniform3fv(glGetUniformLocation(glId, "light_specular"), 1, glm::value_ptr<float>(light->getLightSpecular()));
}

void Program::setMaterial(Material* material) {
	glUniform3fv(glGetUniformLocation(glId, "material_ambient"), 1, glm::value_ptr<float>(material->getAmbient()));
	glUniform3fv(glGetUniformLocation(glId, "material_diffuse"), 1, glm::value_ptr<float>(material->getDiffuse()));
	glUniform3fv(glGetUniformLocation(glId, "material_emissive"), 1, glm::value_ptr<float>(material->getEmission()));
	glUniform3fv(glGetUniformLocation(glId, "material_specular"), 1, glm::value_ptr<float>(material->getSpecular()));
	glUniform1i(glGetUniformLocation(glId, "material_shininess"), material->getShininess());
}

GLuint Program::getUniformLocation(const char* name) {
	if (!name) return -1;
	return glGetUniformLocation(glId, name);
}

void Program::bind(GLuint location, const char* name) {
	glBindAttribLocation(glId, location, name);
}

bool Program::build(Shader& vert, Shader& frag, Program& out)
{
	if (vert.getType() != Shader::TYPE_VERTEX)
	{
		std::cout << "[Program] ERROR: invalid vertex shader." << std::endl;
		return false;
	}
	if (frag.getType() != Shader::TYPE_FRAGMENT)
	{
		std::cout << " [Program] ERROR: invalid fragment shader." << std::endl;
		return false;
	}

	// Delete if already used:
	if (out.glId)
	{
		glDeleteProgram(out.glId);
	}

	// Create program:
	out.glId = glCreateProgram();
	if (out.glId == 0)
	{
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
	if (status == false)
	{
		std::cout << "[Program] ERROR: " << buffer << std::endl;
		return false;
	}
	glValidateProgram(out.glId);
	glGetProgramiv(out.glId, GL_VALIDATE_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::cout << "[Program] ERROR: Unable to validate program." << std::endl;
		return false;
	}

	// Done:
	return true;
}

bool Program::render()
{
	if (glId) {
		glUseProgram(glId);
		Program::current = this;
	}
	else {
		std::cout << "[Program] ERROR: render() method called on invalid Program." << std::endl;
		return false;
	}
	return true;
}
