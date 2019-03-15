#include "../../libapi.h"

#include "../Shader/shader.h"
#include "../light/Light.h"
#include "../material/Material.h"

namespace tunage {
	class LIB_API Program {
	public:

		const static int MAX_LOGSIZE = 4096;

		Program();
		~Program();

		GLuint getId();
		static Program* getCurrent();

		static bool build(Shader& vert, Shader& frag, Program& out);
		void bind(GLuint location, const char* name);
		void setMatrix(const char* name, glm::mat4 mat);
		void setNormalMatrix(glm::mat3 mat);
		void setLight(Light* light);
		void setMaterial(Material* material);
		GLuint getUniformLocation(const char* name);
		bool render();

	private:
		GLuint glId;
		static Program* current;
	};
}