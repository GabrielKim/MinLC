/*********************************************************************

          File: Ptr.inl.hpp

   Description: Ptr.inl Header Part.
                (This part is tenant Ptr.inl.hpp from OpenCV)

        Author: OpenCV Contributors (Unknown)
      Modifier: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2017/02/06

 Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
 Copyright (C) 2009, Willow Garage Inc., all rights reserved.
 Copyright (C) 2013, OpenCV Foundation, all rights reserved.
 Copyright (C) 2015, Itseez Inc., all rights reserved.

 ** Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved(not this part).
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

#ifndef _Ptr_inl_hpp_
#define _Ptr_inl_hpp_

#include <algorithm>

namespace mlc {
  template<typename Y> void DefaultDeleter<Y>::operator()(Y *p) const { delete p; }

  namespace detail {
    struct PtrOwner {
      PtrOwner() : refCount(1) {}

      void incRef() { MLC_FETCH_ADD(&refCount, 1); }
      void decRef() { if (MLC_FETCH_ADD(&refCount, -1) == 1) deleteSelf(); }

    protected:
      virtual ~PtrOwner() {}
      virtual void deleteSelf() = 0;

    private:
      unsigned int refCount;

      PtrOwner(const PtrOwner &);
      PtrOwner &operator=(const PtrOwner &);
    };

    template<typename Y, typename D>
    struct PtrOwnerImpl : PtrOwner {
      PtrOwnerImpl(Y *p, D d) : owned(p), deleter(d) {}

      void deleteSelf() {
        deleter(owned);
        delete this;
      }

    private:
      Y *owned;
      D deleter;
    };
  }

  template<typename T> Ptr<T>::Ptr() : owner(NULL), stored(NULL) {}

  template<typename T>
  template<typename Y>
  Ptr<T>::Ptr(Y *p)
      : owner(p ? detail::PtrOwnerImpl<Y, DefaultDeleter <Y> > (p, DefaultDeleter<Y>()) : NULL), stored(p) {}

  template<typename T>
  template<typename Y, typename D>
  Ptr<T>::Ptr(Y *p, D d)
      : owner(p ? new detail::PtrOwnerImpl<Y, D>(p, d) : NULL), stored(p) {}

  template<typename T>
  Ptr<T>::Ptr(const Ptr &o) : owner(o.owner), stored(o.stored) { if (owner) owner->incRef(); }

  template<typename T>
  template<typename Y>
  Ptr<T>::Ptr(const Ptr<Y> &o) : owner(o.owner), stored(o.stored) { if (owner) owner->incRef(); }

  template<typename T>
  template<typename Y>
  Ptr<T>::Ptr(const Ptr<Y> &o, T *p) : owner(o.owner), stored(p) {
    if (owner) owner->incRef();
  }

  template<typename T>
  Ptr<T>::~Ptr() { release(); }

  template<typename T>
  Ptr<T> &Ptr<T>::operator=(const Ptr<T> &o) {
    Ptr(o).swap(*this);
    return *this;
  }

  template<typename T>
  template<typename Y>
  Ptr<T> &Ptr<T>::operator=(const Ptr<Y> &o) {
    Ptr(o).swap(*this);
    return *this;
  }

  template<typename T>
  void Ptr<T>::release() {
    if (owner) owner->decRef();
    owner = NULL;
    stored = NULL;
  }

  template<typename T>
  template<typename Y>
  void Ptr<T>::reset(Y *p) {
    Ptr(p).swap(*this);
  }

  template<typename T>
  template<typename Y, typename D>
  void Ptr<T>::reset(Y *p, D d) {
    Ptr(p, d).swap(*this);
  }

  template<typename T>
  void Ptr<T>::swap(Ptr<T> &o) {
    std::swap(owner, o.owner);
    std::swap(stored, o.stored);
  }

  template<typename T>
  T *Ptr<T>::get() const {
    return stored;
  }

  template<typename T> typename detail::RefOrVoid<T>::type Ptr<T>::operator*() const { return *stored; }
  template<typename T> T *Ptr<T>::operator->() const { return stored; }
  template<typename T> Ptr<T>::operator T *() const { return stored; }
  template<typename T> bool Ptr<T>::empty() const { return !stored; }

  template<typename T>
  template<typename Y>
  Ptr<Y> Ptr<T>::staticCast() const { return Ptr<Y>(*this, static_cast<Y *>(stored)); }

  template<typename T>
  template<typename Y>
  Ptr<Y> Ptr<T>::constCast() const { return Ptr<Y>(*this, const_cast<Y *>(stored)); }

  template<typename T>
  template<typename Y>
  Ptr<Y> Ptr<T>::dynamicCast() const { return Ptr<Y>(*this, dynamic_cast<Y *>(stored)); }

#ifdef MLC_CXX_MOVE_SEMANTICS
template<typename T>
Ptr<T>::Ptr(Ptr&& o) : owner(o.owner), stored(o.stored) {
    o.owner = NULL;
    o.stored = NULL;
}

template<typename T>
Ptr<T>& Ptr<T>::operator = (Ptr<T>&& o) {
    if (this == &o)
        return *this;

    release();
    owner = o.owner;
    stored = o.stored;
    o.owner = NULL;
    o.stored = NULL;
    return *this;
}
#endif

  template<typename T>
  void swap(Ptr<T> &ptr1, Ptr<T> &ptr2) { ptr1.swap(ptr2); }

  template<typename T>
  bool operator==(const Ptr<T> &ptr1, const Ptr<T> &ptr2) { return ptr1.get() == ptr2.get(); }

  template<typename T>
  bool operator!=(const Ptr<T> &ptr1, const Ptr<T> &ptr2) { return ptr1.get() != ptr2.get(); }

  template<typename T>
  Ptr<T> makePtr() { return Ptr<T>(new T()); }

  template<typename T, typename A1>
  Ptr<T> makePtr(const A1 &a1) {
    return Ptr<T>(new T(a1));
  }

  template<typename T, typename A1, typename A2>
  Ptr<T> makePtr(const A1 &a1, const A2 &a2) {
    return Ptr<T>(new T(a1, a2));
  }

  template<typename T, typename A1, typename A2, typename A3>
  Ptr<T> makePtr(const A1 &a1, const A2 &a2, const A3 &a3) {
    return Ptr<T>(new T(a1, a2, a3));
  }

  template<typename T, typename A1, typename A2, typename A3, typename A4>
  Ptr<T> makePtr(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4) {
    return Ptr<T>(new T(a1, a2, a3, a4));
  }

  template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5>
  Ptr<T> makePtr(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5) {
    return Ptr<T>(new T(a1, a2, a3, a4, a5));
  }

  template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
  Ptr<T> makePtr(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6) {
    return Ptr<T>(new T(a1, a2, a3, a4, a5, a6));
  }

  template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
  Ptr<T> makePtr(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7) {
    return Ptr<T>(new T(a1, a2, a3, a4, a5, a6, a7));
  }

  template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
  Ptr<T> makePtr(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7,
                 const A8 &a8) {
    return Ptr<T>(new T(a1, a2, a3, a4, a5, a6, a7, a8));
  }

  template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
  Ptr<T> makePtr(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7,
                 const A8 &a8, const A9 &a9) {
    return Ptr<T>(new T(a1, a2, a3, a4, a5, a6, a7, a8, a9));
  }

  template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
  Ptr<T> makePtr(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7,
                 const A8 &a8, const A9 &a9, const A10 &a10) {
    return Ptr<T>(new T(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10));
  }

  template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
  Ptr<T> makePtr(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7,
                 const A8 &a8, const A9 &a9, const A10 &a10, const A11 &a11) {
    return Ptr<T>(new T(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11));
  }

  template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
  Ptr<T> makePtr(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7,
                 const A8 &a8, const A9 &a9, const A10 &a10, const A11 &a11, const A12 &a12) {
    return Ptr<T>(new T(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12));
  }
}

#endif // _Ptr_inl_hpp_