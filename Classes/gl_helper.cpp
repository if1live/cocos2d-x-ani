// Ŭnicode please
#include "stdafx.h"
#include "gl_helper.h"
#include "platform/CCCommon.h"
#include "platform/CCPlatformMacros.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "platform/win32/CCGL.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "platform/ios/CCGL.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/CCGL.h"
#else
#error "unknown"
#endif

namespace sora {;
bool GLHelper::CheckFrameBufferStatus(const char *name) {
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status == GL_FRAMEBUFFER_COMPLETE) {
        return true;
    }
    //else,,
    const char *error_msg = NULL;
    switch(status) {
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        error_msg = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
        break;
#ifdef GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
        error_msg = "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS";
        break;
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
        error_msg = "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT";
        break;
#endif
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        error_msg = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
        break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
        error_msg = "GL_FRAMEBUFFER_UNSUPPORTED";
        break;

#ifdef GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        error_msg = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
        break;
#endif
    default:
        CheckError(name);
        error_msg = "Unknown Error";
        break;  
    }
    CCLOG("FrameBuffer Error [%s] : %s", name, error_msg);

    return false;
}

bool GLHelper::CheckError(const char *name) {
    int error = glGetError();
    if (error != GL_NO_ERROR) {
        const char *error_msg;
        switch (error) {
        case GL_INVALID_ENUM:
            error_msg = "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error_msg = "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            error_msg = "GL_INVALID_OPERATION";
            break;
            //case GL_INVALID_FRAMEBUFFER_OPERATION:
            //	error_msg = "GL_INVALID_FRAMEBUFFER_OPERATION";
            //	break;
        case GL_OUT_OF_MEMORY:
            error_msg = "GL_OUT_OF_MEMORY";
            break;
        default:
            error_msg = "UNKNOWN";
            break;
        }
        CCLOG("OpenGL Error [%s] : %s", name, error_msg);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        exit(-1);
#endif
        return false;
    }
    return true;
}
}
