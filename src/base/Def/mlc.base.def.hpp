/*********************************************************************

          File: mlc.base.def.hpp

   Description: mlc.base.def Header Part.
                (This part is tenant cvdef.h from OpenCV)

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

#ifndef _mlc_base_def_hpp_
#define _mlc_base_def_hpp_

#include <limits.h>

#ifndef MLC_INLINE
  #if defined __cplusplus
    #define MLC_INLINE static inline
  #elif defined _MSC_VER
    #define MLC_INLINE __inline
  #else
    #define MLC_INLINE static
  #endif
#endif

#ifdef MLC_FETCH_ADD
// not anymore.
#elif defined __GNUC__
  #if defined __clang__ && __clang_major__ >= 3 && !defined __ANDROID__ && !defined __EMSCRIPTEN__ && !defined(__CUDACC__)
    #ifdef __ATOMIC_ACQ_REL
      #define MLC_FETCH_ADD(addr, delta) __c11_atomic_MLC_FETCH_ADD((_Atomic(int)*)(addr), delta, __ATOMIC_ACQ_REL)
    #else
      #define MLC_FETCH_ADD(addr, delta) __atomic_MLC_FETCH_ADD((_Atomic(int)*)(addr), delta, 4)
    #endif
  #else
    #if defined __ATOMIC_ACQ_REL && !defined __clang__
       // version for gcc >= 4.7
      #define MLC_FETCH_ADD(addr, delta) (int)__atomic_MLC_FETCH_ADD((unsigned*)(addr), (unsigned)(delta), __ATOMIC_ACQ_REL)
    #else
      #define MLC_FETCH_ADD(addr, delta) (int)__sync_fetch_and_add((unsigned*)(addr), (unsigned)(delta))
    #endif
  #endif
#elif defined _MSC_VER && !defined RC_INVOKED
  #include <intrin.h>
  #define MLC_FETCH_ADD(addr, delta) (int)_InterlockedExchangeAdd((long volatile*)addr, delta)
#else
   MLC_INLINE MLC_FETCH_ADD(int* addr, int delta) { int tmp = *addr; *addr += delta; return tmp; }
#endif

#ifndef MLC_CXX_MOVE_SEMANTICS
  #if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__) || defined(_MSC_VER) && _MSC_VER >= 1600
    #define MLC_CXX_MOVE_SEMANTICS 1
  #elif defined(__clang)
    #if __has_feature(cxx_rvalue_references)
      #define MLC_CXX_MOVE_SEMANTICS 1
    #endif
  #endif
#else
  #if MLC_CXX_MOVE_SEMANTICS == 0
    #undef MLC_CXX_MOVE_SEMANTICS
  #endif
#endif

#endif // _mlc_base_def_hpp_
