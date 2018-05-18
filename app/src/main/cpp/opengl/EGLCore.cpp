//
// Created by jay on 18-5-8.
//

#include <android/native_window.h>
#include <algorithm>
#include "EGLCore.h"
#include "../common.h"
#include "../ConfigComparator.h"
#include "EGLError.h"

namespace egl {

    const char *TAG_CORE = "EGLCore";

    EGLCore::EGLCore(EGLContext shareToContext) : m_nativeWindow(NULL),
                                                  m_context(EGL_NO_CONTEXT),
                                                  m_display(EGL_NO_DISPLAY),
                                                  m_surface(EGL_NO_SURFACE),
                                                  m_config(NULL) {
        m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (m_display == EGL_NO_DISPLAY) {
            LOG_D(TAG_CORE, "get display failed");
            return;
        }

        EGLint majorVersion;
        EGLint minorVersion;
        if (!eglInitialize(m_display, &majorVersion, &minorVersion)) {
            LOG_W(TAG_CORE, "initialize failed");
            return;
        }

        LOG_DF(TAG_CORE, "init finished, major version=%d, minor version=%d", majorVersion,
               minorVersion);

        EGLContext sharedContext = (shareToContext == NULL) ? EGL_NO_CONTEXT
                                                            : shareToContext;
        m_context = eglCreateContext(m_display, m_config, sharedContext,
                                     getContextAttributesList());
        if (!m_context) {
            LOG_W(TAG_CORE, "GL create context failed");
        }
    }

    EGLCore::~EGLCore() {
        if (m_display && m_context && !eglDestroyContext(m_display, m_context)) {
            LOG_W(TAG_CORE, "destroy context error");
        }
        detachSurface();
    }

    bool EGLCore::createWindowSurface(ANativeWindow *nativeWindow) {
        if (!nativeWindow) {
            LOG_W(TAG_CORE, "create native window failed");
            return false;
        }
        m_nativeWindow = nativeWindow;

        int const kMaxConfigCount = 40;
        EGLConfig configs[kMaxConfigCount];
        int count = 0;
        if (eglChooseConfig(m_display, getConfigAttributesListRGB8(), configs, kMaxConfigCount,
                            &count) != EGL_TRUE) {
            if (eglChooseConfig(m_display, getConfigAttributesListR5G6B5(), configs,
                                kMaxConfigCount,
                                &count) == EGL_TRUE) {
                LOG_D(TAG_CORE, ("Backbuffer format: R5G6B5"));
            }
        } else {
            LOG_D(TAG_CORE, ("Backbuffer format: RGB8"));
        }

        std::sort(&configs[0], &configs[count], ConfigComparator(m_display));
        for (int i = 0; i < count; ++i) {
            EGLConfig currentConfig = configs[i];

            EGLint format;
            eglGetConfigAttrib(m_display, currentConfig, EGL_NATIVE_VISUAL_ID, &format);
            ANativeWindow_setBuffersGeometry(m_nativeWindow, 0, 0, format);

            EGLint surfaceAttributes[] = {EGL_RENDER_BUFFER, EGL_BACK_BUFFER, EGL_NONE};
            m_surface = eglCreateWindowSurface(m_display, currentConfig, m_nativeWindow,
                                               surfaceAttributes);
            if (m_surface == EGL_NO_SURFACE)
                continue;
            else
                m_config = currentConfig;

            break;
        }

        if (m_surface == EGL_NO_SURFACE) {
            EGLError::logError(eglGetError());
            return false;
        }

        return true;

    }

    void EGLCore::resize(GLsizei width, GLsizei height) {
        glViewport(0, 0, width, height);
    }

    void EGLCore::detachSurface() {
        if (m_display && m_surface && !eglDestroySurface(m_display, m_surface)) {
            EGLError::logError(eglGetError());
        }
        m_surface = EGL_NO_SURFACE;
    }

    void EGLCore::makeCurrent() {
        if (!eglMakeCurrent(m_display, m_surface, m_surface, m_context)) {
            EGLError::logError(eglGetError());
        }
    }

    void EGLCore::makeCurrentNone() {
        if (!eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, m_context)) {
            EGLError::logError(eglGetError());
        }
    }

    void EGLCore::swapBuffer() {
        if (eglSwapBuffers(m_display, m_surface)) {
            EGLError::logError(eglGetError());
        }
    }

}
