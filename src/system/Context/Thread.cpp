/*********************************************************************

          File: Thread.cpp

   Description: Thread Source Part.

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

#include "Thread.hpp"

#if defined(_Thread_hpp_)

// Area of Constructor & Destructor.
#pragma region Constructor & Destructor
Thread::~Thread() {
  _Deinitialize();
}
#pragma endregion Constructor & Destructor

// Area of Private Methods.
#pragma region Private Methods

void Thread::_Initialize() {
  _IsThreadStarted = false;
}

void Thread::_Deinitialize() {
#if defined(SET_LIB_PTHREAD)
  AttacheMode = false;
  KernelMode = false;
#elif defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  StackSize = 0;
  WaitingTime = (DWORD)0;
  InitFlag = (DWORD)0;
#endif
  _IsThreadStarted = false;
}

#pragma endregion Private Methods

// Area of Public Methods.
#pragma region Public Methods

void Thread::StartThread(void *(*_StartAddress)(void *), void *Argument) {
#if defined(SET_LIB_PTHREAD)
  int _TAttacheMode = -1;
  int _TKernelMode = -1;
  _TAttacheMode = (AttacheMode == false) ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE;
  _TKernelMode = (KernelMode == false) ? PTHREAD_SCOPE_SYSTEM : PTHREAD_SCOPE_PROCESS;

  // pthread attribute initialize.
  pthread_attr_init(&_ThreadAttr);
  // Detached thread.
  pthread_attr_setdetachstate(&_ThreadAttr, _TAttacheMode);
  // User space thread(linux do not support kernel mode thread, then don't use & set
  // 'PTHREAD_SCOPE_PROCESS' at pthread_attr_setscope function to scope parameter).
  pthread_attr_setscope(&_ThreadAttr, _TKernelMode);
  // Create thread.
  _IsThreadStarted = true;
  if (pthread_create(&_Thread, NULL, _StartAddress, (void *) Argument) < 0) {
    _IsThreadStarted = false;
    return;
  }
#elif defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  _Thread = (HANDLE)_beginthreadex(NULL, StackSize, _StartAddress, Argument, InitFlag, (unsigned *)&_ThreadID);
#endif
}

int Thread::JoinThread() {
  // _TResult is -1 then detatch mode, so not executive.
  int _TRetult = -1;
#if defined(SET_LIB_PTHREAD)
  if (AttacheMode != false) {
    pthread_join(_Thread, (void **) &_TRetult);
  }
#elif defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  _TRetult = WaitForSingleObject(_Thread, WaitingTime);
#endif
  return _TRetult;
}

#if defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
void Thread::ResumeThread() {
  if(InitFlag == CREATE_SUSPENDED && _IsThreadStarted == true) {
    ResumeThread(_Thread);
  }
}
#endif
#pragma endregion Public Methods
#endif // _Thread_hpp_