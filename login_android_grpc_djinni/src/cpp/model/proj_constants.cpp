//
// Created by melon on 2019/2/3.
//
#include <string>
#include "proj_constants.h"

namespace project_constants{

    const int ResultCode::SUCCESS = 0;
    const int ResultCode::LOGIN_FAIL_ACCOUNT_INVALID = 10000;
    const int ResultCode::LOGIN_FAIL_PASSWORD_ERROR = 10001;
    const int ResultCode::LOGIN_FAIL_ACCOUNT_NO_EXIST = 10002;
    const int ResultCode::SIGN_FAIL_ALREADY_EXIST = 10003;
    const int ResultCode::LOGIN_FAIL_PASSWORD_INVALID = 10004;
    const int ResultCode::USER_IS_NOT_ONLINE = 10005;
    const int ResultCode::DEVICE_OFFLINE = 2005;


    const std::string MsgTip::TOAST_ERROR_ACCOUNT_EMPTY = "账号不能为空，请重新输入";
    const std::string MsgTip::TOAST_ERROR_PASSWORD_EMPTY = "密码不能为空，请重新输入";
    const std::string MsgTip::TOAST_ACCOUNT_OUT_OFF_LINE = "您的账号在另一设备登录，如非本人，请重新登录";


    const std::string Constants::USER_ACCOUNT = "user_account";
    const std::string Constants::TOKEN = "token";
    const std::string Constants::IS_CONNECT = "is_connect";


}
