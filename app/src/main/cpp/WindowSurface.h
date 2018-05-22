//
// Created by jay on 18-5-21.
//

#ifndef SURFACENATIVEDEMO_WINDOWSURFACE_H
#define SURFACENATIVEDEMO_WINDOWSURFACE_H

#include <opengl/EGLCore.h>

class EGLSurfaceBase {

public:
    EGLSurfaceBase(egl::EGLCore *eglCore);

    virtual ~EGLSurfaceBase();

    void createWindowSurface(ANativeWindow *nativeWindow);

    void createOffscreenSurface(int width, int height);

    void makeCurrent();

    void swapBuffer();

private:
    egl::EGLCore *mEglCore;
    EGLSurface mSurface;
    int mWidth, mHeight;
};

class WindowSurface : public EGLSurfaceBase {

public:
    WindowSurface(egl::EGLCore *eglCore);

    ~WindowSurface();

};


#endif //SURFACENATIVEDEMO_WINDOWSURFACE_H
