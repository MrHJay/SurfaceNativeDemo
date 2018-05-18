//
// Created by jay on 18-5-7.
//

#include <GLES2/gl2.h>
#include "common.h"
#include "AndroidOGLContext.h"

const char *TAG_ = "NativeDemoTag";

static EGLint *getContextAttributesList() {
    static EGLint contextAttrList[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
    };
    return contextAttrList;
}

AndroidOGLContext::AndroidOGLContext(EGLDisplay display, EGLSurface surface, EGLConfig config,
                                     AndroidOGLContext *shareToContext)
        : m_display(display), m_surface(surface) {

    EGLContext sharedContext = (shareToContext == NULL) ? EGL_NO_CONTEXT
                                                        : shareToContext->m_context;
    m_context = eglCreateContext(m_display, config, sharedContext, getContextAttributesList());
    if (!m_context) {
        LOG_W(TAG_, "GL create context failed");
    }
}

AndroidOGLContext::~AndroidOGLContext() {

    if (!eglDestroyContext(m_display, m_context)) {
        LOG_W(TAG_, "destroy context error");
    }
}

void AndroidOGLContext::setDefaultFrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void AndroidOGLContext::resetSurface() {
    m_surface = EGL_NO_SURFACE;
}

void AndroidOGLContext::setSurface(EGLSurface surface) {
    m_surface = surface;
}

void AndroidOGLContext::makeCurrent() {
    if (!eglMakeCurrent(m_display, m_surface, m_surface, m_context)) {
        GLint error = eglGetError();
        switch (error) {
            case EGL_BAD_CURRENT_SURFACE:
                LOG_W(TAG_, "GL make current bad current");
                break;
            default:
                LOG_WF(TAG_, "GL make current failed %d", error);

        }
    }
}
