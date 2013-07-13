// Ŭnicode please
#pragma once

#include "arch.h"

//assert
#include <cassert>
//#if RELEASE
//#define SR_ASSERT(STATMENT) do { (void)sizeof(STATMENT); } while(0)
//#else
#if SR_IPHONE
#define SR_ASSERT(STATMENT) do { assert(STATMENT); } while(0)
#elif SR_WIN
//#if USE_GTEST
//ref gpg 1
//#define SR_ASSERT(STATMENT) do { assert(STATMENT); } while(0)
//extern bool CustomAssertFunction(bool, char*, int, char*);
//#define SR_ASSERT(STATMENT) \
//if(CustomAssertFunction((int)(STATMENT), "assert occur", __LINE__, __FILE__))	{ _asm { int 3 } }	///debugger stop
//내장 assert를 쓰지 못한 이유는 google test가 assert의 함수를 바꿔치기하는데 이것때문에 assert에 걸려도
//프로그램이 멈추지않고 그냥 죽어버린다. 그래서 강제로 죽이기 위해서 도입
#ifndef SR_ASSERT
#define SR_ASSERT(STATMENT) if(!(STATMENT)) { _asm { int 3} }
#endif

//#else
//#define SR_ASSERT(STATMENT) do { assert(STATMENT); } while(0)
//#endif
#elif SR_ANDROID
#include <cstdlib>
#include <android/log.h>
//{ if((!"is visible?")) {} else { LOGE("%s:%d", __FILE__, __LINE__); exit(-1); } }
#define SR_ASSERT(STATMENT)	\
		{ if(!(STATMENT)) { __android_log_print(ANDROID_LOG_ERROR, "ASSERT FAILED", "(%s:%d): `%s`", __FILE__, __LINE__, #STATMENT); exit(-1); } }
#endif
//#endif

//강제로 실행 멈추기. assert같은 느낌
#if SR_IPHONE || SR_ANDROID
#define SR_BREAK_FORCE() do { assert(false && __FILE__&& __LINE__); } while(0)
#elif SR_WIN
#define SR_BREAK_FORCE() { _asm { int 3} }
#else
#error "not support"
#endif

//옛날 코드 가져온거라서 IUASSERT로 되어있는 부분이 많다. ASSERT와 동급으로 처리
#define IUASSERT SR_ASSERT
