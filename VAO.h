#pragma once

#include "libapi.h"

#include <GL/glew.h>

namespace tunage {
	class LIB_API VAO
	{
	public:
		VAO();
		~VAO();

	private:

		GLuint glId;

	};
}