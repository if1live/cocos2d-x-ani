// Ŭnicode please
#pragma once

#include <vector>
#include <algorithm>

namespace sora {;
///@brief for_each로 포인터를 폭파시킬떄쓰는 functor
///http://accu.org/index.php/journals/372
/// Delete function object.
struct DeleteObject {
	template<typename Pointer>
	void operator() (Pointer pointer) {
		delete pointer;
	}
};

///@brief map처럼 key-value로 된것인데 value가 포인터로 된것을 해제시키는 functor
///(std::pair<T1,T2> 타입을 받아서 적절히 처리한다)
struct DeleteObjectPair {
	template<typename Key, typename Pointer>
	void operator() (std::pair<Key, Pointer> pair) {
		delete(pair.second);
	}
};


///@brief stl container와 지우는 functor를 받아서 컨테이너 내부의 내용을 비우고 포인터도 삭제한다
template<typename ContainerType, typename Deletor>
void ClearContainer(ContainerType &container, Deletor deletor)
{
	typename ContainerType::iterator beginIt = container.begin();
	typename ContainerType::iterator endIt = container.end();
	std::for_each(beginIt, endIt, deletor);
	container.clear();
}

///@brief list, vector와 같은 stl container가 T*를 타입을 처리할때
///컨테이너의 모든 포인터를 해제함과 컨테이너를 비운다
template<typename ContainerType>
void ClearSequenceContainer(ContainerType &container)
{
	ClearContainer(container, DeleteObject());
}

///@brief map과 같은 stl container가 value(second)의 타입으로 T*일때
///컨테이너의 모든 포인터를 해제함과 컨테이너를 비운다
template<typename ContainerType>
void ClearAssocContainer(ContainerType &container)
{
	ClearContainer(container, DeleteObjectPair());
}

///@brief map중에 key=string인 경우, string목록을 구성하는데 사용하는 함수
template<typename _Tsrc, typename _Tdst>
void GetDictKeyList(const _Tsrc &srcDict, _Tdst &dstList) 
{
	typename _Tsrc::const_iterator it = srcDict.begin();
	typename _Tsrc::const_iterator endit = srcDict.end();
	for( ; it != endit ; it++) {
		dstList.push_back(it->first);
	}
}

///@brief list에 있는 포인터가 assoc container의 second와 동일한 경우, assoc container에서 지우기
///request가 assoc의 second라서 전부 뒤져야되서 속도는 그렇게 좋지 않다
///request container은 지우고자 하는 포인터가 있는 컨테이네
///target assoc container은 지워질 대상. second가 ptr인 경우 지운다
template<typename _TtargetContainer, typename _TfromAssocContainer>
void EraseSecondPtrFromAssocContainer(_TtargetContainer &target, _TfromAssocContainer &from)
{
	typedef std::vector< typename _TfromAssocContainer::key_type > KeyListType;

	//지워질 목록의 key list를 얻기
	KeyListType keylist;

	typename _TtargetContainer::iterator it = target.begin();
	typename _TtargetContainer::iterator endit = target.end();
	for( ; it != endit ; it++) {
		typename _TfromAssocContainer::iterator fromIt = from.begin();
		typename _TfromAssocContainer::iterator fromEndIt = from.end();
		for( ; fromIt != fromEndIt ; fromIt++) {
			if(fromIt->second == *it) {
				keylist.push_back(fromIt->first);
			}
		}
	}

	//key list에 해당하는 항목을 erase
	{
		typename KeyListType::iterator it = keylist.begin();
		typename KeyListType::iterator endit = keylist.end();
		for( ; it != endit ; it++) {
			typename _TfromAssocContainer::key_type &k = *it;
			typename _TfromAssocContainer::iterator found = from.find(k);
			from.erase(found);
		}
	}
	ClearSequenceContainer(target);
}

///@brief 해당 키를 from assoc container에서 없애는 동시에 second를 해제
template<typename _TkeyContainer, typename _TfromAssocContainer>
void EraseKeyFromAssocContainer(_TkeyContainer &keyList, _TfromAssocContainer &from)
{
	typename _TkeyContainer::iterator it = keyList.begin();
	typename _TkeyContainer::iterator endit = keyList.end();
	for( ; it != endit ; it++) {
		typename _TkeyContainer::value_type &k = *it;
		typename _TfromAssocContainer::iterator found = from.find(k);
		if(found != from.end()) {
			delete(found->second);
			from.erase(found);
		}
	}
	keyList.clear();
}

///@brief list에 있는 포인터가 seq container의 포인터와 동일한 경우, seq container에서 지우기
template<typename _TtargetContainer, typename _TfromSeqContainer>
void ErasePtrFromSeqContainer(_TtargetContainer &target, _TfromSeqContainer &from) 
{	
	typename _TtargetContainer::iterator it = target.begin();
	typename _TtargetContainer::iterator endit = target.end();

	_TfromSeqContainer tmp(from);

	typename _TfromSeqContainer::iterator tmpEndit = tmp.end();
	for( ; it != endit ; it++) {
		tmpEndit = std::remove(tmp.begin(), tmpEndit, *it);
	}

	//from.begin~fromEndIt의 내용으로 from을 채운다
	//는 fromEndit~from.end까지의 내용을 버린다와 동일하다
	int resultSize = std::distance(tmp.begin(), tmpEndit);
	from.resize(resultSize);
	std::copy(tmp.begin(), tmpEndit, from.begin());

	//메모리 파괴는 아래에서 적절히
	ClearSequenceContainer(target);
}

template<typename T>
void SafeDelete(T* &ptr)
{
	if(ptr != NULL) {
		delete(ptr);
		ptr = NULL;
	}
}

template<typename T>
void SafeDeleteArray(T* &ptr)
{
	if(ptr != NULL) {
		delete[](ptr);
		ptr = NULL;	
	}
}

template<typename T>
void SafeDeleteWithNullCheck(T* &ptr)
{
	assert(ptr != NULL && "null지우기는 언어상은 지원하나 아마도 잘못 짜여져있을 가능성이 있다");
	delete(ptr);
	ptr = NULL;
}

template<typename T>
void SafeDeleteArrayWithNullCheck(T* &ptr)
{
	assert(ptr != NULL && "null지우기는 언어상은 지원하나 아마도 잘못 짜여져있을 가능성이 있다");
	delete[](ptr);
	ptr = NULL;
}

template<typename T>
void CallConstructor(T *ptr)
{
	new(ptr) T;
}
 
template<typename T>
void CallDestructor(T *ptr)
{
	ptr->~T();
}
}