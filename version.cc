#ifdef _WINDOWS
#include "version.hh"
#else
#include "@CMAKE_CURRENT_SOURCE_DIR@/version.hh"
#endif

using namespace tunage;
const std::string& Version::GIT_SHA1 = "783b0c8";
const std::string& Version::GIT_DATE = "Fri Mar 15 15:57:36 2019";
const std::string& Version::GIT_COMMIT_SUBJECT = "Ping Phong";
