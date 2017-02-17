/*********************************************************************

          File: PacketStructer.hpp

   Description: PacketStructer Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2017/02/05

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#include "dep.inc.hpp"

#if defined(SET_COMMON_MODULE_PACKETSTRUCTER)

#ifndef _PacketStructer_hpp_
#define _PacketStructer_hpp_

#include <iostream>
#include <string>

#include <vector>
#include <map>

#include "DefaultSystemDataTypes.hpp"

#define REFFER_SIZE 8 // reffered by 8bit(1byte).

using namespace std;

class PacketStreamStructer {
private:
  int __DefaultTypeCaseSize[TYPE_CASE] = {1, 1, 2, 2, 4, 4, 8, 8, 8, 8, 4, 8, 16};

  map<string, int> _ManggledNameLink;
  map<string, int> _StringNameLink;
  map<int, pair<int, intptr_t> > _CodeNumberLink;

  void _DecideDefaultSystemType() {
    // Link a default type size on "Manggled String".
    _ManggledNameLink[string(typeid(char).name())]
        = _StringNameLink[__DefaultTypeName[REF_ARRAY]] = T_CHAR;
    _ManggledNameLink[string(typeid(unsigned char).name())]
        = _StringNameLink[__DefaultTypeName[REF_ARRAY + 1]] = T_UCHAR;
    _ManggledNameLink[string(typeid(short).name())]
        = _StringNameLink[__DefaultTypeName[REF_ARRAY + 2]] = T_SHORT;
    _ManggledNameLink[string(typeid(unsigned short).name())]
        = _StringNameLink[__DefaultTypeName[REF_ARRAY + 3]] = T_USHORT;
    _ManggledNameLink[string(typeid(int).name())]
        = _StringNameLink[__DefaultTypeName[REF_ARRAY + 4]] = T_INT;
    _ManggledNameLink[string(typeid(unsigned int).name())]
        = _StringNameLink[__DefaultTypeName[REF_ARRAY + 5]] = T_UINT;
    _ManggledNameLink[string(typeid(long).name())]
        = _StringNameLink[__DefaultTypeName[REF_ARRAY + 6]] = T_LONG;
    _ManggledNameLink[string(typeid(long long).name())]
        = _StringNameLink[__DefaultTypeName[REF_ARRAY + 7]] = T_LONGLONG;
    _ManggledNameLink[string(typeid(unsigned long).name())]
        = _StringNameLink[__DefaultTypeName[REF_ARRAY + 8]] = T_ULONG;
    _ManggledNameLink[string(typeid(unsigned long long).name())]
        = _StringNameLink[__DefaultTypeName[REF_ARRAY + 9]] = T_ULONGLONG;
    _ManggledNameLink[string(typeid(float).name())]
        = _StringNameLink[__DefaultTypeName[REF_ARRAY + 10]] = T_FLOAT;
    _ManggledNameLink[string(typeid(double).name())]
        = _StringNameLink[__DefaultTypeName[REF_ARRAY + 11]] = T_DOUBLE;
    _ManggledNameLink[string(typeid(long double).name())]
        = _StringNameLink[__DefaultTypeName[REF_ARRAY + 12]] = T_LONGDOUBLE;

    _CodeNumberLink[DefaultTypeEnum::T_CHAR] = pair<int, intptr_t>(-1, (intptr_t)&(*(__DefaultTypeCaseSize + REF_ARRAY)));
    _CodeNumberLink[DefaultTypeEnum::T_UCHAR] = pair<int, intptr_t>(-1, (intptr_t)&(*(__DefaultTypeCaseSize + REF_ARRAY + 1)));
    _CodeNumberLink[DefaultTypeEnum::T_SHORT] = pair<int, intptr_t>(-1, (intptr_t)&(*(__DefaultTypeCaseSize + REF_ARRAY + 2)));
    _CodeNumberLink[DefaultTypeEnum::T_USHORT] = pair<int, intptr_t>(-1, (intptr_t)&(*(__DefaultTypeCaseSize + REF_ARRAY + 3)));
    _CodeNumberLink[DefaultTypeEnum::T_INT] = pair<int, intptr_t>(-1, (intptr_t)&(*(__DefaultTypeCaseSize + REF_ARRAY + 4)));
    _CodeNumberLink[DefaultTypeEnum::T_UINT] = pair<int, intptr_t>(-1, (intptr_t)&(*(__DefaultTypeCaseSize + REF_ARRAY + 5)));
    _CodeNumberLink[DefaultTypeEnum::T_LONG] = pair<int, intptr_t>(-1, (intptr_t)&(*(__DefaultTypeCaseSize + REF_ARRAY + 6)));
    _CodeNumberLink[DefaultTypeEnum::T_LONGLONG] = pair<int, intptr_t>(-1, (intptr_t)&(*(__DefaultTypeCaseSize + REF_ARRAY + 7)));
    _CodeNumberLink[DefaultTypeEnum::T_ULONG] = pair<int, intptr_t>(-1, (intptr_t)&(*(__DefaultTypeCaseSize + REF_ARRAY + 8)));
    _CodeNumberLink[DefaultTypeEnum::T_ULONGLONG] = pair<int, intptr_t>(-1, (intptr_t)&(*(__DefaultTypeCaseSize + REF_ARRAY + 9)));
    _CodeNumberLink[DefaultTypeEnum::T_FLOAT] = pair<int, intptr_t>(-1, (intptr_t)&(*(__DefaultTypeCaseSize + REF_ARRAY + 10)));
    _CodeNumberLink[DefaultTypeEnum::T_DOUBLE] = pair<int, intptr_t>(-1, (intptr_t)&(*(__DefaultTypeCaseSize + REF_ARRAY + 11)));
    _CodeNumberLink[DefaultTypeEnum::T_LONGDOUBLE] = pair<int, intptr_t>(-1, (intptr_t)&(*(__DefaultTypeCaseSize + REF_ARRAY + 12)));
  }

  // Packer.
  template <typename T>
  string _ConvertAnyToByteString(T __ConvVar) {
    int _TVarSize = sizeof(__ConvVar);
    char *_TArr = (char *)malloc(_TVarSize * sizeof(char));
    string _TStr = "";
    T _TTypeMaskVar = 0xFF;

    memset(_TArr, 0, sizeof(char));

    for (int i=_TVarSize - 1; i>=0; i--){
      if (i != (_TVarSize -1))
        _TTypeMaskVar = _TTypeMaskVar << REFFER_SIZE;
      _TArr[i] = (__ConvVar & _TTypeMaskVar) >> (REFFER_SIZE * ((_TVarSize - 1) - i));
    }

    for (int i=0; i<_TVarSize; i++)
      _TStr +=_TArr[i];
    free(_TArr);

    return _TStr;
  }

  // Unpacker.
  int _ConvertTypeNameToCode(string __TypeName) {
    return _StringNameLink[__TypeName];
  }

  int _ConvertTypeManggledNameToCode(string __TypeManggledName) {
    return _ManggledNameLink[__TypeManggledName];
  }

  int _GetTypeSize(int __TypeCodeNum) {
    int _TTypeSize = -1;

    if (_CodeNumberLink.count(__TypeCodeNum)) {
      // chase code number link.
      int _TLinkedCodeNum = _CodeNumberLink[__TypeCodeNum].first;
      int _TNowCodeNum = __TypeCodeNum;
      while (_CodeNumberLink[_TLinkedCodeNum].first == -1) {
        _TNowCodeNum = _TLinkedCodeNum;
        _TLinkedCodeNum = _CodeNumberLink[_TLinkedCodeNum].first;
      }
      _TTypeSize = *(int *)((intptr_t)_CodeNumberLink[_TNowCodeNum].second);
    }
    return _TTypeSize;
  }

  template <typename T>
  int _TypeEval(T __TypeCode) {
    int _TTypeSize = -1;

    // check types.
    if (!((typeid(string) == typeid(T))
          || typeid(char *) == typeid(T)
          || typeid(int) == typeid(T))) {
      // contain variable type is support.
      string _TStr = "";
      int _TNumTypeCode = -1;

      if (typeid(int) != typeid(T))
        _TStr = string(__TypeCode);

      if (_TStr.length() != 0) {
        // contain Manggled Name in temporary var?
        if (_ManggledNameLink.count(_TStr))
          _TNumTypeCode = _ConvertTypeManggledNameToCode(_TStr);
        else
          _TNumTypeCode = _ConvertTypeNameToCode(_TStr);
      }
      else
        _TNumTypeCode = (intptr_t)__TypeCode;

      _TTypeSize = _GetTypeSize(_TNumTypeCode);
    }

    return _TTypeSize;
  }

  template <typename T, typename U>
  bool _ConvertByteStringToAny(string __ConvertVar, T &__RetVar, U __TypeCode) {
    int _TTypeSize = _TypeEval(__TypeCode);
    if (_TTypeSize == -1)
      return false;

    T _TRetVar = 0;

    for (int i=_TTypeSize - 1; i>=0; i--) {
      _TRetVar += ((unsigned char)__ConvertVar[i] & 0xFF) << (REFFER_SIZE * ((_TTypeSize -1) - i));
    }

    __RetVar = _TRetVar;

    return true;
  }

public:
  PacketStreamStructer() {
    _DecideDefaultSystemType();
  }

  template <typename T>
  string Convert(T ConvVar) {
    return _ConvertAnyToByteString(ConvVar);
  }

  template <typename T, typename U>
  void Restore(string ConvertVar, T &RetVar, U TypeCode) {
    _ConvertByteStringToAny(ConvertVar, RetVar, TypeCode);
  }
};

#endif //_PacketStructer_hpp_

#endif