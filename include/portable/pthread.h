#pragma once
#include "system.h"

#if defined(__WINDOWS__)

	#include <Windows.h>
	#include <stdint.h>
	#include <time.h>
	
	#define PTHREAD_CALL WINAPI
	
	#define PTHREAD_CREATE_DETACHED   0
	#define PTHREAD_CREATE_JOINABLE   1

	#define PTHREAD_MUTEX_NORMAL   0
	#define PTHREAD_MUTEX_ERRORCHECK   1
	#define PTHREAD_MUTEX_RECURSIVE   2
	#define PTHREAD_MUTEX_DEFAULT   PTHREAD_MUTEX_NORMAL
	
	typedef HANDLE pthread_t;
	typedef DWORD pthread_code_t;

	typedef struct {
		int _reserved;
	} pthread_attr_t;
	
	static inline int pthread_attr_init(pthread_attr_t* attr) { return 0; }
	static inline int pthread_attr_destroy(pthread_attr_t* attr) { return 0; }
	static inline int pthread_attr_setdetachstate(pthread_attr_t* attr, int detachstate) { return 0; }

	// - Rounds to the nearest millisecond on WinAPI
	static inline int nanosleep(const struct timespec* req, struct timespec* rem)
	{
		if (rem)
			memset(rem, 0, sizeof(*rem));
		Sleep(req->tv_sec * 1000 + (req->tv_nsec / 1000000));
		return 0;
	}
	static inline unsigned int sleep(unsigned int seconds) {
		return Sleep(seconds * 1000), seconds;
	}
	
	static inline int pthread_create(pthread_t* thread, const pthread_attr_t* ignored, pthread_code_t(PTHREAD_CALL* start_routine)(void*), void* arg) {
		return (*thread = CreateThread(0, 0, start_routine, arg, 0, 0)) ? 0 : -1;
	}
	
	static inline int pthread_join(pthread_t thread, pthread_code_t* retval)
	{
		BOOL ok = WaitForSingleObject(thread, INFINITE) == WAIT_OBJECT_0;
		if (ok && retval)
			GetExitCodeThread(thread, retval);
		return ok ? 0 : -1;
	}

	static inline int pthread_cancel(pthread_t thread)
	{
		BOOL ok = TerminateThread(thread, (DWORD)-1);
		CloseHandle(thread);
		return ok ? 0 : -1;
	}

#else
	#include <pthread.h>
	#include <unistd.h>

	#define PTHREAD_CALL

	typedef void* pthread_code_t;
#endif

static inline void msleep(unsigned ms)
{
	struct timespec slep;
	slep.tv_sec = ms / 1000, ms -= slep.tv_sec * 1000;
	slep.tv_nsec = ms * 1000000;
	nanosleep(&slep, 0);
}
