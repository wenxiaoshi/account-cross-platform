#include "../conf/server_conf.h"

#include <string>

#include <hiredis/hiredis.h>

using namespace std;

namespace manager{

class Redis
{
    public:
        Redis();

		/*
		主要功能：
		连接Redis

		入口参数
       	conf：	        连接Redis所需的配置信息
		*/
        void connect(ServerConfig conf);

        /*
		主要功能：
		断开Redis
		*/
        void disConnect();

		/*
		主要功能：
		存入数据到Redis

		入口参数
       	key：	        存入redis的数据的键
		value：         存入redis的数据的值

       	出口参数：
       	bool ： 		true表示成功；false表示失败
		*/
        bool setString(const string & key, const string & value);
        bool setString(const string & key, const int & value);
        bool setString(const string & key, const float & value);

		/*
		主要功能：
		从Redis中获取数据

		入口参数
       	key：	        获取数据的键
		value：         获取数据的值

       	出口参数：
       	bool ： 		true表示成功；false表示失败
		*/
        bool getString(const string & key, string & value);
        bool getString(const string & key, int & value);
        bool getString(const string & key, float & value);

		/*
		主要功能：
		从Redis中删除数据

		入口参数
       	key：	        删除数据的键

       	出口参数：
       	bool： 		    true表示成功；false表示失败
		*/
        bool delByKey(const string & key);
      
        /*
		主要功能：
		获取操作Redis的单例

       	出口参数：
       	Redis： 		Redis的实例
		*/
        static Redis * getRedis();

    private:

        /*
		主要功能：
		存入数据到Redis

		入口参数
       	data：	        存入redis的操作语句

       	出口参数：
       	bool ： 		true表示成功；false表示失败
		*/
        bool setString(const string & data);

		/*
		主要功能：
		从Redis中获取数据

		入口参数
       	data：	        获取数据的操作语句

       	出口参数：
       	bool ： 		true表示成功；false表示失败
		*/
        bool getString(const string & key);
        void freeReply();

        //Redis实例
        static Redis * redis;

        //Redis上下文
        redisContext * _context;

        //Redis返回数据实例
        redisReply * _reply;

        
};
}