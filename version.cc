#ifdef _WINDOWS
#include "version.hh"
#else
#include "@CMAKE_CURRENT_SOURCE_DIR@/version.hh"
#endif

using namespace tunage;
const std::string& Version::GIT_SHA1 = "5dc14be-dirty";
const std::string& Version::GIT_DATE = "Fri Apr 19 15:40:55 2019";
const std::string& Version::GIT_COMMIT_SUBJECT = "OVR and Skybox";
