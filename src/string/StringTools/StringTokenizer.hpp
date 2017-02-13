/*********************************************************************

          File: StringTokenizer.hpp

   Description: StringTokenizer Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18

 Copyright (c) 2016, Team "FireBase", Open Robot Marathon, IRC.

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.

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
	// String Token의 종류.
	void Set_SingleToken(const char *SingleToken);
	// Implement me.
	//void Set_MultiToken(const char *, ...);

	bool Go_StringToken();
};

#endif // _StringTokenizer_hpp_

#endif // SET_COMMON_MODULE_STRINGTOOLS
