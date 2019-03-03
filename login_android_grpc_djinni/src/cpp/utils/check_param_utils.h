//
// Created by melon on 2019/2/25.
//

#ifndef ANDROID_CHECK_PARAM_UTILS_H
#define ANDROID_CHECK_PARAM_UTILS_H

#endif //ANDROID_CHECK_PARAM_UTILS_H

#include "string"

using namespace std;

namespace utils{

    class ParamUtils{
    public:
        static bool checkAccountValid(string account,string & errorMsg);
        static bool checkPasswordValid(string password,string & errorMsg);

    private:
        static bool patternMatch(string pattern, string source_str);
    };

}