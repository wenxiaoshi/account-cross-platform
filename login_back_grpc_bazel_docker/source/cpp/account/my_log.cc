
#include "my_log.h"
#include "file_utils.h"

#include <time.h> 
#include <stdio.h> 
#include <iostream>
#include <fstream>
#include <exception>

using namespace utils;

void LogUtil::LOGD(string msg){
    SimpleWrite("[ DEBUG ] ", msg);
}

void LogUtil::LOGE(string msg){
    SimpleWrite("[ ERROR ] ", msg);
}

void LogUtil::LOGI(string msg){
    SimpleWrite("[ INFO ] ", msg);
}

void LogUtil::LOGW(string msg){
    SimpleWrite("[ WARN ] ", msg);
}

void LogUtil::SimpleWrite(string type, string msg){
	cout << type << msg << endl;

	string dirName = "/data/log/";

	time_t t = time(NULL); 
    char c_time[11]; 
    char c_log_time[25];
    strftime(c_time, sizeof(c_time), "%F",localtime(&t)); 
    strftime(c_log_time, sizeof(c_log_time), "%F %T",localtime(&t));
    
    string fileName = string(c_time) + ".log";

	string filePath = dirName + fileName;

	if (!File::isFileExist(filePath.c_str())) {
		if (!File::createFile(dirName.c_str(),fileName.c_str())) {
			cout << "WARN : log file can not be created !" << endl;
			return;
		}
	}
	
	const char * c_filePath = filePath.c_str();
	cout << "DEBUG : log file path = " << filePath << endl;
	
	try{
		FILE *fp = fopen(c_filePath, "a+");  
	    fprintf(fp, "%s %s %s\n", type.c_str(),c_log_time, msg.c_str());  
	    fclose(fp); 
	} catch (exception& e) {  
        cout << "WARN : log file can not be open !" << endl;
        return;
    }
	
	 

}
