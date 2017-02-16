/*********************************************************************

          File: StringSwitch.hpp

   Description: StringSwitch Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#ifndef _StringSwitch_hpp_
#define _StringSwitch_hpp_

#include <map>

/*
	 ! Notice :
	 About this construct that a name of "SSWITCH_BEGIN".
	 if, you use MSVC, must given option that "Zi(Program Database mode)".

	 ! Usage :
	 SSWITCH_BEGIN(Str)
	 {
		 CASE("Name")
			...
			break;
		 CASE("Age")
	　 　...
	 　　break;
	 }
	 SSWITCH_END()
*/

#define SSWITCH_BEGIN(Str) \
{ \
  static std::map<string, int> _TMap; \
  static bool _TInit = false; \
  bool _TLoop = true; \
  while (_TLoop) \
  { \
    int _TNumberOfStr = -1; \
    if (_TInit) { _TNumberOfStr = _TMap[Str]; _TLoop = false; } \
      switch (_TNumberOfStr) \
      { \
        case -1: {

#define CASE(_StrToken) } case __LINE__: if (!_TInit) _TMap[_StrToken] = __LINE__; else {
#define DEFAULT() } case 0: default: if (_TInit) {
#define SSWITCH_END() \
} \
} \
if (!_TInit) _TInit = true; \
} \
}

#endif
