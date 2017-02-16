/*********************************************************************

          File: modules.inc.hpp

   Description: modules.inc Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2017/02/05

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

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
