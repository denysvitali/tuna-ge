#pragma once


// Generic info:

#define LIB_NAME	"TunaGE"
#define LIB_MAJOR	1
#define LIB_MINOR	0
#define LIB_PATCH	0


#ifdef _WINDOWS 	
	#ifdef TUNAGE_EXPORTS
		#define LIB_API __declspec(dllexport)
	#else
		#define LIB_API __declspec(dllimport)
  #endif
#else // Under Linux
	#define LIB_API  // Dummy declaration
#endif


class LIB_API TunaGE {
public:
	static bool init();
	static bool free();
private:
	TunaGE() {}
	~TunaGE() {}
};