#ifdef _WINDOWS
#include "version.hh"
#else
#include "@CMAKE_CURRENT_SOURCE_DIR@/version.hh"
#endif

using namespace tunage;
const std::string& Version::GIT_SHA1 = "NO_SHA1_COMMIT";
const std::string& Version::GIT_DATE = "NO_DATE";
const std::string& Version::GIT_COMMIT_SUBJECT = "[NO COMMIT FOUND]";
