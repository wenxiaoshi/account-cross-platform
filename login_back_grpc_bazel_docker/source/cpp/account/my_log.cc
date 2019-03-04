
#include "my_log.h"
#include <iostream>
#include <fstream>

using namespace utils;

void LogUtil::LOGD(string msg){
    cout << "DEBUG : " << msg << endl;
}

void LogUtil::LOGE(string msg){
    cout << "ERROR : " << msg << endl;
}

void LogUtil::LOGI(string msg){
    cout << "INFO : " << msg << endl;
}

void LogUtil::LOGW(string msg){
    cout << "WARN : " << msg << endl;
}
