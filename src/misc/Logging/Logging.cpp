/*********************************************************************

          File: Loggging.cpp

   Description: Loggging Source Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#include "Logging.hpp"

#if defined(_Logging_hpp_)

#if !defined(NO_LOGGING_MODULE)

void Logging::Logger(const char *CallValueString, const char *Message, ...) {
#if defined(LOGGING_MODE)
  va_list _TArgument_List;
  char _TStr[BUFFER_MAX_4096] = {0,};
  string _TString;
  va_start(_TArgument_List, Message);
  vsprintf(_TStr, std::string(Message).append("\n").c_str(), _TArgument_List);
  va_end(_TArgument_List);

  TLogCallback(CallValueString, _TStr);
#endif
}

#endif

void Logging::SetCallback(void (*TCallbackFunc)(const char *, const char *)) {
#if defined(LOGGING_MODE)
  TLogCallback = TCallbackFunc;
#endif
}

#endif // _LogD_hpp_