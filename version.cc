#ifdef _WINDOWS
#include "version.hh"
#else
#include "@CMAKE_CURRENT_SOURCE_DIR@/version.hh"
#endif

using namespace tunage;
const std::string& Version::GIT_SHA1 = "3db68b1-dirty";
const std::string& Version::GIT_DATE = "Fri Mar 15 13:46:10 2019";
const std::string& Version::GIT_COMMIT_SUBJECT = "shaders WIP";
