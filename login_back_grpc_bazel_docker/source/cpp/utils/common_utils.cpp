//
// Created by melon on 2019/2/28.
//
#include <exception> 

#include "../constant/my_constant.h"

#include "common_utils.h"
#include "log_utils.h"

#include <time.h>
#include <random>
#include <sstream>
#include <iostream>
#include <string.h>
#include <regex>

#define LOGD(msg)  utils::LogUtil::LOGD(msg);
#define LOGW(msg)  utils::LogUtil::LOGW(msg);
#define LOGI(msg)  utils::LogUtil::LOGI(msg);
#define LOGE(msg)  utils::LogUtil::LOGE(msg);

using namespace utils;
using namespace std;
using namespace constants;

unsigned char* CommonUtils::AES_KEY = (unsigned char *) "$L&^E*Usd9k!Ld4%"; // token30天有效时间

AesEncryptor* CommonUtils::aesEncryptor = new AesEncryptor(CommonUtils::AES_KEY);

const long CommonUtils::TOKEN_TIMEOUT = 3600L * 24 * 7; // token7天有效时间
const long CommonUtils::REFRESH_TOKEN_TIMEOUT = 3600L * 24 * 14; // refresh_token14天有效时间

string CommonUtils::GenToken(const unsigned long uid, const string account){
    time_t startTime = time(NULL);
    time_t endTime = startTime + TOKEN_TIMEOUT;
    string randomNumStr = GenRandomStr();

    const int bufSize = 128;
    char tokenChar[bufSize];
    snprintf(tokenChar,bufSize,"%lu:%s:%s:%lu:%lu",uid,account.c_str(),randomNumStr.c_str(),startTime,endTime);
    string tokenAes = aesEncryptor->EncryptString(tokenChar);
    return tokenAes;
}

string CommonUtils::GenRefreshToken(const unsigned long uid, const string account){
    time_t startTime = time(NULL);
    time_t endTime = startTime + REFRESH_TOKEN_TIMEOUT;
    string randomNumStr = GenRandomStr();

    const int bufSize = 128;
    char tokenChar[bufSize];
    snprintf(tokenChar,bufSize,"%lu:%s:%s:%lu:%lu",uid,account.c_str(),randomNumStr.c_str(),startTime,endTime);
    string tokenAes = aesEncryptor->EncryptString(tokenChar);
    return tokenAes;
}

string CommonUtils::DecryptToken(string token){
    return aesEncryptor->DecryptString(token);
}

/**
 * 生成6位随机数
 */
string CommonUtils::GenRandomStr(){
    int array[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (int i = 9; i > 0; i--) {
        default_random_engine e;
        uniform_int_distribution<unsigned> u(0, i); //随机数分布对象
        int index = u(e);
        int tmp = array[index];
        array[index] = array[i - 1];
        array[i - 1] = tmp;
    }

    stringstream strStream;

    for (int i = 0; i < 6; i++)
        strStream << array[i];

    return strStream.str();
}

void CommonUtils::SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c) {
            std::string::size_type pos1, pos2;
            pos2 = s.find(c);
            pos1 = 0;
            while(std::string::npos != pos2)
            {
                v.push_back(s.substr(pos1, pos2-pos1));

                pos1 = pos2 + c.size();
                pos2 = s.find(c, pos1);
            }
            if(pos1 != s.length())
                v.push_back(s.substr(pos1));
}

string CommonUtils::EncryptPwd(string account, string password){
    if (account.empty() || password.empty()) {
        return "";
    }
    try {       
        LOGD("encryt ac`" + account + " psd`" + password);
	    //用sha256对account进行消息摘要，目的是给password加盐
        string sha256_account;
        hash256_hex_string(account, sha256_account);
        const char* c_account = sha256_account.c_str();

        //获取加密前password字符数组
        const char* c_password = password.c_str();

        //合并salt到password字符数组
        size_t length = strlen(c_account) + strlen(c_password);
        char c_source[length+1];
        snprintf(c_source, length+1, "%s%s", c_account, c_password);

        //对字符数组进行MD5计算
        MD5 md5(c_source, length);
        string result = md5.toString();
        LOGD("md5 | " + result);
    	return result;
    } catch (exception& e) {  
        LOGW("EncryptPwd is fail !");
        return "";
    }
}

 /**
  * 判断字符串是否相等
  **/
  bool CommonUtils::isEqual(string origin,string target) {
    const char* oChar = origin.data();
    const char* tChar = target.data();
    if (origin.size() != target.size()) {
      return false;
    }
    for (unsigned int i = 0; i < origin.size(); i++) {
      if (oChar[i] != tChar[i]) {
        return false;
      }
    }
    return true;
  }


