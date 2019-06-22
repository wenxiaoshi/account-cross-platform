
#include <string>

using namespace std;

namespace utils{
     /**
     *  参数校验工具类
     **/
    class ParamUtils{
    public:

        /*
        主要功能：
        检查用户账号是否合法

        入口参数
        account：       用户账号
        errorMsg：      返回的消息，包括错误消息

       	出口参数：
       	bool： 			true表示合法；false表示非法
        */
        static bool CheckAccountValid(string account,string & errorMsg);

        /*
        主要功能：
        检查用户密码是否合法
        按复杂度分成三种（低中高）

        入口参数
        password：      用户密码
        errorMsg：      返回的消息，包括错误消息

       	出口参数：
       	bool： 			true表示合法；false表示非法
        */
        static bool CheckPassLwordValid(string password,string & errorMsg);
        static bool CheckPassMwordValid(string password,string & errorMsg);
        static bool CheckPassHwordValid(string password,string & errorMsg);

        /*
        主要功能：
        检查传入的字符串（是否为空）

        入口参数
        str：           检查的字符串
        errorMsg：      返回的消息，包括错误消息

       	出口参数：
       	bool： 			true表示合法；false表示非法
        */
        static bool CheckStringValid(string str,string & errorMsg);
    private:

        /*
        主要功能：
        进行正则匹配检查字符串是否合法

        入口参数
        pattern：       正则匹配的规则字符串
        source_str：    进行正则匹配的数据源

       	出口参数：
       	bool： 			true表示合法；false表示非法
        */
        static bool PatternMatch(string pattern, string source_str);
    };
}