/*********************************************************************

          File: Timer.cpp

   Description: Timer Source Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2017/02/14

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

#include "Timer.hpp"

#if defined(_Timer_hpp_)

#include "Thread.hpp"

#include <thread>

/*
 it's a simple timer.
 must using C++11 "chrono" and "thread"
 */

void Timer::_Initialize() {
  _IsStarted = false;
}

void Timer::_Elapsed() {
  TTimerExpiredNotifier();
}

void *Timer::_Timer_ElapsedThread(void *Param) {
  Timer *_TTimer = (Timer *) Param;

  while(_TTimer->_IsStarted) {
    std::this_thread::sleep_for(_TTimer->_TimerDurations);
    _TTimer->_Elapsed();
  }
  return 0;
}

void Timer::Set_Timer(long long uSec) {
  std::chrono::microseconds _TTimerDurations(uSec);
  _TimerDurations = _TTimerDurations;
}

void Timer::Start_Timer() {
  Thread _TThread;

  _IsStarted = true;
  _TThread.StartThread(_Timer_ElapsedThread, this);
}

void Timer::Stop_Timer() {
  _IsStarted = false;
}

#endif // _TimeChecker_hpp_