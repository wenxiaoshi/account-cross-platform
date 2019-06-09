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

        /*
        主要功能：
        校验账号是否正确

        入口参数
        account： 	      用户账号
        errorMsg： 	      错误信息

        出口参数
        bool：             true为正确，false为错误
        */
        static bool checkAccountValid(string account,string & errorMsg);

        /*
        主要功能：
        校验密码是否正确

        入口参数
        password： 	      用户密码
        errorMsg： 	      错误信息

        出口参数
        bool：             true为正确，false为错误
        */
        static bool checkPasswordValid(string password,string & errorMsg);

    private:

        /*
        主要功能：
        根据正则字符串匹配数据源是否正确

        入口参数
        pattern： 	      正则字符串
        source_str： 	  数据源

        出口参数
        bool：             true为正确，false为错误
        */
        static bool patternMatch(string pattern, string source_str);
    };

}