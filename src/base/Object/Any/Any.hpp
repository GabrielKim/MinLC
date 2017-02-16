/*********************************************************************

          File: Any.hpp

   Description: Any Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2017/02/06

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#ifndef _Any_hpp_
#define _Any_hpp_

#include <type_traits>
#include <utility>
#include <typeinfo>
#include <string>
#include <cassert>

// for Demangling.
#include <exception>
#include <cxxabi.h>

//using namespace std;

namespace mlc {
  template<class T>
  using __StorageType = typename decay<typename remove_reference<T>::type>::type;

  struct Any {
    private:
    struct Base {
      virtual ~Base() { }
      virtual Base *Clone() const = 0;
      virtual std::string TypeName() const = 0;
      virtual std::string TypenameDemangle() const = 0;
    };

    template<typename T>
    struct Derived : Base {
      template<typename U>
      Derived(U &&Val) : _Val(forward<U>(Val)) { }

      T _Val;

      Base *Clone() const { return new Derived<T>(_Val); }
      std::string TypeName() const { return std::string(typeid(_Val).name()); }
      std::string TypenameDemangle() const {
        int _TStatus;
        return std::string(abi::__cxa_demangle(typeid(_Val).name(), 0, 0, &_TStatus));
      }
    };

    Base *Clone() const {
      if (Ptr)
        return Ptr->Clone();
      else
        return nullptr;
    }

    std::string TypeName() const {
      if (Ptr)
        return Ptr->TypeName();
    }

    std::string TypenameDemangle() const {
      if (Ptr)
        return Ptr->TypenameDemangle();
    }

    Base *Ptr;

    public:
    template<typename U>
    Any(U &&Val) : Ptr(new Derived<__StorageType<U> >(forward<U>(Val))) { }
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
    std::string Get_Type() { return TypeName(); }
    std::string Get_TypeDemangle() { return TypenameDemangle(); }

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

    template<class U> operator U() { return As<__StorageType<U> >(); }
  };
}

#endif //_Any_hpp_