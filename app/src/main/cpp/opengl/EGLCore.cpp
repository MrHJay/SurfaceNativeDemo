//
// Created by jay on 18-5-8.
//

#include <android/native_window.h>
#include <algorithm>
#include "EGLCore.h"
#include "../common.h"
#include "ConfigComparator.h"
#include "EGLError.h"

namespace egl {

    const char *TAG_CORE = "EGLCore";
    const int MAX_EGL_CONFIG_COUNT = 40;

    EGLCore::EGLCore(EGLContext shareToContext) : mContext(EGL_NO_CONTEXT),
                                                  mDisplay(EGL_NO_DISPLAY),
                                                  mConfig(NULL) {
        mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (mDisplay == EGL_NO_DISPLAY) {
            LOG_D(TAG_CORE, "get display failed");
            return;
        }

        EGLint majorVersion;
        EGLint minorVersion;
        if (!eglInitialize(mDisplay, &majorVersion, &minorVersion)) {
            LOG_W(TAG_CORE, "initialize failed");
            return;
        }

        LOG_DF(TAG_CORE, "init finished, major version=%d, minor version=%d", majorVersion,
               minorVersion);

        EGLContext sharedContext = (shareToContext == NULL) ? EGL_NO_CONTEXT
                                                            : shareToContext;
        EGLConfig configs[MAX_EGL_CONFIG_COUNT];
        getConfig(configs, MAX_EGL_CONFIG_COUNT);
        mConfig = configs[0];
        mContext = eglCreateContext(mDisplay, mConfig, sharedContext,
                                    getContextAttributesList());
        if (!mContext) {
            LOG_W(TAG_CORE, "GL create context failed");
        }
    }

    EGLCore::~EGLCore() {
        if (mDisplay && mContext && !eglDestroyContext(mDisplay, mContext)) {
            LOG_W(TAG_CORE, "destroy context error");
        }
    }

    EGLSurface EGLCore::createWindowSurface(ANativeWindow *nativeWindow) {
        EGLSurface eglSurface = EGL_NO_SURFACE;
        if (!nativeWindow) {
            LOG_W(TAG_CORE, "create native window surface failed");
            return eglSurface;
        }

        EGLint format;
        eglGetConfigAttrib(mDisplay, mConfig, EGL_NATIVE_VISUAL_ID, &format);
        ANativeWindow_setBuffersGeometry(nativeWindow, 0, 0, format);

        EGLint surfaceAttributes[] = {EGL_RENDER_BUFFER, EGL_BACK_BUFFER, EGL_NONE};
        eglSurface = eglCreateWindowSurface(mDisplay, mConfig, nativeWindow, surfaceAttributes);

        if (eglSurface == EGL_NO_SURFACE) {
            LOG_E(TAG_CORE, "current EGL config not suitable for create window surface");

            EGLConfig configs[MAX_EGL_CONFIG_COUNT];
            getConfig(configs, MAX_EGL_CONFIG_COUNT);
            for (int i = 0; i < MAX_EGL_CONFIG_COUNT; ++i) {
                EGLConfig currentConfig = configs[i];

                eglGetConfigAttrib(mDisplay, currentConfig, EGL_NATIVE_VISUAL_ID, &format);
                ANativeWindow_setBuffersGeometry(nativeWindow, 0, 0, format);

                eglSurface = eglCreateWindowSurface(mDisplay, currentConfig, nativeWindow,
                                                    surfaceAttributes);
                if (eglSurface == EGL_NO_SURFACE)
                    continue;
                else
                    mConfig = currentConfig;
                break;
            }
        }

        if (eglSurface == EGL_NO_SURFACE) {
            EGLError::logError(eglGetError());
        }

        return eglSurface;
    }

    EGLSurface EGLCore::createOffscreenSurface(GLint width, GLint height) {
        EGLint attr_list[] = {
                EGL_WIDTH, width,
                EGL_HEIGHT, height,
                EGL_NONE
        };
        EGLSurface surface = eglCreatePbufferSurface(mDisplay, mConfig, attr_list);
        return surface;
    }

    void EGLCore::resize(GLsizei width, GLsizei height) {
        glViewport(0, 0, width, height);
    }

    void EGLCore::detachSurface(EGLSurface eglSurface) {
        if (mDisplay && eglSurface && !eglDestroySurface(mDisplay, eglSurface)) {
            EGLError::logError(eglGetError());
        }
    }

    void EGLCore::makeCurrent(EGLSurface drawSurface, EGLSurface readSurface) {
        if (!eglMakeCurrent(mDisplay, drawSurface, readSurface, mContext)) {
            EGLError::logError(eglGetError());
        }
    }

    void EGLCore::makeCurrentNone() {
        if (!eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, mContext)) {
            EGLError::logError(eglGetError());
        }
    }

    void EGLCore::swapBuffer(EGLSurface eglSurface) {
        if (eglSwapBuffers(mDisplay, eglSurface)) {
            EGLError::logError(eglGetError());
        }
    }

    void EGLCore::getConfig(EGLConfig *configs, int size) {

        int count = 0;
        if (eglChooseConfig(mDisplay, getConfigAttributesListRGB8(), configs, size,
                            &count) != EGL_TRUE) {
            if (eglChooseConfig(mDisplay, getConfigAttributesListR5G6B5(), configs, size,
                                &count) == EGL_TRUE) {
                LOG_D(TAG_CORE, ("Backbuffer format: R5G6B5"));
            }
        } else {
            LOG_D(TAG_CORE, ("Backbuffer format: RGB8"));
        }

        std::sort(&configs[0], &configs[count], ConfigComparator(mDisplay));
    }

}
