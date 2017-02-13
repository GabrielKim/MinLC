/*********************************************************************

          File: StringTokenizer.hpp

   Description: StringTokenizer Header Part.

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

#include "modinclude.hpp"

#if defined(SET_COMMON_MODULE_STRINGTOOLS)

#ifndef _StringTokenizer_hpp_
#define _StringTokenizer_hpp_

#include "SystemCommon.hpp"

class StringTokenizer {
private:
	list<string> *_StringList;
	string _Token;

	string _InternalInputString;

	// Initializers.
	void _Initialize_StringTokenizer();
	void _Deinitialize_StringTokenizer();
public:
	StringTokenizer();
	~StringTokenizer();

	void Clear_StringList();
	void Clear_StringToken();

	// Get Functions.
	IMPLEMENT_GET(list<string> *, TokenedStringList, _StringList); // thus, list<string> *Get_TokenedStringList();
	list<char *> *Get_TokenedCharListArrays();

	// Set Functions.
	// String Input Functions.
	void Set_InputCharString(const char* Str);
	IMPLEMENT_SET(string, InputString, _InternalInputString); // thus, void Set_InputString(string Str);
	// case of String Token.
	void Set_SingleToken(const char *SingleToken);
	// Implement me.
	//void Set_MultiToken(const char *, ...);

	bool Go_StringToken();
};

#endif // _StringTokenizer_hpp_

#endif // SET_COMMON_MODULE_STRINGTOOLS
