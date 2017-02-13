/*********************************************************************

          File: syscheck.inc.hpp

   Description: syscheck.inc Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/29

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.
 Third party copyrights are property of their respective owners.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in
    the documentation and/or other materials provided with the
    distribution.
 3. Neither the name of the copyright holder nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 OF THE POSSIBILITY OF SUCH DAMAGE.

*********************************************************************/

#ifndef _syscheck_inc_hpp_
#define _syscheck_inc_hpp_

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
#	ifdef __MINGW32__
// using MinGW(for windows).
#	define MINGW_USING
#		if defined(__GNUC__)
#		define WIN_QT_USING
#		define USING_QT
#		endif
#	endif
// Qt mingw g++ mkspecs using Always.
#else
// using Linux, FreeBSD, Mac OSX.
#	define POSIX_SYS
#	if defined(__GNUC__) && (defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD) || defined(Q_OS_MAC) || defined(Q_OS_UNIX) \
		|| defined(Q_OS_CYGWIN) || defined(Q_OS_DARWIN))
#	define POSIX_QT_USING
#	define USING_QT
#	endif
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