/*********************************************************************

          File: Timer.cpp

   Description: Timer Source Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2017/02/14

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

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