#ifdef _WINDOWS
#include "version.hh"
#else
#include "@CMAKE_CURRENT_SOURCE_DIR@/version.hh"
#endif

using namespace tunage;
const std::string& Version::GIT_SHA1 = "6a75c6a-dirty";
const std::string& Version::GIT_DATE = "Tue May 7 16:28:18 2019";
const std::string& Version::GIT_COMMIT_SUBJECT = "Skybox images";
