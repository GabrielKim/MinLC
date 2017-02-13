/*********************************************************************

          File: StringConverter.cpp

   Description: StringConverter Source Part.

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

#include "StringConverter.hpp"

#if defined(_StringConverter_hpp_)

template<typename T>
string StringConverter::ToString(const T& Any) {
	return dynamic_cast<std::ostringstream &>((std::ostringstream() << std::dec << Any)).str();
}

const char *StringConverter::StringToConstCharPointer(string Str) {
	const char *_TConstStr = Str.c_str();
	char *_TStr = new char[BUFFER_MAX_32767];
	memset(_TStr, 0, sizeof(_TStr));
	strcpy(_TStr, _TConstStr);
	return ConstCharToChar(_TStr);
}

char *StringConverter::ConstCharToChar(const char *Str) {
	char *_TStr = const_cast<char *>(Str);
	return _TStr;
}

char *StringConverter::StringToChar(string Str) {
	return ConstCharToChar(StringToConstCharPointer(Str));
}

#endif // _StringTools_hpp_