/*********************************************************************

          File: TimeChecker.hpp

   Description: TimeChecker Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#include "dep.inc.hpp"

#if defined(SET_COMMON_MODULE_TIME)

#ifndef _TimeChecker_hpp_
#define _TimeChecker_hpp_

#include "required.inc.hpp"

class Time {
private:
  clock_t _StartTime, _EndTime;
public:
  void Start();
  void End();
  double Get_ElapsedTime();
};

#endif // _TimeChecker_hpp_

#endif // SET_COMMON_MODULE_TIME