#ifdef _WINDOWS
#include "version.hh"
#else
#include "@CMAKE_CURRENT_SOURCE_DIR@/version.hh"
#endif

using namespace tunage;
const std::string& Version::GIT_SHA1 = "30cdb4a-dirty";
const std::string& Version::GIT_DATE = "Sun May 5 15:56:46 2019";
const std::string& Version::GIT_COMMIT_SUBJECT = "change scene";
