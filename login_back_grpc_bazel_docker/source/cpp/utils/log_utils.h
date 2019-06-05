
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
    	LogMBean(string m){
    		method = m;
    		start_time = clock();
    	};

    	void addParam(string key, string value){
    		v_key.push_back(key);
    		v_value.push_back(value);
    	};

    	void addParam(string key, int value){
    		v_key.push_back(key);
			stringstream ss;
    		ss << value;
    		v_value.push_back(ss.str());
    	};

    	string toString(){
	    	time_t now_time = time(NULL);
			stringstream ssTime;
    		ssTime << now_time;
    		string msg = "Request method: " + method + " "+ ssTime.str() +" Args [{";
    		if (v_key.size() > 0) {
    			msg += v_key[0] + ":" + v_value[0];
    		}
    		for (unsigned int i = 1; i < v_key.size(); i++) {
    			msg += "," + v_key[i] + ":" + v_value[i];
    		}
    		msg += "}], cost ";

		double diff_seconds = (double)(clock() - start_time)/CLOCKS_PER_SEC;
		stringstream ss;
		ss<< diff_seconds * 1000;
		msg += ss.str() + "ms";
		return msg;
    	}

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
        static void LOGD(string msg);
        static void LOGE(string msg);
        static void LOGI(string msg);
        static void LOGW(string msg);
        static void LOGM(utils::LogMBean bean);
    private:
        static void SimpleWrite(string type, string msg);
    };

}
