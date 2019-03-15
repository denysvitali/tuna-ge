#ifdef _WINDOWS
#include "version.hh"
#else
#include "@CMAKE_CURRENT_SOURCE_DIR@/version.hh"
#endif

using namespace tunage;
const std::string& Version::GIT_SHA1 = "36bb815-dirty";
const std::string& Version::GIT_DATE = "Thu Mar 14 21:07:56 2019";
const std::string& Version::GIT_COMMIT_SUBJECT = "VAO wip";
