//
// Created by jay on 18-5-4.
//
#include <android/native_window_jni.h>
#include <android/log.h>
#include "Surface.h"
#include "common.h"
#include "ConfigComparator.h"
#include <algorithm>
#include <opengl/ZoomDrawable2D.h>
#include <opengl/Font.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

Surface *g_surface = 0;
const char *TAG = "NativeDemoTag";

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

Surface::Surface()
        : m_drawContext(NULL), m_display(EGL_NO_DISPLAY), m_surface(EGL_NO_SURFACE),
          m_config(NULL), textureId(0) {
    g_surface = this;
}

Surface::~Surface() {

    if (m_drawContext) {
        delete m_drawContext;
    }
    if (frameRect) {
        delete frameRect;
    }
}

void Surface::attachSurface(JNIEnv *jniEnv, jobject jSurface) {

    m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (m_display == EGL_NO_DISPLAY) {
        LOG_D(TAG, "get display failed");
    }

    EGLint majorVersion;
    EGLint minorVersion;
    if (!eglInitialize(m_display, &majorVersion, &minorVersion)) {
        LOG_D(TAG, "initialize failed");
        return;
    }

    LOG_DF(TAG, "init major version = %d, minor version = %d", majorVersion, minorVersion);

    m_nativeWindow = ANativeWindow_fromSurface(jniEnv, jSurface);
    if (!m_nativeWindow) {
        LOG_W(TAG, "create native window failed");
        return;
    }

    if (!createNativeWindow()) {
        return;
    }

    m_drawContext = new AndroidOGLContext(m_display, m_surface, m_config, NULL);
    if (m_drawContext) {
        m_drawContext->makeCurrent();
//        m_drawContext->setDefaultFrameBuffer();
    }

    egl::Texture2DProgram *texture2DProgram = new egl::Texture2DProgram();
    egl::ZoomDrawable2D *drawable2D = new egl::ZoomDrawable2D(egl::Prefab::FULL_RECT);
    egl::FontTextureProgram *fontTexture = new egl::FontTextureProgram("/sdcard/test_font.ttf");
    fontTexture->setColor(0xFFCCCCCC);
    fontTexture->setTextSize(64);
    frameRect = new egl::FrameRect(texture2DProgram, fontTexture, drawable2D);

//    AAssetManager* assetManager = AAssetManager_fromJava(jniEnv, jSurface);
//    AAsset* fontFile = AAssetManager_open(assetManager, "test_font.ttf", AASSET_MODE_BUFFER);
//    off_t fontDataSize = AAsset_getLength(fontFile);
//
//    FT_Byte* fontData = new FT_Byte[fontDataSize];
//    AAsset_read(fontFile, fontData, (size_t) fontDataSize);
//    AAsset_close(fontFile);


//    glClearDepthf(1.0);
//    glEnable(GL_DEPTH_TEST);
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

    if (m_drawContext) {
        m_drawContext->resetSurface();
    }
    if (frameRect) {
        delete frameRect;
    }

}

void Surface::drawColor() {
    LOG_D(TAG, "DrawColor");
    glClearColor(1.0f, 0.0f, 0.0f, 0.5f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    eglSwapBuffers(m_display, m_surface);

}

void Surface::changeColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
//    glClearColor(red, green, blue, alpha);
//    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

//    eglSwapBuffers(m_display, m_surface);
    m_drawContext->makeCurrent();
}

bool Surface::createNativeWindow() {
    int const kMaxConfigCount = 40;
    EGLConfig configs[kMaxConfigCount];
    int count = 0;
    if (eglChooseConfig(m_display, getConfigAttributesListRGB8(), configs, kMaxConfigCount,
                        &count) != EGL_TRUE) {
        if (eglChooseConfig(m_display, getConfigAttributesListR5G6B5(), configs, kMaxConfigCount,
                            &count) == EGL_TRUE) {
            LOG_D(TAG, ("Backbuffer format: R5G6B5"));
        }
    } else {
        LOG_D(TAG, ("Backbuffer format: RGB8"));
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
        EGLint error = eglGetError();
        switch (error) {
            case EGL_BAD_MATCH:
                LOG_W(TAG, "1 bad match");
                break;
            case EGL_BAD_CONFIG:
                LOG_W(TAG, "2 bad config");
                break;
            case EGL_BAD_NATIVE_WINDOW:
                LOG_W(TAG, "3 bad native window");
                break;
            case EGL_BAD_ALLOC:
                LOG_W(TAG, "4 bad alloc");
                break;
            default:
                LOG_WF(TAG, "5 bad unknown %d", error);
                break;

        }
        return false;
    }

    return true;
}

GLuint Surface::createTexture() {
    if (frameRect) {
        textureId = frameRect->createTexture();
    }
    return textureId;
}

void Surface::drawFrame(float *matrix) {
//    m_drawContext->makeCurrent();

    glClearColor(0.3f, 0.3f, 0.3f, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT);

    frameRect->drawFrame(textureId, matrix);
    eglSwapBuffers(m_display, m_surface);
}

void Surface::zoom(float x, float y, float scale) {
    if (frameRect) {
        frameRect->zoom(x, y, scale);
    }
}

void Surface::resetZoom() {
    if (frameRect) {
        frameRect->resetScale();
    }
}
