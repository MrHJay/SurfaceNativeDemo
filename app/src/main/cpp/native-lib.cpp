#include <jni.h>
#include "Surface.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_zqautomotive_surfacenativedemo_MainActivity_nativeSurfaceCreated(JNIEnv *env,
                                                                          jobject instance,
                                                                          jobject surface,
                                                                          jobject assetManager) {
    if (g_surface) {
        g_surface->attachSurface(env, surface, assetManager);
//        g_surface->drawColor();
    }

}

extern "C"
JNIEXPORT void JNICALL
Java_com_zqautomotive_surfacenativedemo_MainActivity_nativeSurfaceChanged(JNIEnv *env,
                                                                          jobject instance,
                                                                          jint width,
                                                                          jint height) {

    if (g_surface) {
        g_surface->resize(width, height);
    }

}

extern "C"
JNIEXPORT void JNICALL
Java_com_zqautomotive_surfacenativedemo_MainActivity_nativeSurfaceDestroyed(JNIEnv *env,
                                                                            jobject instance,
                                                                            jobject surface) {

    if (g_surface) {
        g_surface->detachSurface();
    }

}
extern "C"
JNIEXPORT void JNICALL
Java_com_zqautomotive_surfacenativedemo_MainActivity_changeColor(JNIEnv *env, jobject instance) {

    if (g_surface) {
        g_surface->changeColor(0.0, 1.0, 0.0, 0.5);
    }

}

extern "C"
JNIEXPORT jint JNICALL
Java_com_zqautomotive_surfacenativedemo_MainActivity_nativeCreateTexture(JNIEnv *env,
                                                                         jobject instance) {

    if (g_surface) {
        return g_surface->createTexture();
    }
    return 0;

}

extern "C"
JNIEXPORT void JNICALL
Java_com_zqautomotive_surfacenativedemo_MainActivity_drawFrame(JNIEnv *env, jobject instance,
                                                               jfloatArray matrix_,
                                                               jlong timestamp) {
    jfloat *matrix = env->GetFloatArrayElements(matrix_, NULL);

    if (g_surface) {
        g_surface->drawFrame(matrix, timestamp);
    }


    env->ReleaseFloatArrayElements(matrix_, matrix, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zqautomotive_surfacenativedemo_MainActivity_zoom(JNIEnv *env, jobject instance, jfloat x,
                                                          jfloat y, jfloat scale) {

    if (g_surface) {
        g_surface->zoom(x, y, scale);
    }

}

extern "C"
JNIEXPORT void JNICALL
Java_com_zqautomotive_surfacenativedemo_MainActivity_reset(JNIEnv *env, jobject instance) {

    if (g_surface) {
        g_surface->resetZoom();
    }

}