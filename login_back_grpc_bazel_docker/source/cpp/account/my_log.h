
#include <string>

using namespace std;

namespace utils{

    class LogUtil{
    public:
        static void LOGD(string msg);
        static void LOGE(string msg);
        static void LOGI(string msg);
        static void LOGW(string msg);
    private:
    };

}