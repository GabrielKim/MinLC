/*********************************************************************

          File: SyncSignal.cpp

   Description: SyncSignal Source Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#include "SyncSignal.hpp"

#if defined(_SyncSignal_hpp_)

SyncSignal::SyncSignal() {
  _Initialize();
}

SyncSignal::~SyncSignal() {
  _Deinitialize();
}

void SyncSignal::_Initialize() {
  __CONDINIT(_Cond);
#if defined(SET_LIB_PTHREAD)
  __MUTEXINIT(_Mutex);
#elif defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  __CSINIT(_CriticalSection);
#endif
  _IsWaiting = false;
}

void SyncSignal::_Deinitialize() {
  __CONDDESTROY(_Cond);
#if defined(SET_LIB_PTHREAD)
  __MUTEXDESTROY(_Mutex);
#endif
  _IsWaiting = false;
}

void SyncSignal::Wait() {
  _IsWaiting = true;
#if defined(SET_LIB_PTHREAD)
  __MUTEXLOCK(_Mutex);
#elif defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  __CSLOCK(_CriticalSection)
#endif
  //while(_IsWaiting == true)
  __CONDWAIT(_Cond, _Mutex);
#if defined(SET_LIB_PTHREAD)
  __MUTEXUNLOCK(_Mutex);
#elif defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  __CSUNLOCK(_CriticalSection)
#endif
}

void SyncSignal::Signal() {
  _IsWaiting = false;
  __CONDSIGNAL(_Cond);
}

#endif // _SyncSignal_hpp_