/*********************************************************************

          File: Any.hpp

   Description: Any Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2017/02/06

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

#ifndef _Any_hpp_
#define _Any_hpp_

#include <type_traits>
#include <utility>
#include <typeinfo>
#include <string>
#include <cassert>

using namespace std;

template<class T>
using __StorageType = typename decay<typename remove_reference<T>::type>::type;

struct Any {
private:
  struct Base {
    virtual ~Base() { }
    virtual Base *Clone() const = 0;
    virtual string TypeName() const = 0;
  };

  template<typename T>
  struct Derived : Base {
    template<typename U>
    Derived(U &&Val) : _Val(forward<U>(Val)) { }

    T _Val;

    Base *Clone() const { return new Derived<T>(_Val); }
    string TypeName() const { return string(typeid(_Val).name()); }
  };

  Base *Clone() const {
    if (Ptr)
      return Ptr->Clone();
    else
      return nullptr;
  }

  string TypeName() const {
    if (Ptr)
      return Ptr->TypeName();
  }

  Base *Ptr;

public:
  template<typename U>
  Any(U &&Val) : Ptr(new Derived<__StorageType<U>>(forward<U>(Val))) { }
  Any() : Ptr(nullptr) { }
  Any(Any &That) : Ptr(That.Clone()) { }
  Any(Any &&That) : Ptr(That.Ptr) { That.Ptr = nullptr; }
  Any(const Any &That) : Ptr(That.Clone()) { }
  Any(const Any &&That) : Ptr(That.Clone()) { }
  Any &operator=(const Any &Val) {
    if (Ptr == Val.Ptr) return *this;
    auto _TOldPtr = Val.Clone();
    if (_TOldPtr) delete _TOldPtr;
    return *this;
  }
  Any &operator=(Any &&a) {
    if (Ptr == a.Ptr) return *this;

    swap(Ptr, a.Ptr);
    return *this;
  }

  ~Any() { if (Ptr) delete Ptr; }

  bool Is_Null() const { return !Ptr; }
  bool Not_Null() const { return Ptr; }
  string Get_Type() { return TypeName(); }

  template<class U>
  bool Is() const {
    typedef __StorageType<U> T;
    auto _TDerived = dynamic_cast<Derived <T> *> (Ptr);
    return _TDerived;
  }

  template<class U>
  __StorageType<U> &As() {
    typedef __StorageType<U> T;
    auto _TDerived = dynamic_cast<Derived <T> *> (Ptr);

    if (!_TDerived)
      throw bad_cast();

    return _TDerived->_Val;
  }

  template<class U> operator U() { return As<__StorageType<U>>(); }
};

#endif //_Any_hpp_
