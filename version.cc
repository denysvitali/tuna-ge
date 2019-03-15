#ifdef _WINDOWS
#include "version.hh"
#else
#include "@CMAKE_CURRENT_SOURCE_DIR@/version.hh"
#endif

using namespace tunage;
const std::string& Version::GIT_SHA1 = "2d9ad76-dirty";
const std::string& Version::GIT_DATE = "Fri Mar 15 10:46:47 2019";
const std::string& Version::GIT_COMMIT_SUBJECT = "VAOs done, shaders WIP";
