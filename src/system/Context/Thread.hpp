/*********************************************************************

          File: Thread.hpp

   Description: Thread Header Part.

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

#include "dep.inc.hpp"

#if defined(SET_COMMON_MODULE_CONTEXT)

#ifndef _Thread_hpp_
#define _Thread_hpp_

#if defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
#include <windows.h>
#include <process.h>
#else
// for POSIX thread.
#include <pthread.h>
// for POSIX Semaphore.
#include <semaphore.h>
#endif

class Thread {
private:
#if defined(SET_LIB_PTHREAD)
	pthread_t _Thread;
	pthread_attr_t _ThreadAttr;
#elif defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  HANDLE _Thread;
  DWORD _ThreadID;
#endif
  bool _IsThreadStarted;

  void _Initialize();
  void _Deinitialize();
public:
	Thread()
#if defined(SET_LIB_PTHREAD)
      : AttacheMode(false), KernelMode(false)
#elif defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
      : StackSize(0), InitFlag(0), WaitingTime(INFINITE)
#endif
  { _Initialize(); }

#if defined(SET_LIB_PTHREAD)
	Thread(bool ExcuteAttacheMode, bool ExcuteKernelMode) : AttacheMode(ExcuteAttacheMode), KernelMode(ExcuteKernelMode)
#elif defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  // This initializer for windows thread
  Thread(int _StackSize = 0) : StackSize(_StackSize), WaitingTime(INFINITE)
  // it can be able to set "initflag"
  Thread(DWORD _InitFlag = 0) : StackSize(0), InitFlag(_InitFlag), WaitingTime(INFINITE)
  Thread(int _StackSize = 0, DWORD _WaitingTime = INFINITE) : StackSize(_StackSize), WaitingTime(_WaitingTime)
  Thread(int _StackSize = 0, DWORD _InitFlag = 0) : StackSize(_StackSize), InitFlag(_InitFlag)
  Thread(int _StackSize = 0, DWORD _InitFlag = 0, DWORD _WaitingTime = INFINITE) : StackSize(_StackSize), InitFlag(_InitFlag), WaitingTime(_WaitingTime)
#endif
  { _Initialize(); }

	~Thread();

#if defined(SET_LIB_PTHREAD)
	// This is associated with start Thread and join.
	bool AttacheMode;
	bool KernelMode;
#elif defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  int StackSize;
  DWORD WaitingTime;
  DWORD InitFlag;
#endif
	void StartThread(void *(*_StartAddress) (void *), void *Argument);
	int JoinThread();
#if defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  void ResumeThread();
#endif
};

#endif // _Thread_hpp_

#endif // SET_COMMON_MODULE_CONTEXT