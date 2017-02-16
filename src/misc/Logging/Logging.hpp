/*********************************************************************

          File: Loggging.hpp

   Description: Loggging Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#include "dep.inc.hpp"

#if defined(SET_COMMON_MODULE_LOGGING)

#ifndef _Logging_hpp_
#define _Logging_hpp_

#include "const.inc.hpp"

class Logging {
  private:
  public:
  Logging() { /*SetCallback(_DummyAllLogCallback, "All"); */};

  void Logger(const char *CallValueString, const char *Message, ...)
#if defined(NO_LOGGING_MODULE)
  {}
#endif
  ;

  void SetCallback(void (*TCallbackFunc)(const char *, const char *));
  static void _DummyAllLogCallback(const char *CallValueString, const char *Message) {};
};

typedef void(*_T_LOGCALLBACK)(const char *CallValueString, const char *Message);

static _T_LOGCALLBACK TLogCallback;
//static std::map<string, _T_LOGCALLBACK> TLogCallback;
//typedef _T_LOGCALLBACK _T_SLOGCALLBACK;
static Logging *G_LogD = new Logging();
#endif // _Logging_hpp_

#endif // SET_MODULE_LOGGING