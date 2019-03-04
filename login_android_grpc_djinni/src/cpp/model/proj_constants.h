//
// Created by melon on 2019/2/3.
//

#ifndef ANDROID_PROJ_CONSTANTS_H
#define ANDROID_PROJ_CONSTANTS_H

#endif //ANDROID_PROJ_CONSTANTS_H

#include <string>

namespace project_constants{
    class ResultCode{
    public:
        const static int SUCCESS;
        const static int LOGIN_FAIL_ACCOUNT_INVALID;
        const static int LOGIN_FAIL_PASSWORD_INVALID;
        const static int LOGIN_FAIL_PASSWORD_ERROR;
        const static int LOGIN_FAIL_ACCOUNT_NO_EXIST;
        const static int SIGN_FAIL_ALREADY_EXIST;

        const static int USER_IS_NOT_ONLINE;

        const static int DEVICE_OFFLINE;
    };

    class MsgTip{
    public:
        const static std::string TOAST_ERROR_ACCOUNT_EMPTY;
        const static std::string TOAST_ERROR_ACCOUNT_NOT_VALID_PHONE_NUM;
        const static std::string TOAST_ERROR_PASSWORD_EMPTY;
        const static std::string TOAST_ERROR_PASSWORD_NOT_VALID;
        const static std::string TOAST_ERROR_NETWORK_UNVALAIBLE;
        const static std::string TOAST_ACCOUNT_OUT_OFF_LINE;
    };

    class Constants{
    public:
        const static std::string TOKEN;
        const static std::string IS_CONNECT;
        const static std::string USER_ACCOUNT;
    };

}