
#include "log_utils.h"
#include "file_utils.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <exception>

using namespace utils;

bool LogUtil::isConsoleDebugInfo = false;

void LogUtil::setConsoleDebugInfo(bool value){
	isConsoleDebugInfo = value;
}

void LogUtil::LOGD(string msg)
{
	if (isConsoleDebugInfo)
	{
		SimpleWrite("[ DEBUG ] ", msg);
	}
}

void LogUtil::LOGE(string msg)
{
	SimpleWrite("[ ERROR ] ", msg);
}

void LogUtil::LOGI(string msg)
{
	SimpleWrite("[ INFO ] ", msg);
}

void LogUtil::LOGW(string msg)
{
	SimpleWrite("[ WARN ] ", msg);
}

void LogUtil::LOGM(LogMBean bean)
{
	SimpleWrite("[ INFO ] ", bean.toString());
}

void LogUtil::SimpleWrite(string type, string msg)
{
	cout << type << msg << endl;

	string dirName = "/data/log/";

	time_t t = time(NULL);

	char c_time[11];
	char c_log_time[25];
	strftime(c_time, sizeof(c_time), "%F", localtime(&t));
	strftime(c_log_time, sizeof(c_log_time), "%F %T", localtime(&t));

	string fileName = string(c_time) + ".log";

	string filePath = dirName + fileName;

	if (!File::isFileExist(filePath.c_str()))
	{
		if (!File::createFile(dirName.c_str(), fileName.c_str()))
		{
			cout << "[ WARN ] log file can not be created !" << endl;
			return;
		}
	}

	const char *c_filePath = filePath.c_str();

	try
	{
		FILE *fp = fopen(c_filePath, "a+");
		fprintf(fp, "%s %s %s\n", type.c_str(), c_log_time, msg.c_str());
		fclose(fp);
		//cout << "[ DEBUG ] log file had  open ! " << filePath  << endl;
	}
	catch (exception &e)
	{
		cout << "[ WARN ] log file can not be open !" << endl;
		return;
	}
}

LogMBean::LogMBean(string m)
{
	method = m;
	start_time = clock();
};

void LogMBean::addParam(string key, string value)
{
	v_key.push_back(key);
	v_value.push_back(value);
};

void LogMBean::addParam(string key, int value)
{
	v_key.push_back(key);
	stringstream ss;
	ss << value;
	v_value.push_back(ss.str());
};

string LogMBean::toString()
{
	time_t now_time = time(NULL);
	stringstream ssTime;
	ssTime << now_time;
	string msg = "Request method: " + method + " " + ssTime.str() + " Args [{";
	if (v_key.size() > 0)
	{
		msg += v_key[0] + ":" + v_value[0];
	}
	for (unsigned int i = 1; i < v_key.size(); i++)
	{
		msg += "," + v_key[i] + ":" + v_value[i];
	}
	msg += "}], cost ";

	double diff_seconds = (double)(clock() - start_time) / CLOCKS_PER_SEC;
	stringstream ss;
	ss << diff_seconds * 1000;
	msg += ss.str() + "ms";
	return msg;
}
