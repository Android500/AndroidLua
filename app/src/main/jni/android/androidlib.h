//
// Created by Administrator on 2018/1/16.
//

#ifndef ANDROIDLUA_ANDROIDLIB_H
#define ANDROIDLUA_ANDROIDLIB_H

#include <android/log.h>

#define DEV_GRAPHICS  "/dev/graphics/fb0"
#define DEV_UINPUT  "/dev/uinput"

#define TAG    "lua_android_support" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型
#endif //ANDROIDLUA_ANDROIDLIB_H
