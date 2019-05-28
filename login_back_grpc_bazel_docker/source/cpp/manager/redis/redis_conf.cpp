#include "redis_conf.h"
#include <stdlib.h>
#include "source/libs/json/json.h"
#include <string>
#include <iostream>
#include <fstream>
#include "../../utils/log_utils.h"

#define LOGE(msg)  utils::LogUtil::LOGE(msg);

namespace manager{

    using std::ifstream;
    using std::cout;
    using std::endl;

    RedisConf::RedisConf()
    {
        try
        {
            getConf();
        }
        catch(const std::exception& e)
        {
            LOGE(e.what())
            exit(EXIT_FAILURE);
        }
    }

    void RedisConf::getConf()
    {
        ifstream ifs;
        ifs.open("source/conf/redis_conf.json");
        if(!ifs.good())
        {
            LOGE("open RedisConf.json error")
            exit(EXIT_FAILURE);
        }

        Json::Value root;
        Json::Reader reader;
        if(!reader.parse(ifs, root, false))
        {
            LOGE("RedisConf json reader error")
            exit(EXIT_FAILURE);
        }
        
        _ip = root["IP"].asString();
        _port = root["PORT"].asInt();
        ifs.close();
    }

    string RedisConf::getIP()
    {
        return _ip;
    }

    int RedisConf::getPort()
    {
        return _port;
    }

}