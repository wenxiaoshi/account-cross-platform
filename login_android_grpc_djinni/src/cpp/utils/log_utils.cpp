//
// Created by dw on 19-3-4.
//
#include <android/log.h>

#include "log_utils.h"

#define TAG    "com.wechat.mylogin"
#define A_LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
#define A_LOGI(...)  __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
#define A_LOGW(...)  __android_log_print(ANDROID_LOG_WARN,TAG,__VA_ARGS__)
#define A_LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)
#define A_LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,TAG,__VA_ARGS__)

using namespace utils;

void LogUtil::LOGD(string msg){
    A_LOGD("%s",msg.c_str());
}

void LogUtil::LOGE(string msg){
    A_LOGE("%s",msg.c_str());
}

void LogUtil::LOGI(string msg){
    A_LOGI("%s",msg.c_str());
}

void LogUtil::LOGW(string msg){
    A_LOGW("%s",msg.c_str());
}