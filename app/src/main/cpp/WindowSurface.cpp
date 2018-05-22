//
// Created by jay on 18-5-21.
//

#include "WindowSurface.h"

EGLSurfaceBase::EGLSurfaceBase(egl::EGLCore *eglCore) : mEglCore(eglCore),
                                                        mSurface(EGL_NO_SURFACE),
                                                        mWidth(0), mHeight(0) {
}

EGLSurfaceBase::~EGLSurfaceBase() {
    if (mSurface != EGL_NO_SURFACE) {
        mEglCore->detachSurface(mSurface);
    }
}

void EGLSurfaceBase::createWindowSurface(ANativeWindow *nativeWindow) {
    if (mSurface!=EGL_NO_SURFACE) {
        mEglCore->detachSurface(mEglCore);
    }

    mSurface = mEglCore->createWindowSurface(nativeWindow);
}

void EGLSurfaceBase::createOffscreenSurface(int width, int height) {
    if (mSurface!=EGL_NO_SURFACE) {
        mEglCore->detachSurface(mEglCore);
    }

    mSurface = mEglCore->createOffscreenSurface(width, height);
}

void EGLSurfaceBase::makeCurrent() {
    mEglCore->makeCurrent(mSurface, mSurface);
}

void EGLSurfaceBase::swapBuffer() {
    mEglCore->swapBuffer(mSurface);
}

WindowSurface::WindowSurface(egl::EGLCore *eglCore): EGLSurfaceBase(eglCore) {

}

WindowSurface::~WindowSurface() {

}
