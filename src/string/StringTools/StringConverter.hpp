/*********************************************************************

          File: StringConverter.hpp

   Description: StringConverter Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#include "dep.inc.hpp"

#if defined(SET_COMMON_MODULE_STRINGTOOLS)

#ifndef _StringConverter_hpp_
#define _StringConverter_hpp_

#include "required.inc.hpp"

class StringConverter {
private:

public:
  template<typename T> string ToString(const T &Any);
  const char *StringToConstCharPointer(string Str);
  char *ConstCharToChar(const char *Str);
  char *StringToChar(string Str);
};

#endif // _StringTools_hpp_

#endif // SET_COMMON_MODULE_STRINGTOOLS