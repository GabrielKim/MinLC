/*********************************************************************

          File: StringConverter.cpp

   Description: StringConverter Source Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#include "StringConverter.hpp"

#if defined(_StringConverter_hpp_)

template<typename T>
string StringConverter::ToString(const T &Any) {
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