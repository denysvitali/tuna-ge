#pragma once

#ifdef _WINDOWS
#include <direct.h>
	#define GetCurrentDir _getcwd
#else
#include <unistd.h>
	#define GetCurrentDir getcwd
#endif