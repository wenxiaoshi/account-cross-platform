#include "../conf/server_conf.h"

#include <string>

#include <hiredis/hiredis.h>

using namespace std;

namespace manager{

class Redis
{
    public:
        Redis();

        void connect(ServerConfig conf);
        void disConnect();

        bool setString(const string & key, const string & value);
        bool setString(const string & key, const int & value);
        bool setString(const string & key, const float & value);

        bool getString(const string & key, string & value);
        bool getString(const string & key, int & value);
        bool getString(const string & key, float & value);

        bool delByKey(const string & key);
      
        static Redis * getRedis();

    private:
        bool setString(const string & data);
        bool getString(const string & key);
        void freeReply();

        static Redis * redis;

        redisContext * _context;
        redisReply * _reply;

        
};
}