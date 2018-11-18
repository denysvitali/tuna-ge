#pragma once
#include "stdafx.h"

#ifdef _WINDOWS
#include "version.hh"
#else
#include "../../tuna-ge/version.hh"
#endif

using namespace tunage;
const std::string Version::GIT_SHA1 = "f2b793b-dirty";
const std::string Version::GIT_DATE = "Sat Nov 17 19:13:56 2018";
const std::string Version::GIT_COMMIT_SUBJECT = "Implement basic rendering";
