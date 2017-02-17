/*********************************************************************

          File: Context.inc.hpp

   Description: Context.inc Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2017/02/06

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#ifndef _Context_inc_hpp_
#define _Context_inc_hpp_

#include "dep.inc.hpp"

#if defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
#include <windows.h>
#include <process.h>
#else
#include <pthread.h> // for POSIX thread.
#include <semaphore.h> // for POSIX Semaphore.
#endif

#endif // _Context_inc_hpp_
