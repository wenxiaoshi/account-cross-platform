
#include <string>
#include <vector>
#include <time.h>
#include <sstream>

using namespace std;

namespace utils{

    class LogUtil{
    public:
        static void LOGD(string msg);
        static void LOGE(string msg);
        static void LOGI(string msg);
        static void LOGW(string msg);
        static void LOGM(LogMBean bean);
    private:
    	static void SimpleWrite(string type, string msg);
    };

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

    	string getString(){
    		string msg = "Request method: " + method + ", Args [{";
    		if (v_key.size() > 0) {
    			msg += v_key[0] + ":" + v_value[0];
    		}
    		for (unsigned int i = 0; i < count; i++) {
    			msg += "," + v_key[i] + ":" + v_value[i];
    		}
    		msg += "}], cost ";

			stringstream ss;
		    ss<< clock() - start_time;
		    msg += ss.str() + "ms";
		    return msg;
    	}

    private:
    	string method;
    	clock_t start_time;
    	vector<string> v_key;
    	vector<string> v_value;
    };

}