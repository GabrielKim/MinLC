/*********************************************************************

          File: Deleter.hpp

   Description: Deleter Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2017/02/06

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#ifndef _Deleter_hpp_
#define _Deleter_hpp_

namespace mlc {
  // only create "new"
  template <typename T>
  void DeletePointer(T *obj) {
    delete obj;
    obj = NULL;
  }

  // only create "*alloc"
  template <typename T>
  void FreePointer(T *obj) {
    free(obj);
    obj = NULL;
  }
}

#endif // _Deleter_hpp_
