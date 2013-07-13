// Ŭnicode please
#pragma once

#if SR_USE_PCH == 0
#include <algorithm>
#endif

namespace sora {;
///@brief stl container와 지우는 functor를 받아서 컨테이너 내부의 내용을 비우고 포인터도 삭제한다
template<typename ContainerType, typename Deletor>
void ClearContainer(ContainerType &container, Deletor deletor);

///@brief list, vector와 같은 stl container가 T*를 타입을 처리할때
///컨테이너의 모든 포인터를 해제함과 컨테이너를 비운다
template<typename ContainerType>
void ClearSequenceContainer(ContainerType &container);

///@brief map과 같은 stl container가 value(second)의 타입으로 T*일때
///컨테이너의 모든 포인터를 해제함과 컨테이너를 비운다
template<typename ContainerType>
void ClearAssocContainer(ContainerType &container);

///@brief map중에 key=string인 경우, string목록을 구성하는데 사용하는 함수
template<typename _Tsrc, typename _Tdst>
void GetDictKeyList(const _Tsrc &srcDict, _Tdst &dstList);

///@brief list에 있는 포인터가 assoc container의 second와 동일한 경우, assoc container에서 지우기
///request가 assoc의 second라서 전부 뒤져야되서 속도는 그렇게 좋지 않다
///request container은 지우고자 하는 포인터가 있는 컨테이네
///target assoc container은 지워질 대상. second가 ptr인 경우 지운다
template<typename _TtargetContainer, typename _TfromAssocContainer>
void EraseSecondPtrFromAssocContainer(_TtargetContainer &target, _TfromAssocContainer &from);

///@brief 해당 키를 from assoc container에서 없애는 동시에 second를 해제
template<typename _TkeyContainer, typename _TfromAssocContainer>
void EraseKeyFromAssocContainer(_TkeyContainer &keyList, _TfromAssocContainer &from);

///@brief list에 있는 포인터가 seq container의 포인터와 동일한 경우, seq container에서 지우기
template<typename _TtargetContainer, typename _TfromSeqContainer>
void ErasePtrFromSeqContainer(_TtargetContainer &target, _TfromSeqContainer &from);

template<typename T>
void SafeDelete(T* &ptr);

template<typename T>
void SafeDeleteArray(T* &ptr);

//null이 들어올 경우 죽인다
template<typename T>
void SafeDeleteWithNullCheck(T* &ptr);

template<typename T>
void SafeDeleteArrayWithNullCheck(T* &ptr);

template<typename T>
void CallConstructor(T *ptr);
 
template<typename T>
void CallDestructor(T *ptr);

template<typename T, int N>
int GetArraySize(T (&arr)[N]) {	return N; }

}

#include "template_lib.hpp"
