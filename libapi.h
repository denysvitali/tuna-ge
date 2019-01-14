#pragma once

#ifdef _WINDOWS 
#include <Windows.h>
#ifdef TUNAGE_EXPORTS
	#define LIB_API __declspec(dllexport)
#else
	#define LIB_API __declspec(dllimport)
#endif
#else // Under Linux
	#define LIB_API  // Dummy declaration
#endif