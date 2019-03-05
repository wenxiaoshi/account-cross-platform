
#include "my_constant.h"

using namespace constants;
using namespace std;

// 公共错误码（0 ~ 99）
const int ResultCode::SUCCESS = 0;
const int ResultCode::ReqParamError = 1;

//业务逻辑错误（1xxx 客户端，2xxx服务端）
//2001XXX 用户登录接口
const int ResultCode::UserLogin_AccountNotExist = 2001001;
const int ResultCode::UserLogin_PasswordInitFail = 2001002;
const int ResultCode::UserLogin_PasswordError = 2001003;

const string MsgTip::ERROR_ACCOUNT_EMPTY = "账号不能为空，请重新输入";//1
const string MsgTip::ERROR_ACCOUNT_NOT_VALID_PHONE_NUM = "账号格式错误，请输入正确的手机号码";//1
const string MsgTip::ERROR_PASSWORD_EMPTY = "密码不能为空，请重新输入";//1
const string MsgTip::ERROR_PASSWORD_NOT_VALID = "密码格式错误，请输入6-18位的数字或字母";//1
const string MsgTip::ERROR_TOKEN_EMPTY = "Token不能为空";//1

const string MsgTip::MSG_ACCOUNT_OUT_OFF_LINE = "您的账号在另一设备登录，如非本人，请重新登录";


