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

#define LOGD(msg) utils::LogUtil::LOGD(msg);
#define LOGW(msg) utils::LogUtil::LOGW(msg);
#define LOGI(msg) utils::LogUtil::LOGI(msg);
#define LOGE(msg) utils::LogUtil::LOGE(msg);

using namespace utils;
using namespace std;
using namespace constants;

AesEncryptor *CommonUtils::aesEncryptor;

//const long CommonUtils::TOKEN_TIMEOUT = 3600L * 24 * 7; // token 7天有效时间
long CommonUtils::TOKEN_TIMEOUT = 30L;                     // token 30秒有效时间
long CommonUtils::REFRESH_TOKEN_TIMEOUT = 3600L * 24 * 14; // refresh_token 14天有效时间

void CommonUtils::setAesKey(string key){
    aesEncryptor = new AesEncryptor((unsigned char*)key.c_str());
}

//设置Token的过期时间
void CommonUtils::setTokenTimeout(long time){
    TOKEN_TIMEOUT = time;
}

//设置RefreshToken的过期时间
void CommonUtils::setRefreshTokenTimeout(long time){
    REFRESH_TOKEN_TIMEOUT = time;
}

//获取Token的过期时间
long CommonUtils::getTokenTimeout(){
    return TOKEN_TIMEOUT;
}

//获取RefreshToken的过期时间
long CommonUtils::getRefreshTokenTimeout(){
    return REFRESH_TOKEN_TIMEOUT;
}

/**
 * 生成Token
 */
string CommonUtils::GenToken(const unsigned long uid, const string account)
{
    //获取时间变量
    time_t startTime = time(NULL);
    time_t endTime = startTime + TOKEN_TIMEOUT;

    //获取随机数
    string randomNumStr = GenRandomStr(6);

    //构建Token
    const int bufSize = 128;
    char tokenChar[bufSize];
    snprintf(tokenChar, bufSize, "%lu:%s:%s:%lu:%lu", uid, account.c_str(), randomNumStr.c_str(), startTime, endTime);

    //进行对称加密
    string tokenAes = aesEncryptor->EncryptString(tokenChar);
    return tokenAes;
}

/**
 * 生成RefreshToken
 */
string CommonUtils::GenRefreshToken(const unsigned long uid, const string account)
{
    //获取时间变量
    time_t startTime = time(NULL);
    time_t endTime = startTime + REFRESH_TOKEN_TIMEOUT;

    //获取随机数
    string randomNumStr = GenRandomStr(6);

    //构建Token
    const int bufSize = 128;
    char tokenChar[bufSize];
    snprintf(tokenChar, bufSize, "%lu:%s:%s:%lu:%lu", uid, account.c_str(), randomNumStr.c_str(), startTime, endTime);

    //进行对称加密
    string tokenAes = aesEncryptor->EncryptString(tokenChar);
    return tokenAes;
}

string CommonUtils::DecryptToken(string token)
{
    return aesEncryptor->DecryptString(token);
}

/**
 * 获取密码盐值（随机字符串）
 */
string CommonUtils::GenPwdSalt()
{
    return GenRandomStr(16);
}

/**
 * 生成随机字符串
 */
string CommonUtils::GenRandomStr(uint count)
{
    srand(time(0)); //产生随机化种子

    char randomChars[count + 1];
    randomChars[count] = '\0';
    for (uint i = 0; i < count; i++)
    {
        uint type = rand() % 3;
        switch (type)
        {
        case 0:
            randomChars[i] = rand() % ('9' - '0' + 1) + '0';
            break;
        case 1:
            randomChars[i] = rand() % ('Z' - 'A' + 1) + 'A';
            break;
        case 2:
            randomChars[i] = rand() % ('z' - 'a' + 1) + 'a';
            break;
        default:
            break;
        }
    }
    return randomChars;
}

/**
 * 切割字符串
 */
void CommonUtils::SplitString(const std::string &s, std::vector<std::string> &v, const std::string &c)
{
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while (std::string::npos != pos2)
    {

        v.push_back(s.substr(pos1, pos2 - pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if (pos1 != s.length())
        v.push_back(s.substr(pos1));
}

/**
 * 替换字符串
 */
void CommonUtils::replaceAll(std::string &str, const std::string old, const std::string reg)
{
    vector<string> delimVec;
    CommonUtils::SplitString(str, delimVec, old);
    if (delimVec.size() <= 0)
    {
        return;
    }
    string target(delimVec[0]);
    vector<string>::iterator it = delimVec.begin();
    ++it;
    for (; it != delimVec.end(); ++it)
    {
        target = target + reg + (*it);
    }
    str = target;
    return;
}

/**
 * 密码初始化
 */
string CommonUtils::EncryptPwd(string account, string password, string pwdSalt)
{
    if (account.empty() || password.empty())
    {
        return "";
    }
    try
    {
        LOGD("encryt ac`" + account + " psd`" + password);
        //用sha256对account进行消息摘要，目的是给password加盐
        string sha256_account;
        hash256_hex_string(account, sha256_account);
        const char *c_account = sha256_account.c_str();

        //取出系统盐值
        const char *c_salt = pwdSalt.c_str();

        //获取加密前password字符数组
        const char *c_password = password.c_str();

        //合并salt到password字符数组
        size_t length = strlen(c_account) + strlen(c_salt) + strlen(c_password);
        char c_source[length + 1];
        snprintf(c_source, length + 1, "%s%s%s", c_account, c_salt, c_password);

        //对字符数组进行MD5计算
        string sha256_enc_password;
        hash256_hex_string((string)c_source, sha256_enc_password);
        LOGD("sha256 | " + sha256_enc_password);
        return sha256_enc_password;
    }
    catch (exception &e)
    {
        LOGW("EncryptPwd is fail !");
        return "";
    }
}

/**
 * 将string转换成int
 **/
int CommonUtils::getIntByString(string str)
{
    stringstream ss;
    ss << str;
    int i_data;
    ss >> i_data;
    return i_data;
}
