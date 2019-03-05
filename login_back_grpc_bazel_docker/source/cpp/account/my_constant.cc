
#include "my_constant.h"

using namespace constants;
using namespace std;

// 公共错误码（0 ~ 99）
const int ResultCode::SUCCESS = 0;
const int ResultCode::ReqParamError = 1;

const string MsgTip::ERROR_ACCOUNT_EMPTY = "账号不能为空，请重新输入";//1
const string MsgTip::ERROR_ACCOUNT_NOT_VALID_PHONE_NUM = "账号格式错误，请输入正确的手机号码";//1
const string MsgTip::ERROR_PASSWORD_EMPTY = "密码不能为空，请重新输入";//1
const string MsgTip::ERROR_PASSWORD_NOT_VALID = "密码格式错误，请输入6-18位的数字或字母";//1
const string MsgTip::ERROR_TOKEN_EMPTY = "Token不能为空";//1

//业务逻辑错误（1xxx 账号模块，2xxx 卡包模块, ......）

//1001XXX 用户登录接口
const int ResultCode::UserLogin_AccountNotExist = 1001001;
const int ResultCode::UserLogin_PasswordInitFail = 1001002;
const int ResultCode::UserLogin_PasswordError = 1001003;
const int ResultCode::UserLogin_UpdateTokenFail = 1001004;

const string MsgTip::UserLogin_AccountNotExist = "该账号不存在";//1001001
const string MsgTip::UserLogin_PasswordInitFail = "初始化密码失败";//1001002
const string MsgTip::UserLogin_PasswordError = "密码输入错误";//1001003
const string MsgTip::UserLogin_UpdateTokenFail = "用户Token更新失败";//1001004

//2002XXX 用户注册接口
const int ResultCode::UserSign_AccountHadExist = 1002001;
const int ResultCode::UserSign_PasswordInitFail = 1002002;
const int ResultCode::UserSign_CreateAccountFail = 1002003;
const int ResultCode::UserSign_GetAccountInfoFail = 1002004;
const int ResultCode::UserSign_CreateSeesionFail = 1002005;

const string MsgTip::UserSign_AccountHadExist = "该账号已经注册";//1002001
const string MsgTip::UserSign_PasswordInitFail = "初始化密码失败";//1002002
const string MsgTip::UserSign_CreateAccountFail = "账号创建失败";//1002003
const string MsgTip::UserSign_GetAccountInfoFail = "获取账号信息失败";//1002004
const string MsgTip::UserSign_CreateSeesionFail = "账号会话信息创建失败";//1002005

//2003XXX 用户退出登录接口
const int ResultCode::UserLogout_TokenNotValid = 1003001;
const int ResultCode::UserLogout_UpdateSessionFail = 1003002;

const string MsgTip::UserLogout_TokenNotValid = "Token格式错误";//1003001
const string MsgTip::UserLogout_UpdateSessionFail = "账号会话信息更新失败";//1003002

//2004XXX 检查在线状态接口
const int ResultCode::CheckConnect_TokenNotValid = 1004001;
const int ResultCode::CheckConnect_TokenHadExpire = 1004002;
const int ResultCode::CheckConnect_AccountOffline = 1004003;
const int ResultCode::CheckConnect_AccountTokenNotEqual = 1004004;

const string MsgTip::CheckConnect_TokenNotValid = "Token格式错误";//1004001
const string MsgTip::CheckConnect_TokenHadExpire = "Token已经失效";//1004002
const string MsgTip::CheckConnect_AccountOffline = "账号已离线";//1004003
const string MsgTip::CheckConnect_AccountTokenNotEqual = "Token与在线Token不符";//1004004