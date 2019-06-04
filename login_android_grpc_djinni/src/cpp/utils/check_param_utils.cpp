//
// Created by melon on 2019/2/25.
//

#include <regex>

#include "check_param_utils.h"
#include "proj_constants.h"

using namespace utils;
using namespace project_constants;
using namespace std;

bool ParamUtils::checkAccountValid(string account,string & errorMsg){
    if (account.empty()) {
        errorMsg = ToastTip::TOAST_ERROR_ACCOUNT_EMPTY;
        return false;
    }
    string pattern = "^[1][3,4,5,6,7,8,9]\\d{9}$";
    if (!patternMatch(pattern, account)) {
        errorMsg = ToastTip::TOAST_ERROR_ACCOUNT_NOT_VALID_PHONE_NUM;
        return false;
    }
    return true;
}

bool ParamUtils::checkPasswordValid(string password,string & errorMsg){
    if (password == ""){
        errorMsg = ToastTip::TOAST_ERROR_PASSWORD_EMPTY;
        return false;
    }
    string pattern = "^[a-z0-9A-Z]{6,18}$";
    if (!patternMatch(pattern, password)) {
        errorMsg = ToastTip::TOAST_ERROR_PASSWORD_NOT_VALID;
        return false;
    }
    return true;
}

bool ParamUtils::patternMatch(string pattern, string source_str){
    string strPattern(pattern);
    regex r(strPattern);
    smatch result;
    return regex_search(source_str, result, r);
}