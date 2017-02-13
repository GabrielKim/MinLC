/*********************************************************************

          File: LogD.hpp

   Description: LogD Header Part.

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