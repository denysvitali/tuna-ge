#ifdef _WINDOWS
#include "version.hh"
#else
#include "@CMAKE_CURRENT_SOURCE_DIR@/version.hh"
#endif

using namespace tunage;
const std::string& Version::GIT_SHA1 = "1cfe640-dirty";
const std::string& Version::GIT_DATE = "Mon Apr 29 18:07:06 2019";
const std::string& Version::GIT_COMMIT_SUBJECT = "Smaller scene";
