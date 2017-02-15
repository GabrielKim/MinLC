/*********************************************************************

          File: mlc.base.def.hpp

   Description: mlc.base.def Header Part.
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

#ifndef _Ptr_hpp_
#define _Ptr_hpp_

#include "mlc.Base.def.hpp"

namespace mlc {
  namespace detail {
    template<typename T>
    struct RefOrVoid { typedef T &type; };

    template<>
    struct RefOrVoid<void> { typedef void type; };

    template<>
    struct RefOrVoid<const void> { typedef const void type; };

    template<>
    struct RefOrVoid<volatile void> { typedef volatile void type; };

    template<>
    struct RefOrVoid<const volatile void> { typedef const volatile void type; };
    struct PtrOwner;
  }

  template<typename Y> struct DefaultDeleter { void operator () (Y *p) const; };

  template<typename T>
  struct Ptr {
    typedef T element_type;

    Ptr();

    template<typename Y>
//#ifdef DISABLE_OPENCV_24_COMPATIBILITY
    explicit
//#endif
    Ptr(Y *p);

    template<typename Y, typename D> Ptr(Y *p, D d);
    Ptr(const Ptr &o);
    template<typename Y> Ptr(const Ptr<Y> &o);
    template<typename Y> Ptr(const Ptr<Y> &o, T *p);
    ~Ptr();

    Ptr &operator=(const Ptr &o);
    template<typename Y> Ptr &operator=(const Ptr<Y> &o);

    void release();
    template<typename Y> void reset(Y *p);
    template<typename Y, typename D> void reset(Y *p, D d);

    void swap(Ptr &o);
    T *get() const;
    typename detail::RefOrVoid<T>::type operator*() const;
    T *operator->() const;
    operator T *() const;
    bool empty() const;
    template<typename Y> Ptr<Y> staticCast() const;
    template<typename Y> Ptr<Y> constCast() const;
    template<typename Y> Ptr<Y> dynamicCast() const;

#ifdef CV_CXX_MOVE_SEMANTICS
    Ptr(Ptr&& o);
    Ptr& operator = (Ptr&& o);
#endif
    private:
    detail::PtrOwner *owner;
    T *stored;

    template<typename Y>
    friend
    struct Ptr;
  };

  template<typename T> void swap(Ptr<T> &ptr1, Ptr<T> &ptr2);
  template<typename T> bool operator==(const Ptr<T> &ptr1, const Ptr<T> &ptr2);
  template<typename T> bool operator!=(const Ptr<T> &ptr1, const Ptr<T> &ptr2);
  template<typename T> Ptr<T> makePtr();

  template<typename T, typename A1> Ptr<T> makePtr(const A1 &a1);
  template<typename T, typename A1, typename A2> Ptr<T> makePtr(const A1 &a1, const A2 &a2);
  template<typename T, typename A1, typename A2, typename A3>
  Ptr<T> makePtr(const A1 &a1, const A2 &a2, const A3 &a3);
  template<typename T, typename A1, typename A2, typename A3, typename A4>
  Ptr<T> makePtr(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4);
  template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5>
  Ptr<T> makePtr(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5);
  template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
  Ptr<T> makePtr(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6);
  template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
  Ptr<T> makePtr(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7);
  template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
  Ptr<T> makePtr(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7,
                 const A8 &a8);
  template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
  Ptr<T> makePtr(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7,
                 const A8 &a8, const A9 &a9);
  template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
  Ptr<T> makePtr(const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7,
                 const A8 &a8, const A9 &a9, const A10 &a10);
}

#include "Ptr.inl.hpp"

#endif // _Ptr_hpp_
