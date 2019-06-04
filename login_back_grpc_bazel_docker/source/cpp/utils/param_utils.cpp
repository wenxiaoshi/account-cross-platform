#include <regex>

#include "param_utils.h"
#include "../constant/my_constant.h"

using namespace utils;
using namespace constants;

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

bool ParamUtils::CheckPasswordValid(string password,string & errorMsg){
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

bool ParamUtils::CheckStringValid(string str,string & errorMsg){
    if (str == ""){
        errorMsg = MsgTip::ERROR_STRING_EMPTY;
        return false;
    }
    return true;
}

bool ParamUtils::PatternMatch(string pattern, string source_str){
    string strPattern(pattern);
    regex r(strPattern);
    smatch result;
    return regex_search(source_str, result, r);
}
