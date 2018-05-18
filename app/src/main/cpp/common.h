//
// Created by jay on 18-5-7.
//
#include <android/log.h>

#ifndef SURFACENATIVEDEMO_COMMON_H
#define SURFACENATIVEDEMO_COMMON_H

#ifndef DEBUG
#define DEBUG
#endif

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

template<typename T>
static int count(T& x) {
    return sizeof(x) / sizeof(x[0]);
}

#define LOG_I(TAG, MSG) __android_log_print(ANDROID_LOG_INFO, TAG, MSG)
#define LOG_IF(TAG, MSG, ...) __android_log_print(ANDROID_LOG_INFO, TAG, MSG, __VA_ARGS__)
#ifdef DEBUG
#define LOG_D(TAG, MSG) __android_log_print(ANDROID_LOG_DEBUG, TAG, MSG)
#define LOG_W(TAG, MSG) __android_log_print(ANDROID_LOG_WARN, TAG, MSG)
#define LOG_E(TAG, MSG) __android_log_print(ANDROID_LOG_ERROR, TAG, MSG)

#define LOG_DF(TAG, MSG, ...) __android_log_print(ANDROID_LOG_DEBUG, TAG, MSG, __VA_ARGS__)
#define LOG_WF(TAG, MSG, ...) __android_log_print(ANDROID_LOG_WARN, TAG, MSG, __VA_ARGS__)
#define LOG_EF(TAG, MSG, ...) __android_log_print(ANDROID_LOG_ERROR, TAG, MSG, __VA_ARGS__)
#endif

#endif //SURFACENATIVEDEMO_COMMON_H
