
#include "my_constant.h"

using namespace constants;
using namespace std;

const int ResultCode::SUCCESS = 0;
const int ResultCode::ERROR_PARAM_IS_INVALID = 2011;

const string MsgTip::ERROR_ACCOUNT_EMPTY = "账号不能为空，请重新输入";
const string MsgTip::ERROR_ACCOUNT_NOT_VALID_PHONE_NUM = "账号格式错误，请输入正确的手机号码";
const string MsgTip::ERROR_PASSWORD_EMPTY = "密码不能为空，请重新输入";
const string MsgTip::ERROR_PASSWORD_NOT_VALID = "密码格式错误，请输入6-18位的数字或字母";
const string MsgTip::ERROR_TOKEN_EMPTY = "Token不能为空";

const string MsgTip::TOAST_ACCOUNT_OUT_OFF_LINE = "您的账号在另一设备登录，如非本人，请重新登录";


