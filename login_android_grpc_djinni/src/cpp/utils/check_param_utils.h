//
// Created by melon on 2019/2/25.
//

#ifndef ANDROID_CHECK_PARAM_UTILS_H
#define ANDROID_CHECK_PARAM_UTILS_H

#endif //ANDROID_CHECK_PARAM_UTILS_H

#include "string"

namespace utils{

    class CheckParam{
    public:
        static bool checkAccountValid(std::string account,std::string & errorMsg);
        static bool checkPasswordValid(std::string password,std::string & errorMsg);
    };

}