/*********************************************************************

          File: modules.inc.hpp

   Description: modules.inc Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2017/02/05

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

#ifndef _modules_inc_hpp_
#define _modules_inc_hpp_

// algorithm
#if defined(USING_COMMON_MODULE_HASH)
#define SET_COMMON_MODULE_HASH
#endif
#if defined(USING_COMMON_MODULE_RANDOM)
#define SET_COMMON_MODULE_RANDOM
#endif

// data
#if defined(USING_COMMON_MODULE_PACKETSTRUCTER)
#define SET_COMMON_MODULE_PACKETSTRUCTER
#endif

// database
#if defined(USING_COMMON_MODULE_DATABASE)
#define SET_COMMON_MODULE_DATABASE
#endif

// misc
#if defined(USING_COMMON_MODULE_I18N)
#define SET_COMMON_MODULE_I18N
#endif
#if defined(USING_COMMON_MODULE_LOGGING)
#define SET_COMMON_MODULE_LOGGING
#endif
#if defined(USING_COMMON_MODULE_MATHEMATICAL)
#define SET_COMMON_MODULE_MATHEMATICAL
#endif
#if defined(USING_COMMON_MODULE_POSITION)
#define SET_COMMON_MODULE_POSITION
#endif

// network
#if defined(USING_COMMON_MODULE_ETHERNET)
#define SET_COMMON_MODULE_ETHERNET
#endif
#if defined(USING_COMMON_MODULE_IPC)
#define SET_COMMON_MODULE_IPC
#endif

// search
#if defined(USING_COMMON_MODULE_LISTITER)
#define SET_COMMON_MODULE_LISTITER
#endif

// string
#if defined(USING_COMMON_MODULE_STRINGTOOLS)
#define SET_COMMON_MODULE_STRINGTOOLS
#endif

// system
#if defined(USING_COMMON_MODULE_CONTEXT)
#define SET_COMMON_MODULE_CONTEXT
#endif
#if defined(USING_COMMON_MODULE_TIME)
#define SET_COMMON_MODULE_TIME
#endif

// type
#if defined(USING_COMMON_MODULE_TYPE)
#define SET_COMMON_MODULE_TYPE
#endif

#endif // _modules_inc_hpp_
