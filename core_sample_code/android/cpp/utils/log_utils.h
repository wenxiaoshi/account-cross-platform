//
// Created by dw on 19-3-4.
//

#ifndef ANDROID_LOG_UTILS_H
#define ANDROID_LOG_UTILS_H

#endif //ANDROID_LOG_UTILS_H

#include <string>

using namespace std;

namespace utils{

    class LogUtil{
    public:
        
        /*
         主要功能：
         写信息到控制台
         
         出口参数
         msg：               打印信息
         */
        static void LOGD(string msg);
        static void LOGE(string msg);
        static void LOGI(string msg);
        static void LOGW(string msg);
    private:
    };

}
