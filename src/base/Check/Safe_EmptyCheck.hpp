/*********************************************************************

          File: Safe_EmptyCheck.hpp

   Description: Safe_EmptyCheck Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2017/02/06

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#ifndef _Safe_EmptyCheck_hpp_
#define _Safe_EmptyCheck_hpp_

#include "Context.def.hpp"

namespace mlc {
  template<typename T>
  bool IsEmptySTLObject_Safe(T &__STLObj, ThreadMutex &__Mutex) {
    bool _TResult = false;

    __MUTEXLOCK(__Mutex);
    _TResult = __STLObj.empty();
    __MUTEXUNLOCK(__Mutex);

    return _TResult;
  }
}

#endif // _Safe_EmptyCheck_hpp_
