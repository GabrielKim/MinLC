/*********************************************************************

          File: SyncSignal.hpp

   Description: SyncSignal Header Part.

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

#ifndef _SyncSignal_hpp_
#define _SyncSignal_hpp_

#include "MacroTools.hpp"

#if defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
#include <windows.h>
#include <process.h>
#else
#include <pthread.h> // for POSIX thread.
#include <semaphore.h> // for POSIX Semaphore.
#endif

#if defined(SET_LIB_PTHREAD)
  #define ThreadExit pthread_exit
  #define ThreadSelf pthread_self

  #define ThreadMutex pthread_mutex_t
  #define ThreadCond pthread_cond_t
  #define ThreadSemaphore sem_t

  #define __SEMWAIT(__SemVar) sem_wait(&__SemVar)
  #define __SEMPOST(__SemVar) sem_post(&__SemVar)

  #define __MUTEXINIT(__MutexVar) { __MutexVar = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER; \
                                    pthread_mutex_init(&__MutexVar, NULL); }
  #define __MUTEXDESTROY(__MutexVar) pthread_mutex_destroy(&__MutexVar)
  #define __MUTEXLOCK(__MutexVar) pthread_mutex_lock(&__MutexVar)
  #define __MUTEXTRYLOCK(__MutexVar) pthread_mutex_trylock(&__MutexVar)
  #define __MUTEXUNLOCK(__MutexVar) pthread_mutex_unlock(&__MutexVar)

  #define __CONDINIT(__CondVar) { __CondVar = (pthread_cond_t)PTHREAD_COND_INITIALIZER; \
                                  pthread_cond_init(&__CondVar, NULL); }
  #define __CONDDESTROY(__CondVar) pthread_cond_destroy(&__CondVar)
  #define __CONDWAIT(__CondVar, __MutexVar) pthread_cond_wait(&__CondVar, &__MutexVar)
  #define __CONDTIMEWAIT(__CondVar, __MutexVar, __Time) pthread_cond_timedwait(&__CondVar, &__MutexVar, &__Time)
  #define __CONDSIGNAL(__CondVar) pthread_cond_signal(&__CondVar)
  #define __CONDBROADCAST(__CondVar) pthread_cond_broadcast(&__CondVar)
#elif defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  #define ThreadMutex HANDLE
  #define ThreadCond CONDITION_VARIABLE
  #define ThreadCS CRITICAL_SECTION
  //#define ThreadSemaphore sem_t

  // #define __SEMWAIT(__SemVar) sem_wait(&__SemVar) //
  // #define __SEMPOST(__SemVar) sem_post(&__SemVar) //

  #define __MUTEXINIT(__MutexVar) { __MutexVar = CreateMutex(NULL, FALSE, NULL); }
  #define __MUTEXDESTROY(__MutexVar) CloseHandle(&__MutexVar)
  #define __MUTEXLOCK(__MutexVar) WaitForSingleObject(__MutexVar, INFINITE)
  #define __MUTEXUNLOCK(__MutexVar) ReleaseMutex(__MutexVar)

  // This part is critical section part(only windows).
  #define __CSINIT(__CSVar) InitializeCriticalSection(&__CSVar)
  #define __CSDESTROY(__CSVar) DeleteCriticalSection(&__CSVar)
  #define __CSLOCK(__CSVar) EnterCriticalSection(&__CSVar)
  #define __CSUNLOCK(__CSVar) LeaveCriticalSection(&__CSVar)

  // This part is condition variable using critical section part(only windows).
  #define __CONDINIT(__CondVar) InitializeConditionVariable(&__CondVar)
  #define __CONDWAIT(__CondVar, __MutexVar) SleepConditionVariableCS(&__CondVar, &__MutexVar, INFINITE)
  #define __CONDTIMEWAIT(__CondVar, __MutexVar, __Time) SleepConditionVariableCS(&__CondVar, &__MutexVar, (__Time * 1000))
  #define __CONDSIGNAL(__CondVar) WakeConditionVariable(&__CondVar)
  #define __CONDBROADCAST(__CondVar) WakeAllConditionVariable(&__CondVar)
#endif

// easy to use the Semaphore.
#define IMPLEMENT_SEMAPHORE_GET(__ReturnType, __FuncName, __ReturnValue, __SemaphoreVal) \
  __ReturnType _Get##__FuncName##WithSemaphore() \
  { \
    __ReturnType _TVal; \
    __SEMWAIT(__SemaphoreVal); \
    _TVal = __ReturnValue; \
    __SEMPOST(__SemaphoreVal); \
    return _TVal; \
  }

#define IMPLEMENT_SEMAPHORE_SET(__Type, __FuncName, __DestValue, __SemaphoreVal) \
  void _Set##__FuncName##WithSemaphore(__Type __TxV) \
  { \
    __SEMWAIT(__SemaphoreVal); \
    __DestValue = __TxV; \
    __SEMPOST(__SemaphoreVal); \
  }

#define IMPLEMENT_SEMAPHORE_GET_SET(__Type, __FuncName, __Value, __MutexVal) \
  IMPLEMENT_SEMAPHORE_GET(__Type, __FuncName, __Value, __SemaphoreVal) \
  IMPLEMENT_SEMAPHORE_SET(__Type, __FuncName, __Value, __SemaphoreVal)

// easy to use the mutex.
#define IMPLEMENT_MUTEX_GET(__ReturnType, __FuncName, __ReturnValue, __MutexVal) \
  __ReturnType _Get##__FuncName##WithMutex() \
  { \
    __ReturnType _TVal; \
    __MUTEXLOCK(__MutexVal); \
    _TVal = __ReturnValue; \
    __MUTEXUNLOCK(__MutexVal); \
    return _TVal; \
  }

#define IMPLEMENT_MUTEX_SET(__Type, __FuncName, __DestValue, __MutexVal) \
  void _Set##__FuncName##WithMutex(__Type __TxV) \
  { \
    __MUTEXLOCK(__MutexVal); \
    __DestValue = __TxV; \
    __MUTEXUNLOCK(__MutexVal); \
  }

#define IMPLEMENT_MUTEX_GET_SET(__Type, __FuncName, __Value, __MutexVal) \
  IMPLEMENT_MUTEX_GET(__Type, __FuncName, __Value, __MutexVal) \
  IMPLEMENT_MUTEX_SET(__Type, __FuncName, __Value, __MutexVal)

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