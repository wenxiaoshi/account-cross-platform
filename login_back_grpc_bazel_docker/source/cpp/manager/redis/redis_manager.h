
#include <string>

#include "redis_conf.h"

#include <hiredis/hiredis.h>

using namespace std;

namespace manager{

class Redis
{
    public:
        Redis();

        void Connect();
        void disConnect();

        bool setString(const string & key, const string & value);
        bool setString(const string & key, const int & value);
        bool setString(const string & key, const float & value);

        bool getString(const string & key, string & value);
        bool getString(const string & key, int & value);
        bool getString(const string & key, float & value);

        static Redis * getRedis();

    private:
        bool setString(const string & data);
        bool getString(const string & key);
        void freeReply();
        bool isError();

        RedisConf _conf;
        redisContext * _context;
        redisReply * _reply;

        static Redis * redis;
};
}