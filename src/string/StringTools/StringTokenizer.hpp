/*********************************************************************

          File: StringTokenizer.hpp

   Description: StringTokenizer Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#include "dep.inc.hpp"

#if defined(SET_COMMON_MODULE_STRINGTOOLS)

#ifndef _StringTokenizer_hpp_
#define _StringTokenizer_hpp_

#include "required.inc.hpp"

#include "MacroTools.hpp"

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
  void Set_InputCharString(const char *Str);

  IMPLEMENT_SET(string, InputString, _InternalInputString); // thus, void Set_InputString(string Str);
  // case of String Token.
  void Set_SingleToken(const char *SingleToken);
  // Implement me.
  //void Set_MultiToken(const char *, ...);
  bool Go_StringToken();
};

#endif // _StringTokenizer_hpp_

#endif // SET_COMMON_MODULE_STRINGTOOLS