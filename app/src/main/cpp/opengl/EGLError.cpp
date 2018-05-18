//
// Created by jay on 18-5-8.
//

#include <EGL/egl.h>
#include "EGLError.h"
#include "../common.h"

void EGLError::logError(EGLint errorCode) {
    logError(errorCode, NULL);
}

void EGLError::logError(EGLint errorCode, const char *msg) {
    if (errorCode == EGL_SUCCESS)
        return;
    const char *headMsg = msg == NULL ? "" : msg;
    const char *tag = "EGL error";
    switch (errorCode) {
        case EGL_BAD_MATCH:
            LOG_WF(tag, "%s bad match", headMsg);
            break;
        case EGL_BAD_CONFIG:
            LOG_WF(tag, "%s bad config", headMsg);
            break;
        case EGL_BAD_NATIVE_WINDOW:
            LOG_WF(tag, "%s bad native window", headMsg);
            break;
        case EGL_BAD_ALLOC:
            LOG_WF(tag, "%s bad alloc", headMsg);
            break;
        case EGL_BAD_SURFACE:
            LOG_WF(tag, "%s bad surface", headMsg);
            break;
        case EGL_BAD_CURRENT_SURFACE:
            LOG_WF(tag, "%s bad current surface", headMsg);
            break;
        case EGL_BAD_ACCESS:
            LOG_WF(tag, "%s bad access", headMsg);
            break;
        case EGL_BAD_ATTRIBUTE:
            LOG_WF(tag, "%s bad attribute", headMsg);
            break;
        case EGL_BAD_CONTEXT:
            LOG_WF(tag, "%s bad context", headMsg);
            break;
        case EGL_BAD_DISPLAY:
            LOG_WF(tag, "%s bad display", headMsg);
            break;
        case EGL_BAD_PARAMETER:
            LOG_WF(tag, "%s bad parameter", headMsg);
            break;
        case EGL_BAD_NATIVE_PIXMAP:
            LOG_WF(tag, "%s bad native pixmap", headMsg);
            break;
        default:
            LOG_WF(tag, "%s bad unknown %d", headMsg, errorCode);
            break;

    }
}

void EGLError::checkEGLError(const char *msg) {
    EGLint error = eglGetError();
    if (error != 0) {
        logError(error, msg);
    }

}
