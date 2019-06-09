//
// Created by melon on 2019/2/3.
//
#include <string>
#include "proj_constants.h"

using namespace std;

namespace project_constants{

    //客户端
    const string Constants::USER_ACCOUNT = "user_account";
    const string Constants::TOKEN = "token";
    const string Constants::REFRESH_TOKEN = "refresh_token";
    const string Constants::IS_CONNECT = "is_connect";

    //客户端错误码
    const int ClientCode::SUCCESS = 0;
    const int ClientCode::LOGIN_FAIL_ACCOUNT_INVALID = 10000;
    const int ClientCode::LOGIN_FAIL_PASSWORD_ERROR = 10001;
    const int ClientCode::LOGIN_FAIL_ACCOUNT_NO_EXIST = 10002;
    const int ClientCode::SIGN_FAIL_ALREADY_EXIST = 10003;
    const int ClientCode::LOGIN_FAIL_PASSWORD_INVALID = 10004;

    const int ClientCode::USER_IS_NOT_ONLINE = 10005;
    const int ClientCode::USER_IS_OUT_OF_LINE = 10006;
    const int ClientCode::USER_ACCOUNT_FO_EMPTY = 10007;

    //客户端提示
    const string ToastTip::TOAST_ERROR_ACCOUNT_EMPTY = "账号不能为空，请重新输入";
    const string ToastTip::TOAST_ERROR_ACCOUNT_NOT_VALID_PHONE_NUM = "账号格式错误，请输入正确的手机号码";
    const string ToastTip::TOAST_ERROR_PASSWORD_EMPTY = "密码不能为空，请重新输入";
    const string ToastTip::TOAST_ERROR_PASSWORD_NOT_VALID = "密码格式错误，请输入6-18位的数字或字母";
    const string ToastTip::TOAST_ERROR_NETWORK_UNVALAIBLE = "网络错误，请稍后重试";

    const string ToastTip::TOAST_ACCOUNT_OUT_OFF_LINE = "您的账号在另一设备登录，如非本人，请重新登录";
    const string ToastTip::TOAST_ACCOUNT_INFO_EMPTY = "账号信息缺失，请重新登录";



    //服务端
    // 公共错误码（0 ~ 99）
    const int ResultCode::SUCCESS = 0;
    const int ResultCode::ReqParamError = 1;

    const string MsgTip::ERROR_ACCOUNT_EMPTY = "账号不能为空，请重新输入";//1
    const string MsgTip::ERROR_ACCOUNT_NOT_VALID_PHONE_NUM = "账号格式错误，请输入正确的手机号码";//1
    const string MsgTip::ERROR_PASSWORD_EMPTY = "密码不能为空，请重新输入";//1
    const string MsgTip::ERROR_PASSWORD_NOT_VALID = "密码格式错误，请输入6-18位的数字或字母";//1
    const string MsgTip::ERROR_TOKEN_EMPTY = "Token不能为空";//1
    const string MsgTip::ERROR_STRING_EMPTY = "string can not be null or empty";//1

    //业务逻辑错误（1xxxzzz 账号模块，2xxxzzz 卡包模块, ......）

    //1001zzz 用户登录接口
    const int ResultCode::UserLogin_AccountNotExist = 1001001;
    const int ResultCode::UserLogin_PasswordInitFail = 1001002;
    const int ResultCode::UserLogin_PasswordError = 1001003;
    const int ResultCode::UserLogin_UpdateTokenFail = 1001004;

    const string MsgTip::UserLogin_AccountNotExist = "账号不存在";//1001001
    const string MsgTip::UserLogin_PasswordInitFail = "密码初始化失败";//1001002
    const string MsgTip::UserLogin_PasswordError = "用户密码错误";//1001003
    const string MsgTip::UserLogin_UpdateTokenFail = "用户Token更新失败";//1001004

    //1002zzz 用户注册接口
    const int ResultCode::UserSign_AccountHadExist = 1002001;
    const int ResultCode::UserSign_PasswordInitFail = 1002002;
    const int ResultCode::UserSign_CreateAccountFail = 1002003;
    const int ResultCode::UserSign_GetAccountInfoFail = 1002004;
    const int ResultCode::UserSign_CreateSeesionFail = 1002005;

    const string MsgTip::UserSign_AccountHadExist = "账号已经存在";//1002001
    const string MsgTip::UserSign_PasswordInitFail = "密码初始化失败";//1002002
    const string MsgTip::UserSign_CreateAccountFail = "账号创建失败";//1002003
    const string MsgTip::UserSign_GetAccountInfoFail = "账号信息获取失败";//1002004
    const string MsgTip::UserSign_CreateSeesionFail = "账号会话信息创建失败";//1002005

    //1003zzz 用户退出登录接口
    const int ResultCode::UserLogout_TokenNotValid = 1003001;
    const int ResultCode::UserLogout_UpdateSessionFail = 1003002;
    const int ResultCode::UserLogout_TokenNotExist = 1003003;

    const string MsgTip::UserLogout_TokenNotValid = "Token格式错误";//1003001
    const string MsgTip::UserLogout_UpdateSessionFail = "账号会话信息更新失败";//1003002
    const string MsgTip::UserLogout_TokenNotExist = "Token已失效";//1003003

    //1004zzz 检查在线状态接口
    const int ResultCode::CheckConnect_TokenNotValid = 1004001;
    const int ResultCode::CheckConnect_TokenHadExpire = 1004002;
    const int ResultCode::CheckConnect_AccountTokenNotEqual = 1004003;

    const string MsgTip::CheckConnect_TokenNotValid = "Token格式错误";//1004001
    const string MsgTip::CheckConnect_TokenHadExpire = "Token已经失效";//1004002
    const string MsgTip::CheckConnect_AccountTokenNotEqual = "Token校验失败";//1004003

    //1005zzz 刷新用户Token接口
    const int ResultCode::RefreshToken_TokenNotValid = 1005001;
    const int ResultCode::RefreshToken_RefreshTokenNotValid = 1005002;
    const int ResultCode::RefreshToken_RefreshATokenNotEqual = 1005003;
    const int ResultCode::RefreshToken_RefreshTokenHadExpire = 1005004;
    const int ResultCode::RefreshToken_CreateSeesionFail = 1005005;

    const string MsgTip::RefreshToken_TokenNotValid = "Token格式错误";//1005001
    const string MsgTip::RefreshToken_RefreshTokenNotValid = "RefreshToken格式错误";//1005002
    const string MsgTip::RefreshToken_RefreshATokenNotEqual = "Token与RefreshToken不对应";//1005003
    const string MsgTip::RefreshToken_RefreshTokenHadExpire = "RefreshToken已经失效";//1005004
    const string MsgTip::RefreshToken_CreateSeesionFail = "账号会话信息创建失败";//1005005

}
