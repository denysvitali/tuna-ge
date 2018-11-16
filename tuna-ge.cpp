#include "stdafx.h"
#include "tuna-ge.h"
#include "version.hh"
#include <iostream>
#include <sstream>



bool TunaGE::init() {
	std::cout << "TunaGE::init()" << std::endl;
	return true;
}

std::string TunaGE::version() {
    std::stringstream ss{};
    ss << LIB_MAJOR << "." << LIB_MINOR << "." << LIB_PATCH;
    if(!Version::GIT_SHA1.empty()){
        ss << "-" << Version::GIT_SHA1;
    }
    return ss.str();
}