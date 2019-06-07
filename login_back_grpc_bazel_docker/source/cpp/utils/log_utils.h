
#include <string>
#include <vector>
#include <time.h>
#include <sstream>
#include <iostream>

using namespace std;

namespace utils{

   	/**
     *  日志打印接口Model
     **/
    class LogMBean{

    public:
    	LogMBean(string m);

    	void addParam(string key, string value);
    	void addParam(string key, int value);
    	string toString();

    private:
    	string method;
    	clock_t start_time;
    	vector<string> v_key;
    	vector<string> v_value;
    };

    /**
     *  日志打印工具类
     **/
    class LogUtil{
    public:

		/*
        主要功能：
        进行信息打印和日志记录

		说明：
		LOGI 	相关INFO信息可以让运维保留下来，可用来分析，比如接口访问信息
		LOGD 	相关DEBUG信息提供给开发来用调试程序
		LOGW 	相关WARN信息提示程序没有按照预期的运行，但不会影响到整体正常运行，需要被FIX
		LOGE 	相关ERROR信息提示程序出现了严重错误，影响到系统的正常运行，必须被FIX
		LOGM	接口访问情况封装成Model，把Model信息打印在控制台，并记录到日志中

        入口参数
        msg：       	需要进行打印和写入日志的信息
        */
        static void LOGD(string msg);
        static void LOGE(string msg);
        static void LOGI(string msg);
        static void LOGW(string msg);
        static void LOGM(LogMBean bean);
    private:

		/*
        主要功能：
        写信息到日志中

        入口参数
        type：     	写日志的类型
        msg：     	写日志的信息
        */
        static void SimpleWrite(string type, string msg);
    };

}
