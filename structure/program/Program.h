#include "../shader/Shader.h"

class Program {
public:

	const static int MAX_LOGSIZE = 4096;
	
	Program();
	~Program();

	GLuint getId();
	static Program* getCurrent();

	static bool build(Shader& vert, Shader& frag, Program& out);
	void bind(GLuint location, const char* name);
	void setMatrix(const char* name, glm::mat4 mat);
	void setMatrix(int location, glm::mat4 mat);
	void setMatrix(int location, glm::mat3 mat);
	GLint getUniformLocation(const char* name);
	bool render();

private:
	GLuint glId;
	static Program* current;
};