/*********************************************************************

          File: syscheck.inc.hpp

   Description: syscheck.inc Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/29

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#ifndef _syscheck_inc_hpp_
#define _syscheck_inc_hpp_

#include <limits.h>

/*
	A "System Dependent" section specification syntax
	that its some of auto detection during compilation.

 	DO NOT TOUCH THIS CODE!!
*/

/* Distinguish environment that System and Compiler */
// Windows or POSIX.
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32)
 // using windows.
 #define WINDOWS_SYS
 #ifdef __MINGW32__
   // using MinGW(for windows).
  #define MINGW_USING
  #if defined(__GNUC__)
   #define WIN_QT_USING
   #define USING_QT
  #endif
 #endif
 // Qt and mingw g++ mkspecs using Always.
#else
// using Linux, FreeBSD, Mac OSX.
 #define POSIX_SYS
 #if defined(__GNUC__) && (defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD) || defined(Q_OS_MAC) || defined(Q_OS_UNIX) \
  || defined(Q_OS_CYGWIN) || defined(Q_OS_DARWIN))
  #define POSIX_QT_USING
  #define USING_QT
 #endif
 #if defined(__MACH__)
  #define __M_MACH_
 #endif
#endif

// using or not QT.
#if defined(USING_QT)
 #define MAKE_ARTFAO
#endif

/* Debug Mode part */
// Debug Mode or not.
// If defined Debug mode by compiler(M$ Visual Studio & Qt Creator).
#if defined(_DEBUG) || !defined(QT_NO_DEBUG)
 #define DEBUG_MODE
#endif

/* Test Mode part */
// do or not for Test Build.
#if defined(MAKE_TEST)
 #define TEST_MODE
#endif

#if defined(MAKE_LOG_WRITE)
 #define LOG_WRITE_MODE
#endif

#endif // _syscheck_inc_hpp_