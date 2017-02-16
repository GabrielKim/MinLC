/*********************************************************************

          File: Capsulator.hpp

   Description: Capsulator Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#ifndef _Capsulator_hpp_
#define _Capsulator_hpp_

#define IMPLEMENT_GET(__ReturnType, __FuncName, __ReturnValue) \
  __ReturnType Get_##__FuncName() \
  { \
    return __ReturnValue; \
  }

#define IMPLEMENT_SET(__Type, __FuncName, __DestValue) \
  void Set_##__FuncName(__Type __TxV) \
  { \
    __DestValue = __TxV; \
  }

#define IMPLEMENT_GET_SET(__Type, __FuncName, __Value) \
  IMPLEMENT_GET(__Type, __FuncName, __Value) \
  IMPLEMENT_SET(__Type, __FuncName, __Value)

#endif // _Capsulator_hpp_