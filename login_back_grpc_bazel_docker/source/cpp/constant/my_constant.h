
#include <string>

using namespace std;

namespace constants{

    /**
     *  错误码Code
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

        const static int CheckConnect_TokenNotValid;
        const static int CheckConnect_TokenHadExpire;
        const static int CheckConnect_AccountOffline;
        const static int CheckConnect_AccountTokenNotEqual;

    };

    /**
     *  错误码Message
     **/
    class MsgTip{
    public:
        const static string ERROR_ACCOUNT_EMPTY;                //1
        const static string ERROR_ACCOUNT_NOT_VALID_PHONE_NUM;  //1
        const static string ERROR_PASSWORD_EMPTY;               //1
        const static string ERROR_PASSWORD_NOT_VALID;           //1
        const static string ERROR_TOKEN_EMPTY;                  //1

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

        const static string CheckConnect_TokenNotValid;         //1004001
        const static string CheckConnect_TokenHadExpire;        //1004002
        const static string CheckConnect_AccountOffline;        //1004003
        const static string CheckConnect_AccountTokenNotEqual;  //1004004
    };

    class Constants{
    public:
   
    };

}
