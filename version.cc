#ifdef _WINDOWS
#include "version.hh"
#else
#include "@CMAKE_CURRENT_SOURCE_DIR@/version.hh"
#endif

using namespace tunage;
const std::string& Version::GIT_SHA1 = "aeab0d7-dirty";
const std::string& Version::GIT_DATE = "Thu Mar 28 23:08:26 2019";
const std::string& Version::GIT_COMMIT_SUBJECT = "Add fbo class, fix texture";
