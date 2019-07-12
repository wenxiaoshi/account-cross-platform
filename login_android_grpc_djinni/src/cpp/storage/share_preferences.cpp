//
// Created by melon on 2019/2/25.
//
#include "../utils/file_utils.h"
//#include <android/log.h>
#include "../utils/glob_utils.h"
#include "share_preferences.h"
#include "../utils/log_utils.h"

using namespace std;
using namespace utils;
using namespace my_struct;
using namespace storage;

#define LOGW(msg)  utils::LogUtil::LOGW(msg)

HashMap* SharePreferences::shareMap = NULL;

const char* SharePreferences::file_path = const_cast<char *>("/data/data/com.wechat.mylogin/hello.txt");
const char* SharePreferences::dir_path = const_cast<char *>("/data/data/com.wechat.mylogin/");
const char* SharePreferences::file_name = const_cast<char *>("hello.txt");

/**
 * 保存键值对
 */
bool SharePreferences::save(string key, string value) {
    if (shareMap == NULL){
        readFileToSharePref();
        if (shareMap == NULL){
            shareMap = new HashMap(10);
        }
    }
    return shareMap->insert(key,value);
}

/**
 * 保存键值对
 * 并执行文件更新
 */
bool SharePreferences::saveNow(string key, string value) {
    if (shareMap == NULL){
        readFileToSharePref();
        if (shareMap == NULL){
            shareMap = new HashMap(10);
        }
    }
    bool result = shareMap->insert(key,value);
    if (!execute()){
        result = false;
    }
    return result;
}

/**
 * 执行Map数据更新到文件
 */
bool SharePreferences::execute() {
    if (shareMap == NULL){
        readFileToSharePref();
        if (shareMap == NULL){
            shareMap = new HashMap(10);
        }
    }
    if (!writeFileToSharePref()){
        LOGW("[share_pref]when save kv, share file save fail");
        return false;
    }
    return true;
}

/**
 * 读取键值对
 */
string SharePreferences::get(string key) {
    if (shareMap == NULL){
        readFileToSharePref();
        if (shareMap == NULL){
            shareMap = new HashMap(10);
        }
    }
    return shareMap->find(key);
}

/**
 * 读取键值对
 */
int32_t SharePreferences::getInt32(string key) {
    string value = get(key);
    stringstream ss;
    ss << value;
    int32_t result;
    ss >> result;
    return result;
}

/**
 * 从文件读取键值对信息到内存中
 */
bool SharePreferences::readFileToSharePref() {
    if (!checkAndCreateSharePrefFile()){
        return false;
    }
    if (shareMap != NULL){
        shareMap->clean();
        delete(shareMap);
        shareMap = NULL;
    }
    shareMap = new HashMap(10);

    std::string content = File::readFile(file_path);

    if (content.empty()){
        return false;
    }

    std::vector<std::string> v;
    utils::String::splitString(content, v,"\n");

    for (string kv : v){
        std::vector<std::string> kvs;
        utils::String::splitString(kv, kvs," : ");
        if (kvs.size() == 2){
            shareMap->insert(kvs[0],kvs[1]);
        }
    }
    return true;
}

/**
 * 从内存往文件写入键值对信息
 */
bool SharePreferences::writeFileToSharePref() {
    if (!checkAndCreateSharePrefFile()){
        return false;
    }
    if (shareMap == NULL){
        return true;
    }
    std::string content = shareMap->list();
    if (!File::writeFile(file_path,content.data())){
        LOGW("[share_pref]file write fail");
        return false;
    }
    return true;
}

/**
 * 检查键值对信息文件是否存在，不存在则创建
 */
bool SharePreferences::checkAndCreateSharePrefFile() {
    const char* filePath = file_path;
    if (!File::isFileExist(filePath)){
        if (!File::createFile(dir_path,file_path)){
            LOGW("[share_preferences]create file fail");
            return false;
        }
    }
    return true;
}

