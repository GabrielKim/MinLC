/*********************************************************************

          File: TimeChecker.cpp

   Description: TimeChecker Source Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#include "TimeChecker.hpp"

#if defined(_TimeChecker_hpp_)

void Time::Start() {
  _StartTime = clock();
}

void Time::End() {
  _EndTime = clock();
}

double Time::Get_ElapsedTime() {
  return (double) (_EndTime - _StartTime) / CLOCKS_PER_SEC;
}

#endif // _TimeChecker_hpp_