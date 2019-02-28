//
// Created by melon on 2019/2/25.
//

#include "check_param_utils.h"
#include "proj_constants.h"

using namespace utils;
using namespace project_constants;


bool ParamUtils::checkAccountValid(std::string account,std::string & errorMsg){
    if (account.empty()){
        errorMsg = MsgTip::TOAST_ERROR_ACCOUNT_EMPTY;
        return false;
    }
    return true;
}

bool ParamUtils::checkPasswordValid(std::string password,std::string & errorMsg){
    if (password == ""){
        errorMsg = MsgTip::TOAST_ERROR_PASSWORD_EMPTY;
        return false;
    }
    return true;
}