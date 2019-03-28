#ifdef _WINDOWS
#include "version.hh"
#else
#include "@CMAKE_CURRENT_SOURCE_DIR@/version.hh"
#endif

using namespace tunage;
const std::string& Version::GIT_SHA1 = "188fdb3-dirty";
const std::string& Version::GIT_DATE = "Sun Mar 17 20:03:44 2019";
const std::string& Version::GIT_COMMIT_SUBJECT = "Refactor shader";
