/*********************************************************************

          File: FisherYatesShuffle.cpp

   Description: FisherYatesShuffle Source Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/30

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

#include "FisherYatesShuffle.hpp"

#if defined(_FisherYatesShuffle_hpp_)

#include "required.inc.hpp"

int FisherYatesShuffle::_PsudoRandomNumber(int __Range) {
  srand(time(NULL));
  return (int)(rand() % __Range);
}

int FisherYatesShuffle::_PsudoFisherYatesShuffle(int __Range) {
  if (__Range == 0)
    return 0;

  int _tCnt = _PsudoRandomNumber(__Range);
  int _tGetRandomNum = -1;

  // Array Initialize
  vector<int> _tShuffleDump = vector<int>();

  for (int i=0; i<__Range; ++i) {
    _tShuffleDump.push_back(i);
  }

  // STL Standard Shuffle
  random_shuffle(_tShuffleDump.begin(), _tShuffleDump.end());

  // Cnt번 만큼 땡기면 끝이다. 마치 러시안 룰렛 같다.
  while (_tCnt > -1) {
    // Fisher-Yates Shuffle
    int _tRandomNum = _PsudoRandomNumber(_tShuffleDump.size());
    _tGetRandomNum = _tShuffleDump[_tRandomNum];

    // 해당 인덱스와 맨 끝 요소를 Swap.
    iter_swap(_tShuffleDump.begin() + _tRandomNum, _tShuffleDump.end() - 1);
    // 맨 끝 요소는 버림.
    _tShuffleDump.pop_back();
    _tCnt--;
    // 더 짧게 끝내기 위해 급사 시킴.
    //if (_PsudoRandomNumber(10) > 4)
    //break;
  }

  return _tGetRandomNum;
}

int FisherYatesShuffle::Go_Shuffle(int Range) {
  return _PsudoFisherYatesShuffle(Range);
}

#endif