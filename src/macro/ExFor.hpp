/*********************************************************************

          File: ExFor.hpp

   Description: ExFor Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#ifndef _ExFor_hpp_
#define _ExFor_hpp_

// Iter name using "_TVal"
#define for_IterToEnd(_TTemplate, _TType, _TTypeObject, _TVName) \
for (_TTemplate<_TType >::iterator _##_TVName = _TTypeObject.begin(); \
  _##_TVName != _TTypeObject.end(); _##_TVName++)

#define for_IterToEndC(_TTemplate, _TType, _TTypeObject, _TVName) \
for (_TTemplate<_TType >::iterator _##_TVName = _TTypeObject->begin(); \
  _##_TVName != _TTypeObject->end(); _##_TVName++)

#define for_IterToBegin(_TTemplate, _TType, _TTypeObject, _TVName) \
for (_TTemplate<_TType >::iterator _##_TVName = _TTypeObject.end(); \
  _##_TVName != _TTypeObject.begin(); _##_TVName--)

#define for_IterToBeginC(_TTemplate, _TType, _TTypeObject, _TVName) \
for (_TTemplate<_TType >::iterator _##_TVName = _TTypeObject->end(); \
  _##_TVName != _TTypeObject->begin(); _##_TVName--)

#endif // _ExFor_hpp_