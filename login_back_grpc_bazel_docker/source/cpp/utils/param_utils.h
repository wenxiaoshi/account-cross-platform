
#include <string>

using namespace std;

namespace utils{
     /**
     *  参数校验工具类
     **/
    class ParamUtils{
    public:
        static bool CheckAccountValid(string account,string & errorMsg);
        static bool CheckPasswordValid(string password,string & errorMsg);
        static bool CheckTokenValid(string token,string & errorMsg);
        static bool CheckStringValid(string str,string & errorMsg);
    private:
        static bool PatternMatch(string pattern, string source_str);
    };
}