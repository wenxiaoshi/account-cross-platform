#include "redis_manager.h"

#include <string.h>
#include <stdlib.h>

#include <sstream>
#include <iostream>

#include "../../utils/log_utils.h"

#define LOGE(msg)  utils::LogUtil::LOGE(msg);
#define LOGD(msg)  utils::LogUtil::LOGD(msg);
#define LOGI(msg)  utils::LogUtil::LOGI(msg);

using std::stringstream;

using namespace utils;

namespace manager{

Redis * Redis::redis = new Redis();

Redis * Redis::getRedis(){
   return  Redis::redis;
}

Redis::Redis()
{
}

void Redis::connect(ServerConfig _conf)
{
    _context = ::redisConnect(_conf.getRedisIP().c_str(), _conf.getRedisPort());
    
    if(_context && _context->err)
    {
        LOGE("connect redis error");
        exit(EXIT_FAILURE);    
    }
    LOGD("redis connect success !");
}

void Redis::disConnect()
{
    ::redisFree(_context);
    LOGD("redis disConnect success");
}

bool Redis::setString(const string & data)
{
    bool isSuccess;
    try
    {
        _reply = (redisReply*)::redisCommand(_context, data.c_str());
        if (NULL == _reply || !(_reply->type == REDIS_REPLY_STATUS && strcasecmp(_reply->str,"OK") == 0))
        {
            LOGE("Failed to execute SET(string)");
            isSuccess = false;
        } 
        else
        {
            isSuccess = true;
        }     
        freeReply();
    }
    catch(const std::exception& e)
    {
        LOGE(e.what());
        isSuccess = false;
    }
    return isSuccess;
}

bool Redis::setString(const string & key, const string & value)
{
    stringstream ss;
    ss << "SET " << key << " " << value;
    LOGD("redis " + ss.str());
    return setString(ss.str());
}

bool Redis::setString(const string & key, const int & value)
{
    stringstream ss;
    ss << "SET " << key << " " << value;
    LOGD("redis " + ss.str());
    return setString(ss.str());
}

bool Redis::setString(const string & key, const float & value)
{
    stringstream ss;
    ss << "SET " << key << " " << value;
    LOGD("redis " + ss.str());
    return setString(ss.str());
}

bool Redis::getString(const string & key)
{
    LOGD("redis getString " + key);
    bool isSuccess;
    try
    {
        _reply = (redisReply*)::redisCommand(_context, "GET %s", key.c_str());
        if(NULL != _reply && _reply->type == REDIS_REPLY_STRING)
        {
            isSuccess = true;
        }
        else
        {
            isSuccess = false;
        }
    }
    catch(const std::exception& e)
    {
        LOGD(e.what());
        isSuccess = false;
    }
    return isSuccess;
}

bool Redis::getString(const string & key, string & value)
{
    bool isSuccess = getString(key);
    if(isSuccess)
    {
        value = _reply->str;
    }
    freeReply();
    return isSuccess;
}

bool Redis::getString(const string & key, int & value)
{
    bool isSuccess = getString(key);
    if(isSuccess)
    {
        value = ::atoi(_reply->str);
    }
    freeReply();
    return isSuccess;
}

bool Redis::getString(const string & key, float & value)
{
      bool isSuccess = getString(key);
    if(isSuccess)
    {
        value = ::atof(_reply->str);
    }
    freeReply();
    return isSuccess;
}

void Redis::freeReply()
{
    if(_reply)
    {
        freeReplyObject(_reply);
        _reply = NULL;
    }
}

}