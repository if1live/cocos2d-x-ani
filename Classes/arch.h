// Ŭnicode please
#pragma once

//platform check macro
//cocos2d의 내장 기능을 써도 되는데 그것을 쓰면 프로젝트자체에서 Macro 설정을 해야되서
//귀찮아 -_- 그래서 독립 파일로 인클루드해도 돌아가도록 만들엇다
#ifdef __APPLE__
//iphone check
//#import <Availability.h>
//#if __IPHONE_2_0
#define SR_IPHONE 1
//#endif
#else
#define SR_IPHONE 0
#endif

#if (defined _WIN32) || (defined _W64)
#define SR_WIN 1
#else
#define SR_WIN 0
#endif

//android check
#if (SR_IPHONE == 0) && (SR_WIN == 0)
#define SR_ANDROID 1
#else
#define SR_ANDROID 0
#endif

//pch
#if SR_WIN || SR_IPHONE
#define SR_USE_PCH 1
#else
#define SR_USE_PCH 0
#endif

#ifdef __cplusplus
#define SR_NS_BEGIN(NAMESPACE)     namespace NAMESPACE {;
#define SR_NS_END(NAMESPACE)       }
#else
#define SR_NS_BEGIN(NS)     
#define SR_NS_END(NS)       
#endif 
