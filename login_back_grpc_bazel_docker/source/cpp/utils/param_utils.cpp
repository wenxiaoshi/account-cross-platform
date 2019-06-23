#include <regex>

#include "param_utils.h"
#include "../constant/my_constant.h"

using namespace utils;
using namespace constants;

/**
 * 手机号规则：
 * 第一位为1，总共9位，剩余8位由3\4\5\6\7\8\9组成
 **/ 
bool ParamUtils::CheckAccountValid(string account,string & errorMsg){
    if (account.empty()) {
        errorMsg = MsgTip::ERROR_ACCOUNT_EMPTY;
        return false;
    }
    string pattern = "^[1][3,4,5,6,7,8,9]\\d{9}$";
    if (!PatternMatch(pattern, account)) {
        errorMsg = MsgTip::ERROR_ACCOUNT_NOT_VALID_PHONE_NUM;
        return false;
    }
    return true;
}

/**
 * 密码规则（初级密码）：
 * 总共6-18位，
 * 由任意数字或字母组成
 **/ 
bool ParamUtils::CheckPassLwordValid(string password,string & errorMsg){
    if (password == ""){
        errorMsg = MsgTip::ERROR_PASSWORD_EMPTY;
        return false;
    }
    string pattern = "^[a-z0-9A-Z]{6,18}$";
    if (!PatternMatch(pattern, password)) {
        errorMsg = MsgTip::ERROR_PASSWORD_NOT_VALID;
        return false;
    }
    return true;
}

/**
 * 密码规则（中级密码）：
 * 总共6-18位
 * 至少1个数字字符
 * 至少1个小写字母
 * 至少1个大写字母
 * 其他可以是任意字符
 **/ 
bool ParamUtils::CheckPassMwordValid(string password,string & errorMsg){
    if (password == ""){
        errorMsg = MsgTip::ERROR_PASSWORD_EMPTY;
        return false;
    }
    string pattern = "^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)[^]{6,18}$";

    if (!PatternMatch(pattern, password)) {
        errorMsg = MsgTip::ERROR_PASSWORD_NOT_VALID;
        return false;
    }
    return true;
}

/**
 * 密码规则（高级密码）：
 * 总共6-18位，
 * 至少1个数字字符
 * 至少1个小写字母
 * 至少1个大写字母
 * 至少1个特殊字符
 **/ 
bool ParamUtils::CheckPassHwordValid(string password,string & errorMsg){
    if (password == ""){
        errorMsg = MsgTip::ERROR_PASSWORD_EMPTY;
        return false;
    }
    string pattern = "^(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9])(?=.*?[^\\w\\s]).{6,18}$";
    if (!PatternMatch(pattern, password)) {
        errorMsg = MsgTip::ERROR_PASSWORD_NOT_VALID;
        return false;
    }
    return true;
}



/**
 * 字符串规则：
 * 不能为空
 **/ 
bool ParamUtils::CheckStringValid(string str,string & errorMsg){
    if (str == ""){
        errorMsg = MsgTip::ERROR_STRING_EMPTY;
        return false;
    }
    return true;
}

/**
 * 正则匹配
 **/ 
bool ParamUtils::PatternMatch(string pattern, string source_str){
    string strPattern(pattern);
    regex r(strPattern);
    smatch result;
    return regex_search(source_str, result, r);
}
