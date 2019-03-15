#include "../Shader/shader.h"

class Program {
public:

	const static int MAX_LOGSIZE = 4096;

	Program();
	~Program();

	GLuint getId();

	static bool build(Shader& vert, Shader& frag, Program& out);
	bool render();

private:
	GLint glId;
};