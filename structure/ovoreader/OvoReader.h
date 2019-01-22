#pragma once

#include "../node/Node.h"

namespace tunage {
	class OvoReader {
	public:
		static Node* parse(const char* path);
	};
}