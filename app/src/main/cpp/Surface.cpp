//
// Created by jay on 18-5-4.
//
#include <android/native_window_jni.h>
#include <android/log.h>
#include "Surface.h"
#include "common.h"
#include "opengl/ConfigComparator.h"
#include <algorithm>
#include <opengl/ZoomDrawable2D.h>
#include <opengl/Font.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

Surface *g_surface = 0;
const char *TAG = "NativeDemoTag";

Surface::Surface()
        : textureId(0) {
    g_surface = this;
}

Surface::~Surface() {

    if (mEglCore) {
        if (mDisplaySurface) {
            delete mDisplaySurface;
        }
        delete mEglCore;
    }
    if (frameRect) {
        delete frameRect;
    }
}

void Surface::attachSurface(JNIEnv *jniEnv, jobject jSurface, jobject assetManager) {

    mEglCore = new egl::EGLCore(NULL);

    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(jniEnv, jSurface);
    if (!nativeWindow) {
        LOG_W(TAG, "create native window failed");
        return;
    }

    mDisplaySurface = new WindowSurface(mEglCore);
    mDisplaySurface->createWindowSurface(nativeWindow);

    mDisplaySurface->makeCurrent();

    AAssetManager *manager = AAssetManager_fromJava(jniEnv, assetManager);
    AAsset *fontFile = AAssetManager_open(manager, "test_font.ttf", AASSET_MODE_BUFFER);
    off_t fontDataSize = AAsset_getLength(fontFile);

    FT_Byte *fontData = new FT_Byte[fontDataSize];
    AAsset_read(fontFile, fontData, (size_t) fontDataSize);
    AAsset_close(fontFile);
    egl::Font *font = new egl::NumberFont();
    font->init(fontData, fontDataSize);

    egl::Texture2DProgram *texture2DProgram = new egl::Texture2DProgram();
    egl::ZoomDrawable2D *drawable2D = new egl::ZoomDrawable2D(egl::Prefab::FULL_RECT);
    egl::FontTextureProgram *fontTexture = new egl::FontTextureProgram(font);
    fontTexture->setColor(0xFFCCCCCC);
    fontTexture->setTextSize(64);
    frameRect = new egl::FrameRect(texture2DProgram, fontTexture, drawable2D);

}

void Surface::resize(jint width, jint height) {
    mViewWidth = width;
    mViewHeight = height;
    glViewport(0, 0, mViewWidth, mViewHeight);
    if (frameRect) {
        frameRect->projection(mViewWidth, mViewHeight);
    }
    LOG_DF(TAG, "onResize h=%d, w=%d", height, width);
}

void Surface::detachSurface() {

    mEglCore->makeCurrentNone();
    if (mDisplaySurface) {
        delete mDisplaySurface;
    }
    if (frameRect) {
        delete frameRect;
    }

}

void Surface::drawColor() {
    LOG_D(TAG, "DrawColor");
    glClearColor(1.0f, 0.0f, 0.0f, 0.5f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    eglSwapBuffers(mDisplay, mSurface);
    mDisplaySurface->swapBuffer();

}

void Surface::changeColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
//    glClearColor(red, green, blue, alpha);
//    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

//    eglSwapBuffers(mDisplay, mSurface);
//    mDrawContext->makeCurrent();

    glDeleteTextures(1, &textureId);
}


GLuint Surface::createTexture() {
    if (textureId) {
        glDeleteTextures(1, &textureId);
    }
    if (frameRect) {
        textureId = frameRect->createTexture();
    }
    return textureId;
}

void Surface::drawFrame(float *matrix, long timestamp) {
    mDisplaySurface->makeCurrent();

    glClearColor(0.3f, 0.3f, 0.3f, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT);

    frameRect->drawFrame(textureId, matrix, timestamp / 1000);
    mDisplaySurface->swapBuffer();
}

void Surface::zoom(float x, float y, float scale) {
    if (frameRect) {
        frameRect->zoom(x / mViewWidth, y / mViewHeight, scale);
    }
}

void Surface::resetZoom() {
    if (frameRect) {
        frameRect->resetScale();
    }
}
