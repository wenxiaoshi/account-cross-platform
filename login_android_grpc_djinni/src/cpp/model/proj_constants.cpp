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

    const int ResultCode::DEVICE_OFFLINE = 1004004;


    const std::string MsgTip::TOAST_ERROR_ACCOUNT_EMPTY = "账号不能为空，请重新输入";
    const std::string MsgTip::TOAST_ERROR_ACCOUNT_NOT_VALID_PHONE_NUM = "账号格式错误，请输入正确的手机号码";
    const std::string MsgTip::TOAST_ERROR_PASSWORD_EMPTY = "密码不能为空，请重新输入";
    const std::string MsgTip::TOAST_ERROR_PASSWORD_NOT_VALID = "密码格式错误，请输入6-18位的数字或字母";
    const std::string MsgTip::TOAST_ERROR_NETWORK_UNVALAIBLE = "网络错误，请稍后重试";

    const std::string MsgTip::TOAST_ACCOUNT_OUT_OFF_LINE = "您的账号在另一设备登录，如非本人，请重新登录";


    const std::string Constants::USER_ACCOUNT = "user_account";
    const std::string Constants::TOKEN = "token";
    const std::string Constants::REFRESH_TOKEN = "refresh_token";
    const std::string Constants::IS_CONNECT = "is_connect";


}
