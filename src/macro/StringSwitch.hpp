/*********************************************************************

          File: StringSwitch.hpp

   Description: StringSwitch Header Part.

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

#ifndef _StringSwitch_hpp_
#define _StringSwitch_hpp_

#include "SystemGlobalHeaders.hpp"

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
