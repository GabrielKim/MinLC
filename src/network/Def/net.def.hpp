/*********************************************************************

          File: net.def.hpp

   Description: net definition Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2017/02/13

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#ifndef _net_def_hpp_
#define _net_def_hpp_

#include "dep.inc.hpp"

#if defined(WINDOWS_SYS)
//	#ifdef _AFXDLL
//#include <afxwin.h>
//	#else
#	if defined(MINGW_USING)
#include <winsock.h>
//#include <winsock2.h>
#	else
//#		if !defined(USING_MARIA_DB)
#include <windows.h>
//#		endif
#include <process.h>
#	endif
#include <tchar.h>
//#endif
// link the ws2_32.lib
#pragma comment(lib, "ws2_32.lib")
#elif defined(POSIX_SYS)
// for TCP/IP
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>

// for IPC
#include <sys/stat.h>
#include <sys/un.h>

// common
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
// support poll.
#include <sys/poll.h>
#endif

#define IP_ADDR_LOCAL "127.0.0.1"
#define DEFAULT_TCP_PORT 7000
#define DEFAULT_TCP_LISTEN_QUEUE 200

#define TCP_MAX_CONNECTIONS DEFAULT_TCP_LISTEN_QUEUE
#define POLL_MAX_CONNECTIONS (TCP_MAX_CONNECTIONS+1)
#define T_INVAILD -1

typedef
#if defined(WINDOWS_SYS)
  SOCKET
#elif defined(POSIX_SYS)
  int
#endif
    T_SOCKET;

typedef
#if defined(WINDOWS_SYS)
  unsigned long
#elif defined(POSIX_SYS)
  in_addr_t
#endif
    T_IN_ADDR_T;

typedef
#if defined(WINDOWS_SYS)
  SOCKADDR_IN
#elif defined(POSIX_SYS)
  sockaddr_in
#endif
    T_SOCKADDR_IN;

typedef
#if defined(WINDOWS_SYS)
  SOCKADDR_UN
#elif defined(POSIX_SYS)
  sockaddr_un
#endif
    T_SOCKADDR_UN;

typedef
#if defined(WINDOWS_SYS)
  int
#elif defined(POSIX_SYS)
  ssize_t
#endif
    T_SSIZE_T;

#endif // _net_def_hpp_
