#pragma once
#include "system.h"
#include "portable_endian.h"

#if defined(__WINDOWS__)

	#include <WinSock2.h>
	#include <WS2tcpip.h>
	#pragma comment(lib, "Ws2_32.lib") // MSVC only, manually link if other compiler...
	
	typedef SOCKET socket_t;
	
	static inline int socket_startup()
	{
		WSADATA data;
		return WSAStartup(MAKEWORD(2, 2), &data);
	}

	static inline int socket_cleanup() {
		return WSACleanup();
	}

#else

	#include <sys/socket.h>
	#include <arpa/inet.h>
	
	typedef int socket_t;

	#define socket_startup() ((int)0)
	#define socket_cleanup() ((int)0)

#endif

#define SOCKET_VALID(sock) ((sock < 0) || (sock == -1)) // Due to unsigned versus signed socket types
