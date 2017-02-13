/*********************************************************************

          File: CommonVariables.hpp

   Description: CommonVariables Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/29

 Copyright (c) 2016, Team "FireBase", Open Robot Marathon, IRC.

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.

*********************************************************************/

#ifndef _CommonVariables_hpp_
#define _CommonVariables_hpp_

#include "SystemCommon.hpp"

#include "CommonModuleControl.hpp"

#if defined(SET_COMMON_MODULE_DATABASE)

// 무조건 std::string을 쓰자!
// std::stringstream은 못쓰겠다.. 너무 느리기도 하고..
class AtomicData {
private:
	int _Length;
	string _Data;

	void _init() {
		_Length = -1;
		_Data.clear();
	}
public:
	AtomicData() {
		_init();
	}

	template <typename T>
	AtomicData(T _InputData) {
		_init();
		Set_Data(_InputData);
	}

	// Database용. 실제로 등록 안된 Class를 집어 넣으려고 하려면 일정한 변환을 한 뒤에 집어 넣어주길 바람.
	template <typename T>
	void Set_Data(T _InputData) {
		// 일반적으로, Anytype이 오려면,
		std::stringstream *_TStreamData = new std::stringstream();
		*_TStreamData << "";
		*_TStreamData << _InputData;
		_Length = _TStreamData->str().size();
		_Data = _TStreamData->str(); // 요것 검증.
		delete _TStreamData;
	}

	void Set_StringData(string &_InputStringData) {
		_Data = _InputStringData;
		_Length = _InputStringData.length();
	}

	int Get_DataLength() {
		return _Length;
	}

	string Get_Data() {
		return _Data;
	}

	// 연산자 재정의. 이 연산은 이 망할놈의............. std::stringstream 때문에 그렇다.
	// 아.. _AtomicData 포인터 형으로 하면, _AtomicData의 주소값만 복사되고,
	// 원래 값은 복사가 되지 않는 치명적인 단점과, 일반 변수형으로 쓰려고 해도,
	// 정말 미친듯이 까다로운 std::stringstream 때문에 vector에 들어가지 않는 상황도 그렇고,
	// 더 어이없는건 포인터형으로 _AtomicData를 만들었을 때, vector같은데서 clear 같은걸 하면
	// 소멸 할때, 안에 있던 주소값까지 소멸해버려서 만약 '=' 같은 연산자로 받으면 주소값만 deep copy되어서
	// 다른곳에서 소멸해버리면, std::stringstream의 주소가 전부 날아가버려서 읽을때 error가 나기 때문에
	// std::stringstream안의 값 자체를 copy 하지 않으면 죽었다가 깨어나도 값을 유지할 수 없다.
	// 이 망할 거지 같은 상황을 어떻게던 해결해보려 한다. -________- 아 빡쳐!!!!!!!!
	/*
	AtomicData &operator =(const AtomicData &__OtherData) {
		// 여기에는, Memory 복사가 일어나야 한다.
		// 진짜 젠장이다.. 생성한거 다시 지우고, 또 생성하고, 초기화하고, 메모리 복사 해줘야 한다. ㅡㅡ;
		// 우어!!! 빡친다!!!!
		if (_Data != NULL) delete _Data;
		_Data = new std::stringstream();
		_init();
		// 길이가 -1 이하거나 0인건 하지마!!!!!! 거지같은 쉐캬 -_- 아예 음수인거나 0인건 오지마!!!!
		if (!(__OtherData._Length <= -1) && __OtherData._Length != 0) {
			memcpy(_Data, (const void *)(__OtherData._Data->str().c_str()), __OtherData._Data->str().size());
		}
		return *this;
	}
	*/
};

typedef struct _STable {
	vector<string> _ColumnName;
	vector<string> _FieldType;
	vector<vector<AtomicData> > _NowTable;
	int _NumberOfColumns;
	unsigned long long _NumberOfRows;
} STable;
#endif

#endif
