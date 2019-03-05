
#include <string>

using namespace std;

namespace constants{

    class ResultCode{
    public:
        const static int SUCCESS;
		
		const static int ERROR_PARAM_IS_INVALID;
    };

    class MsgTip{
    public:
        const static string ERROR_ACCOUNT_EMPTY;
        const static string ERROR_ACCOUNT_NOT_VALID_PHONE_NUM;
        const static string ERROR_PASSWORD_EMPTY;
        const static string ERROR_PASSWORD_NOT_VALID;
        const static string ERROR_TOKEN_EMPTY;

	const static string MSG_ACCOUNT_OUT_OFF_LINE;
    };

    class Constants{
    public:
   
    };

}

//错误码定义,100之前系统，100之后对应各个Request业务错误码
enum RetCode 
{
    // 公共错误码 (0 ~ 99)
    RetCode_Ok            = 0,


    //业务逻辑错误
    RetCode_InvalidAccount = 1001, // 账号密码错误
    RetCode_Login_Expire   = 1002, // 登陆状态过期
}
