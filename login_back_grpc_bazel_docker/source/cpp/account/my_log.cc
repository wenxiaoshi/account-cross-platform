
#include "my_log.h"

using namespace log_utils;

bool isHadInit = false;

void init(){
	if (!isHadInit)
	{
		isHadInit = true;
		ofstream logfile("source/log/error.log");
    	clog.rdbuf(logfile.rdbuf());
	}
}

void error(string info){
	init();
	clog<<info;
}

