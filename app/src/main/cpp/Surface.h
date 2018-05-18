//
// Created by jay on 18-5-4.
//
#pragma once
#ifndef SURFACENATIVEDEMO_SURFACE_H
#define SURFACENATIVEDEMO_SURFACE_H


#include <jni.h>
#include <GLES2/gl2.h>

#include <opengl/FrameRect.h>
#include <opengl/FontTextureProgram.h>
#include "AndroidOGLContext.h"
#include "opengl/Texture2DProgram.h"

class Surface {

public:
    Surface();

    ~Surface();

    void attachSurface(JNIEnv *jniEnv, jobject jSurface);

    void resize(jint width, jint height);

    void zoom(float x, float y, float scale);

    void resetZoom();

    void detachSurface();

    void drawColor();

    void changeColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

    GLuint createTexture();

    void drawFrame(float *matrix);

private:
    bool createNativeWindow();

    AndroidOGLContext *m_drawContext;
    ANativeWindow *m_nativeWindow;

    EGLDisplay m_display;
    EGLSurface m_surface;
    EGLConfig m_config;

    int mViewWidth, mViewHeight;

    GLuint textureId;
    egl::FrameRect *frameRect;
    egl::Drawable2D *drawable2D;
    egl:: FontTextureProgram * fontTexture;

};

extern Surface *g_surface;
#endif //SURFACENATIVEDEMO_SURFACE_H
