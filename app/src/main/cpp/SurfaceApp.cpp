//
// Created by jay on 18-5-7.
//
#include <jni.h>
#include "Surface.h"


extern "C"
JNIEXPORT void JNICALL
Java_com_zqautomotive_surfacenativedemo_SurfaceApp_initNativeEngine(JNIEnv *env, jobject instance) {

    if (!g_surface) {
        g_surface = new Surface();
    }

}