#ifndef _SERVER_CONF
#define _SERVER_CONF

#include <string>

using std::string;

namespace manager{
    
    class ServerConfig
    {
        public:
            ServerConfig();
            
            //获取服务的IP和端口
            string getServerIpAndPort();

            //获取SSL的Key和Cert
            string getSSLPathKey();
            string getSSLPathCert();

            //获取Redis的IP和端口
            string getRedisIP();
            int getRedisPort();

            //获取MySQL的IP
            string getMySqlHost();
            //获取MySQL的端口
            int getMySqlPort();

            //获取登入MySQL的用户名
            string getMySqlUser();
            //获取登入MySQL的密码
            string getMySqlPassword();
            //获取登入MySQL的DB名称
            string getMySqlDBName();
            //获取登入MySQL的字符集
            string getMySqlCharset();

            //获取Token进行对称加密的AES密钥
            string getTokenAesKey();

            //获取是否打印和记录DEBUG信息
            bool isConsoleDebugInfo();

        private:

            string SERVER_IP_POST;

            string SSL_PATH_KEY;
            string SSL_PATH_CERT;

            string REDIS_IP;
            int REDIS_POST;

            string MYSQL_HOST;
            string MYSQL_USER;
            int MYSQL_POST;
            string MYSQL_PASSWORD;
            string MYSQL_DB_NAME; 
            string MYSQL_CHARSET;

            string TOKEN_AES_KEY;

            bool IS_CONSOLE_DEBUG_INFO;

            void getConf();
    };
}

#endif