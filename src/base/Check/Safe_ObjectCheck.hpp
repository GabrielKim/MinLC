/*********************************************************************

          File: Safe_ObjectCheck.hpp

   Description: Safe_ObjectCheck Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2017/02/06

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#ifndef _Safe_ObjectCheck_hpp_
#define _Safe_ObjectCheck_hpp_

#include "Context.def.hpp"

namespace mlc {
  template<typename T>
  bool IsEmptyObject(T &__Obj, ThreadMutex &__Mutex) {
    bool _TResult = false;

    __MUTEXLOCK(__Mutex);
    if (__Obj) _TResult = true;
    __MUTEXUNLOCK(__Mutex);

    return _TResult;
  }
}

#endif // _Safe_ObjectCheck_hpp_
