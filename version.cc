#ifdef _WINDOWS
#include "version.hh"
#else
#include "@CMAKE_CURRENT_SOURCE_DIR@/version.hh"
#endif

using namespace tunage;
const std::string& Version::GIT_SHA1 = "15b566a-dirty";
const std::string& Version::GIT_DATE = "Fri Mar 29 23:12:21 2019";
const std::string& Version::GIT_COMMIT_SUBJECT = "Refactor of displayCB: class FrontRender to handle FBOs";
