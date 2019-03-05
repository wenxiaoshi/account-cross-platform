
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
