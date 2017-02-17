/*********************************************************************

          File: SyncSignal.hpp

   Description: SyncSignal Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#include "dep.inc.hpp"

#if defined(SET_COMMON_MODULE_CONTEXT)

#ifndef _SyncSignal_hpp_
#define _SyncSignal_hpp_

#include "Context.def.hpp"

#include "MacroTools.hpp"

class SyncSignal {
private:
  ThreadMutex _Mutex;
#if defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  ThreadCS _CriticalSection;
#endif
  ThreadCond _Cond;
  bool _IsWaiting;

  void _Initialize();
  void _Deinitialize();

public:
  SyncSignal();
  ~SyncSignal();

  void Wait();
  void Signal();

  IMPLEMENT_GET(bool, IsWaiting, _IsWaiting);
};

#endif // _SyncSignal_hpp_

#endif // SET_COMMON_MODULE_CONTEXT