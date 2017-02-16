/*********************************************************************

          File: ListIter.hpp

   Description: ListIter.hpp Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#include "dep.inc.hpp"

#if defined(SET_COMMON_MODULE_LISTITER)

#ifndef _ListIter_hpp_
#define _ListIter_hpp_

#include "required.inc.hpp"
#include "MacroTools.hpp"

using namespace std;

template<typename T>
class ListIter {
  private:
  list<T> *_List;
  typedef typename list<T>::iterator ISI;
  ISI _ListIter;

  bool _IsInitializedList;
  bool _IsInitialize;
  int _EndPoint;

  // Area of Private Methods.
#pragma region Private Methods

  void _Initialize_Members() {
    _IsInitializedList = false;
    _IsInitialize = false;
  }

  void _Initialize() {}

  void _Deinitialize() {
    if (_IsInitialize == true)
      _Release_Pointer();
  }

  void _Initialize_List(list<T> *__List) {
    _List = new list<T>();

    _List = __List;
    _ListIter = _List->begin();
    _EndPoint = __List->size();
    _IsInitialize = true;
    _IsInitializedList = true;
  }

  void _Release_Pointer() {
    if (_List != NULL && _IsInitializedList == true)
      delete _List;

    _IsInitialize = false;
  }

#pragma endregion Private Methods
  public:
#pragma region Constructor & Destructor
  ListIter() {
    _Initialize_Members();
    _Initialize();
  }

  ListIter(list<T> *AnyList) {
    _Initialize_Members();
    _Initialize();
    _Initialize_List(AnyList);
  }

  ~ListIter() {
    _Deinitialize();
    _Initialize_Members();
  }
#pragma endregion Constructor & Destructor

  // Area of Public Methods.
#pragma region Public Methods
  IMPLEMENT_GET(typename list<T>::iterator, NowStringIter, _ListIter); // thus, list<T>::iterator Get_NowStringIter();

  bool Move_PrevStringIter() {
    if (_IsInitialize == true && Is_FirstStringIter() != true) {
      _ListIter--;
      return true;
    }
    return false;
  }

  bool Move_NextStringIter() {
    if (_IsInitialize == true && Is_LastStringIter() != true) {
      _ListIter++;
      return true;
    }
    return false;
  }

  bool Is_FirstStringIter() {
    typename list<T>::iterator _TIter = _ListIter;
    _TIter--;
    if (_IsInitialize == true && _List->begin() != _TIter) {
      return false;
    }
    return true;
  }

  bool Is_LastStringIter() {
    typename list<T>::iterator _TIter = _ListIter;
    _TIter++;
    if (_IsInitialize == true && _List->end() != _TIter) {
      return false;
    }
    return true;
  }

#pragma endregion Public Methods
};

#endif // _ListIter_hpp_

#endif // SET_COMMON_MODULE_LISTITER