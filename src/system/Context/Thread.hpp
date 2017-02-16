/*********************************************************************

          File: Thread.hpp

   Description: Thread Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#include "dep.inc.hpp"

#if defined(SET_COMMON_MODULE_CONTEXT)

#ifndef _Thread_hpp_
#define _Thread_hpp_

#if defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
#include <windows.h>
#include <process.h>
#else
#include <pthread.h> // for POSIX thread.
#include <semaphore.h> // for POSIX Semaphore.
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

  void StartThread(void *(*_StartAddress)(void *), void *Argument);
  int JoinThread();
#if defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  void ResumeThread();
#endif
};

#endif // _Thread_hpp_

#endif // SET_COMMON_MODULE_CONTEXT