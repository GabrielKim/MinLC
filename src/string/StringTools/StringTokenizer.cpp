/*********************************************************************

          File: StringTokenizer.cpp

   Description: StringTokenizer Source Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#include "StringTokenizer.hpp"

#if defined(_StringTokenizer_hpp_)

StringTokenizer::StringTokenizer() {
  _Initialize_StringTokenizer();
}

StringTokenizer::~StringTokenizer() {
  _Deinitialize_StringTokenizer();
}

void StringTokenizer::_Initialize_StringTokenizer() {
  Clear_StringToken();
  _StringList = new list<string>();
}

void StringTokenizer::_Deinitialize_StringTokenizer() {
  delete _StringList;
}

void StringTokenizer::Clear_StringList() {
  // String List Clear.
  _StringList->clear();
}

void StringTokenizer::Clear_StringToken() {
  // Token List Clear.
  _Token = "";
}

list<char *> *StringTokenizer::Get_TokenedCharListArrays() {
  list<char *> *_TempTokenedCharList = new list<char *>();

  for_IterToEndC(list, string, _StringList, i) {
    char *_TCharArray = (char *) (_i->c_str());
    _TempTokenedCharList->push_back(_TCharArray);
  }

  return _TempTokenedCharList;
}

// String Input Functions.
void StringTokenizer::Set_InputCharString(const char *Str) {
  _InternalInputString.append(Str);
}

void StringTokenizer::Set_SingleToken(const char *SingleToken) {
  string _TempTokenString(SingleToken);
  _Token = _TempTokenString;
}

// Implement me.
/*
void StringTokenizer::Set_MultiToken(const char *, ...) {

}
*/

bool StringTokenizer::Go_StringToken() {
  string _TempString = _InternalInputString;
  int _TTokenPointer = 0;

  // if, return Key or Empty that return false.
  if (_TempString.c_str() == "\r" || _TempString.c_str() == "" || _TempString.empty())
    return false;

  // Go Tokening.
  while ((_TTokenPointer = _TempString.find_first_of(_Token)) != _TempString.npos) {
    if (_TTokenPointer > 0)
      _StringList->push_back(_TempString.substr(0, _TTokenPointer));
    _TempString = _TempString.substr(_TTokenPointer + 1);
  }

  // Put a last String.
  if (_TempString.length() > 0)
    _StringList->push_back(_TempString.substr(0, _TTokenPointer));

  return true;
}

#endif // _StringTokenizer_hpp_