/*********************************************************************

          File: SyncSignal.cpp

   Description: SyncSignal Source Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18

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