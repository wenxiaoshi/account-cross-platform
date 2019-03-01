
#include "my_log.h"
#include <iostream>
#include <fstream>
using namespace std;
using namespace log_utils;

bool LOG::isHadInit = false;

void LOG::init(){
	if (!isHadInit)
	{
	   isHadInit = true;
	   ofstream logfile("source/log/error.log");
    	   clog.rdbuf(logfile.rdbuf());
	}
}

void LOG::error(string info){
	init();
	clog<<info<<endl;
}

