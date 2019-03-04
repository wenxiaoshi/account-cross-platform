
#include "my_log.h"
#include "file_utils.h"

#include <time.h> 
#include <stdio.h> 
#include <iostream>
#include <fstream>
#include <exception>

using namespace utils;

void LogUtil::LOGD(string msg){
    SimpleWrite("DEBUG : ", msg);
}

void LogUtil::LOGE(string msg){
    SimpleWrite("ERROR : ", msg);
}

void LogUtil::LOGI(string msg){
    SimpleWrite("INFO : ", msg);
}

void LogUtil::LOGW(string msg){
    SimpleWrite("WARN : ", msg);
}

void LogUtil::SimpleWrite(string type, string msg){
	cout << type << msg << endl;

	string dirName = "/data/log/";

	time_t t = time(NULL); 
    char c_time[11]; 
    strftime(c_time, sizeof(c_time), "%Y-%m-%d",localtime(&t)); 
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
	    fprintf(fp, "%s%ul %s\n", type, t, msg);  
	    fclose(fp); 
	} catch (exception& e) {  
        cout << "WARN : log file can not be open !" << endl;
        return;
    }
	
	 

}