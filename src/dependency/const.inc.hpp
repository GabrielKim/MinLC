/*********************************************************************

          File: const.inc.hpp

   Description: const.inc Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/29

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#ifndef _const_inc_hpp_
#define _const_inc_hpp_

#include "syscheck.inc.hpp"

// Define the all system constant in here.
#define ZERO_ 0
//#undef NULL
//#define NULL ((void *)0)
#if defined(MINGW_USING)
#define NULL 0
#define VOIDNULL ((void *)0)
#define NULL_ __null
#define NOTNULL_ __notnull
#define MAYBENULL_ __maybenull
#endif

// Buffer Size Defines.
#define BUFFER_MAX_16 16
#define BUFFER_MAX_32 32
#define BUFFER_MAX_64 64
#define BUFFER_MAX_128 128
#define BUFFER_MAX_256 256
#define BUFFER_MAX_512 512
#define BUFFER_MAX_1024 1024
#define BUFFER_MAX_2048 (BUFFER_MAX_1024 * 2)
#define BUFFER_MAX_4096 (BUFFER_MAX_1024 * 4)
#define BUFFER_MAX_32767 ((BUFFER_MAX_1024 * 32) - 1)
#define BUFFER_MAX_65535 ((BUFFER_MAX_1024 * 64) - 1)

#if defined(WINDOWS_SYS)
// using MFC.
//#if !MINGW_USING
//#define _AFXDLL
//#endif
// or no MFC.

// Maximum Process Limit.
#define PROCESS_MAX 32767
#endif

#endif // _const_inc_hpp_