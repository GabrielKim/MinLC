/*********************************************************************

          File: FisherYatesShuffle.hpp

   Description: FisherYatesShuffle Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/30

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#include "dep.inc.hpp"

#if defined(SET_COMMON_MODULE_RANDOM)

#ifndef _FisherYatesShuffle_hpp_
#define _FisherYatesShuffle_hpp_

class FisherYatesShuffle {
private:
  int _PsudoRandomNumber(int __Range);
  int _PsudoFisherYatesShuffle(int __Range);
public:
  int Go_Shuffle(int Range);
};

#endif // _FisherYatesShuffle_hpp_

#endif