
#include <string>

using namespace std;

namespace constants{

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

    class MsgTip{
    public:
        const static string ERROR_ACCOUNT_EMPTY;
        const static string ERROR_ACCOUNT_NOT_VALID_PHONE_NUM;
        const static string ERROR_PASSWORD_EMPTY;
        const static string ERROR_PASSWORD_NOT_VALID;
        const static string ERROR_TOKEN_EMPTY;

        const static string UserLogin_AccountNotExist;          //2001001
        const static string UserLogin_PasswordInitFail;         //2001002
        const static string UserLogin_PasswordError;            //2001003
        const static string UserLogin_UpdateTokenFail;          //2001004

        const static string UserSign_AccountHadExist;           //2002001
        const static string UserSign_PasswordInitFail;          //2002002
        const static string UserSign_CreateAccountFail;         //2002003
        const static string UserSign_GetAccountInfoFail;        //2002004
        const static string UserSign_CreateSeesionFail;         //2002005

        const static string UserLogout_TokenNotValid;           //2003001
        const static string UserLogout_UpdateSessionFail;       //2003002
        
        const static string CheckConnect_TokenNotValid;         //2004001
        const static string CheckConnect_TokenHadExpire;        //2004002
        const static string CheckConnect_AccountOffline;        //2004003
        const static string CheckConnect_AccountTokenNotEqual;  //2004004
    };

    class Constants{
    public:
   
    };

}
