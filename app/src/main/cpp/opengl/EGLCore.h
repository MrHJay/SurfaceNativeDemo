//
// Created by jay on 18-5-8.
//

#ifndef SURFACENATIVEDEMO_EGLCORE_H
#define SURFACENATIVEDEMO_EGLCORE_H


#include <jni.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

namespace egl {

    class EGLCore {
    public:

        EGLCore(EGLContext shareToContext);

        ~EGLCore();

        EGLSurface createWindowSurface(ANativeWindow *nativeWindow);

        EGLSurface createOffscreenSurface(GLint width, GLint height);

        void resize(GLsizei width, GLsizei height);

        void detachSurface(EGLSurface eglSurface);

        void makeCurrent(EGLSurface drawSurface, EGLSurface readSurface);

        void makeCurrentNone();

        void swapBuffer(EGLSurface eglSurface);

    private:

        void getConfig(EGLConfig *configs, int size);

        EGLContext mContext;
        EGLDisplay mDisplay;
        EGLConfig mConfig;
    };

    static EGLint *getContextAttributesList() {
        static EGLint contextAttrList[] = {
                EGL_CONTEXT_CLIENT_VERSION, 2,
                EGL_NONE
        };
        return contextAttrList;
    }

    static EGLint *getConfigAttributesListRGB8() {
        static EGLint attr_list[] = {
                EGL_RED_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_ALPHA_SIZE, 0,
                EGL_STENCIL_SIZE, 0,
                EGL_DEPTH_SIZE, 16,
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                EGL_SURFACE_TYPE, EGL_PBUFFER_BIT | EGL_WINDOW_BIT,
                EGL_NONE
        };
        return attr_list;
    }

    static EGLint *getConfigAttributesListR5G6B5() {
        static EGLint attr_list[] = {
                EGL_RED_SIZE, 5,
                EGL_GREEN_SIZE, 6,
                EGL_BLUE_SIZE, 5,
                EGL_STENCIL_SIZE, 0,
                EGL_DEPTH_SIZE, 16,
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                EGL_SURFACE_TYPE, EGL_PBUFFER_BIT | EGL_WINDOW_BIT,
                EGL_NONE
        };
        return attr_list;
    }

}
#endif //SURFACENATIVEDEMO_EGLCORE_H
