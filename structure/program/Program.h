#include "../../libapi.h"

#include "../shader/Shader.h"
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
		void setMatrix4x4(const char* name, glm::mat4 mat);
		void setMatrix3x3(const char* name, glm::mat3 mat);
		void setVec4(const char* name, glm::vec4 vec);
		void setLight(Light* light, glm::mat4 pos);
		void setMaterial(Material* material);
		GLuint getUniformLocation(const char* name);
		bool render();

	private:
		GLuint glId;
		static Program* current;
		static unsigned int idCounter;
	};
}