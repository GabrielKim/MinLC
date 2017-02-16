/*********************************************************************

          File: Timer.hpp

   Description: Timer Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2017/02/14

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#include "dep.inc.hpp"

#if defined(SET_COMMON_MODULE_TIME)

#ifndef _Timer_hpp_
#define _Timer_hpp_

#include <chrono>

class Timer {
private:
  std::chrono::microseconds _TimerDurations;

  bool _IsStarted;

  void _Initialize();
  void _Elapsed();

  static void *_Timer_ElapsedThread(void *Param);

public:
  Timer() { _Initialize(); }
  ~Timer() {}
  typedef void (* _T_TIMEREXPIREDNOTIFIER)(void);
  _T_TIMEREXPIREDNOTIFIER TTimerExpiredNotifier;

  void Set_Timer(long long uSec);
  void Start_Timer();
  void Stop_Timer();
};

#endif // _Timer_hpp_

#endif // SET_COMMON_MODULE_TIME