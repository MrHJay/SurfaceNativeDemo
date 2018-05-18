//
// Created by jay on 18-5-7.
//
#pragma once

#include <EGL/egl.h>


#ifndef SURFACENATIVEDEMO_ANDROIDOGLCONTEXT_H
#define SURFACENATIVEDEMO_ANDROIDOGLCONTEXT_H


class AndroidOGLContext {

public:
    AndroidOGLContext(EGLDisplay display, EGLSurface surface, EGLConfig config,
                      AndroidOGLContext *shareToContext);

    ~AndroidOGLContext();

    void setDefaultFrameBuffer();

    void makeCurrent();

    void setSurface(EGLSurface surface);

    void resetSurface();

private:
    EGLContext m_context;

    EGLDisplay m_display;
    EGLSurface m_surface;
};


#endif //SURFACENATIVEDEMO_ANDROIDOGLCONTEXT_H
