#ifndef _SERVER_CONF
#define _SERVER_CONF

#include <string>

using std::string;

namespace manager{
    
    class ServerConfig
    {
        public:
            ServerConfig();
            
            string getRedisIP();
            int getRedisPort();

            string getMySqlHost();
            string getMySqlUser();
            int getMySqlPort();
            string getMySqlPassword();
            string getMySqlDBName();
            string getMySqlCharset();

        private:
            string REDIS_IP;
            int REDIS_POST;

            string MYSQL_HOST;
            string MYSQL_USER;
            int MYSQL_POST;
            string MYSQL_PASSWORD;
            string MYSQL_DB_NAME; 
            string MYSQL_CHARSET;

            void getConf();
    };
}

#endif