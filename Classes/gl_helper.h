// Ŭnicode please
#pragma once

#include <string>
#include "platform/CCPlatformMacros.h"

namespace sora {;
class GLHelper {
public:
    static bool CheckFrameBufferStatus(const char *name);
    static bool CheckFrameBufferStatus(const std::string &name) {
    	return CheckFrameBufferStatus(name.c_str());
    }
    static bool CheckError(const char *name);
    static bool CheckError(const std::string &name) {
        return CheckError(name.c_str());
    }
};
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#ifdef DEBUG
#define CHECK_GLES_ERROR(NAME)	{ cocos2d::GLHelper::CheckError(NAME); }
#else
#define CHECK_GLES_ERROR(NAME)
#endif
#else
#if _DEBUG
#define CHECK_GLES_ERROR(NAME)	{ cocos2d::GLHelper::CheckError(NAME); }
#else
#define CHECK_GLES_ERROR(NAME)
#endif
#endif