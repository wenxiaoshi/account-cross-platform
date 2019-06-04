//
// Created by melon on 2019/2/3.
//

#ifndef ANDROID_PROJ_CONSTANTS_H
#define ANDROID_PROJ_CONSTANTS_H

#endif //ANDROID_PROJ_CONSTANTS_H

#include <string>

using namespace std;

namespace project_constants{

    /**
     * 客户端业务常量
     */
    class Constants{
    public:
        const static string TOKEN;
        const static string REFRESH_TOKEN;
        const static string IS_CONNECT;
        const static string USER_ACCOUNT;
    };

    /**
     * 客户端业务错误码
     */
    class ClientCode{
    public:
        const static int SUCCESS;
        const static int LOGIN_FAIL_ACCOUNT_INVALID;
        const static int LOGIN_FAIL_PASSWORD_INVALID;
        const static int LOGIN_FAIL_PASSWORD_ERROR;
        const static int LOGIN_FAIL_ACCOUNT_NO_EXIST;
        const static int SIGN_FAIL_ALREADY_EXIST;

        const static int USER_IS_NOT_ONLINE;
        const static int USER_IS_OUT_OF_LINE;
        const static int USER_ACCOUNT_FO_EMPTY;
    };

    /**
     * 客户端业务提示
     */
    class ToastTip{
    public:
        const static string TOAST_ERROR_ACCOUNT_EMPTY;
        const static string TOAST_ERROR_ACCOUNT_NOT_VALID_PHONE_NUM;
        const static string TOAST_ERROR_PASSWORD_EMPTY;
        const static string TOAST_ERROR_PASSWORD_NOT_VALID;
        const static string TOAST_ERROR_NETWORK_UNVALAIBLE;

        const static string TOAST_ACCOUNT_OUT_OFF_LINE;
        const static string TOAST_ACCOUNT_INFO_EMPTY;
    };

    /**
     *  服务端错误码
     **/
    class ResultCode{
    public:
        const static int SUCCESS;

        const static int ReqParamError;

        const static int UserLogin_AccountNotExist;
        const static int UserLogin_PasswordInitFail;
        const static int UserLogin_PasswordError;
        const static int UserLogin_UpdateTokenFail;

        const static int UserSign_AccountHadExist;
        const static int UserSign_PasswordInitFail;
        const static int UserSign_CreateAccountFail;
        const static int UserSign_GetAccountInfoFail;
        const static int UserSign_CreateSeesionFail;

        const static int UserLogout_TokenNotValid;
        const static int UserLogout_UpdateSessionFail;
        const static int UserLogout_TokenNotExist;

        const static int CheckConnect_TokenNotValid;
        const static int CheckConnect_TokenHadExpire;
        const static int CheckConnect_AccountOffline;
        const static int CheckConnect_AccountTokenNotEqual;

        const static int RefreshToken_TokenNotValid;
        const static int RefreshToken_RefreshTokenNotValid;
        const static int RefreshToken_RefreshATokenNotEqual;
        const static int RefreshToken_RefreshTokenHadExpire;
        const static int RefreshToken_CreateSeesionFail;

    };

    /**
     *  服务端错误提示
     **/
    class MsgTip{
    public:
        const static string ERROR_ACCOUNT_EMPTY;                //1
        const static string ERROR_ACCOUNT_NOT_VALID_PHONE_NUM;  //1
        const static string ERROR_PASSWORD_EMPTY;               //1
        const static string ERROR_PASSWORD_NOT_VALID;           //1
        const static string ERROR_TOKEN_EMPTY;                  //1
        const static string ERROR_STRING_EMPTY;                 //1

        const static string UserLogin_AccountNotExist;          //1001001
        const static string UserLogin_PasswordInitFail;         //1001002
        const static string UserLogin_PasswordError;            //1001003
        const static string UserLogin_UpdateTokenFail;          //1001004

        const static string UserSign_AccountHadExist;           //1002001
        const static string UserSign_PasswordInitFail;          //1002002
        const static string UserSign_CreateAccountFail;         //1002003
        const static string UserSign_GetAccountInfoFail;        //1002004
        const static string UserSign_CreateSeesionFail;         //1002005

        const static string UserLogout_TokenNotValid;           //1003001
        const static string UserLogout_UpdateSessionFail;       //1003002
        const static string UserLogout_TokenNotExist;           //1003003

        const static string CheckConnect_TokenNotValid;         //1004001
        const static string CheckConnect_TokenHadExpire;        //1004002
        const static string CheckConnect_AccountOffline;        //1004003
        const static string CheckConnect_AccountTokenNotEqual;  //1004004

        const static string RefreshToken_TokenNotValid;         //1005001
        const static string RefreshToken_RefreshTokenNotValid;  //1005002
        const static string RefreshToken_RefreshATokenNotEqual; //1005003
        const static string RefreshToken_RefreshTokenHadExpire; //1005004
        const static string RefreshToken_CreateSeesionFail;     //1005005

    };

}