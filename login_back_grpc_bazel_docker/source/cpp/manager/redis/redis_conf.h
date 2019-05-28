#include <string>

using std::string;

namespace manager{
    class RedisConf
    {
        public:
            RedisConf();
            void getConf();
            string getIP();
            int getPort();
        private:
            string _ip;
            int _port;
    };
}