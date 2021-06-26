#include "system.h"

#if defined(__WINDOWS__)
	#include <direct.h>
	#define getcwd _getcwd
#else
	#include <unistd.h>
#endif
