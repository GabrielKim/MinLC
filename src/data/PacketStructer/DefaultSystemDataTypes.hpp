/*********************************************************************

          File: DefaultSystemDataTypes.hpp

   Description: DefaultSystemDataTypes Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2017/02/08

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

#ifndef _DefaultSystemDataTypes_hpp_
#define _DefaultSystemDataTypes_hpp_

#define TYPE_CASE 13
#define REF_ARRAY 0

/*
    System Types(Reffered by Unix System).

    >> characters.
    char : 1, unsigned char : 1

    >> integers.
    short : 2, unsigned short : 2
    int : 4, unsigned int : 4
    long : 8, long long : 8, unsigned long : 8, unsigned long long : 8

    >> Floating point.
    float : 4, double : 8, long double : 16

    >> Others.
    do not Support.
 */

const char *__DefaultTypeName[TYPE_CASE] = {
    "char", "unsigned char", // character.
    "short", "unsigned short", "int", "unsigned int", "long", "long long", "unsigned long", "unsigned long long", // integers.
    "float", "double", "long double" // Floating point(not yet..).
};
enum DefaultTypeEnum {
  T_DEFSTART = 0,
  T_CHAR = 1001, T_UCHAR = 1002, // character(number of 1).
  T_SHORT = 2001, T_USHORT = 2002, T_INT = 2003, T_UINT = 2004, T_LONG = 2005, T_LONGLONG = 2006, T_ULONG = 2007, T_ULONGLONG = 2008, // integers(number of 2).
  T_FLOAT = 3001, T_DOUBLE = 3002, T_LONGDOUBLE = 3003, // Floating point(number of 3).
  T_DEFEND = 14
};

#endif //_DefaultSystemDataTypes_hpp_
