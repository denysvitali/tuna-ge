#pragma once
#include <string>

#define GUI_MAJOR 1
#define GUI_MINOR 0
#define GUI_PATCH 0

namespace tunage {
	struct Version {
		static const std::string GIT_SHA1;
		static const std::string GIT_DATE;
		static const std::string GIT_COMMIT_SUBJECT;
	};
}