#ifdef _WINDOWS
#include "version.hh"
#else
#include "@CMAKE_CURRENT_SOURCE_DIR@/version.hh"
#endif

using namespace tunage;
const std::string& Version::GIT_SHA1 = "f309395-dirty";
const std::string& Version::GIT_DATE = "Fri Mar 8 14:08:07 2019";
const std::string& Version::GIT_COMMIT_SUBJECT = "comments cleanup";
