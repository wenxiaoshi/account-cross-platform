
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <exception>
#include <vector>
#include <sstream>

#include "my_db.h"
#include "source/cpp/account/my_log.h"

#define LOGD(msg)  utils::LogUtil::LOGD(msg);
#define LOGW(msg)  utils::LogUtil::LOGW(msg);
#define LOGI(msg)  utils::LogUtil::LOGI(msg);
#define LOGE(msg)  utils::LogUtil::LOGE(msg);

using namespace db_utils;
using namespace std;
using namespace my_model;

Database* Database::database = new Database();; 

Database::Database(){
	init();
}

Database::~Database(){
	delete sql;
	sql = NULL;
}

/**
 * 数据库初始化
**/
bool Database::init() {
	if (isHadInit) {
		LOGW("database had init !");
		return true;
	}
	isHadInit = true;

    //sql文件的路径
	const char * path = "source/db/user_sys.db";

    // 根据文件路径打开数据库连接。如果数据库不存在，则创建。
    // 数据库文件的路径必须以C字符串传入。
    int result = sqlite3_open_v2(path, &sql, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE, NULL);

    if (result == SQLITE_OK) {
        LOGD("open db success !");
        //打开数据库成功，检查表是否存在
        checkAndCreateTable();
        return true;
    } else {
        LOGE("open db fail ! error_code = " + result);
        return false;
    }

}

/**
 * 检查表是否存在，不存在则创建
**/
void Database::checkAndCreateTable() {

    if (!isTableExist(TABLE_USER_ACCOUNT)) {
        string str_sql = "CREATE TABLE " + TABLE_USER_ACCOUNT + "(           \
            ID          INTEGER PRIMARY KEY AUTOINCREMENT   NOT NULL,   \
            ACCOUNT     CHAR(50)                            NOT NULL,   \
            PASSWORD    CHAR(50)                            NOT NULL    \
            );";
        const char* sqlSentence = str_sql.c_str();

        // 创建用户表
        ret = sqlite3_exec(sql, sqlSentence, NULL, NULL, &zErrMsg);
        // 创建失败，打印信息
        if( ret != SQLITE_OK ){
            LOGE("create table USER_ACCOUNT fail | " + string(zErrMsg));
            sqlite3_free(zErrMsg);
        } else {
            LOGD("create table USER_ACCOUNT success");
        }
    }
    
    if (!isTableExist(TABLE_USER_SESSION)) {
        if (isTableExist(TABLE_USER_ACCOUNT)) {
            string str_sql = "CREATE TABLE " + TABLE_USER_SESSION + "(           \
            ID          INTEGER PRIMARY KEY AUTOINCREMENT   NOT NULL,   \
            UID         INTEGER                             NOT NULL,   \
            TOKEN       CHAR(120)                           NOT NULL,    \
            IS_ONLINE   INTEGER                             NOT NULL,    \
            FOREIGN KEY(UID) REFERENCES " + TABLE_USER_ACCOUNT + "(ID)        \
            );";
            const char* sqlSentence = str_sql.c_str();

            // 创建用户表
            ret = sqlite3_exec(sql, sqlSentence, NULL, NULL, &zErrMsg);
            // 创建失败，打印信息
            if( ret != SQLITE_OK ){
                LOGE("create table USER_SESSION fail | " + string(zErrMsg));
                sqlite3_free(zErrMsg);
            } else {
                LOGD("create table USER_SESSION success");
            }
        } else {
            LOGE("create table TABLE_USER_SESSION fail | TABLE_USER_ACCOUNT is not exist");
        }
    }
}

/**
 * 根据SQL判断是否存在
**/
bool Database::isExist(string str_sql){
    
    //获得SQL语句
    const char *sqlSentence = str_sql.c_str();
    sqlite3_stmt *stmt = NULL;    // stmt语句句柄

    bool isExist = false;

    //检查SQL语句是否正确
    int result = sqlite3_prepare_v2(sql, sqlSentence, -1, &stmt, NULL);
    
    //执行SQL语句    
    if (result == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int count = sqlite3_column_int(stmt, 0);
            if(count > 0) {
                isExist = true;
            } 
        }
    } else {
        LOGW("sql is invalid !");
    }

    //清理语句句柄，准备执行下一个语句
    sqlite3_finalize(stmt);

    return isExist;
}

/**
 * 查询表是否存在
**/
bool Database::isTableExist(string tableName){
    string str_sql = "select count(*) from sqlite_master where type='table' and name='"+ tableName +"';";
    return isExist(str_sql);
}

/**
 * 插入数据
**/
bool Database::insert(string tabls_name, std::vector<string> v_key, std::vector<string> v_value){
    
    //检查数据是否正确
    if (tabls_name.empty() || v_key.size() == 0 || v_value.size() == 0) {
        LOGW("db insert , param is invalid !");
        return false;
    }

    //封装数据
    string str_key = v_key[0];
    for (unsigned int i = 1; i < v_key.size(); i++) {
       str_key += "," + v_key[i];
    }

    string str_value = v_value[0];
    for (unsigned int i = 1; i < v_value.size(); i++) {
       str_value += "," + v_value[i];
    }

    //获得插入SQL语句
    const int bufSize = 1024;
    char c_sql[bufSize];
    try {  
         snprintf(c_sql,bufSize,"INSERT INTO %s(%s) VALUES(%s);",tabls_name.c_str(),str_key.c_str(),str_value.c_str());
    } catch (exception& e) {  
         LOGW("db insert , sqlSentence create fail");
         return false;
    }
    const char *sqlSentence = c_sql;
    
    //执行插入语句
    int result =  sqlite3_exec(sql, sqlSentence, NULL, NULL, &zErrMsg);
    if (result == SQLITE_OK) {
        LOGD("db insert success");
    } else {
        LOGW("db insert fail | " + string(zErrMsg));
        return false;
    }

    sqlite3_free(zErrMsg);
    return true;
}

/**
 * 更新数据
**/
bool Database::update(string tabls_name, std::vector<string> v_key, std::vector<string> v_value, std::vector<string> where_key, std::vector<string> where_value){
	//检查数据是否正确
    if (tabls_name.empty() || v_key.size() == 0 || v_value.size() == 0) {
        LOGW("db update fail, param is invalid !");
        return false;
    }

    if (v_key.size() != v_value.size() || where_key.size() != where_value.size()) {
        LOGW("db update fail, param is invalid !");
        return false;
    }

    //封装数据
    string str_key = v_key[0];
    for (unsigned int i = 1; i < v_key.size(); i++) {
       str_key += "," + v_key[i];
    }

    string str_value = v_value[0];
    for (unsigned int i = 1; i < v_value.size(); i++) {
       str_value += "," + v_value[i];
    }

    //获得更新SQL语句
    string str_sql = "UPDATE " + tabls_name + " SET ";

    //set值组装
    str_sql += v_key[0] + " = " + v_value[0];
    for (unsigned int i = 1; i < v_key.size(); i++) {
        str_sql += ", " + v_key[i] + " = " + v_value[i];
    }

    //where条件组装
    if (where_key.size() != 0) {
        str_sql += " WHERE " + where_key[0] + " = " + where_value[0];
    }
    for (unsigned int i = 1; i < where_key.size(); i++) {
        str_sql += "and " + where_key[i] + " = " + where_value[i];
    }

    str_sql += ";";

    const char *sqlSentence = str_sql.c_str();
    
    //执行插入语句
    int result =  sqlite3_exec(sql, sqlSentence, NULL, NULL, &zErrMsg);
    if (result == SQLITE_OK) {
        LOGD("db update success");
    } else {
        LOGW("db update fail | " + string(zErrMsg));
        return false;
    }

    sqlite3_free(zErrMsg);
    return true;
}

/***************************以下是对外的接口**************************/

/**
 * 对外接口
 * 新增用户账号
**/
bool Database::addUserAccount(string account, string password){
    if (account.empty() || password.empty()) {
        LOGW("db | addUserAccount | param is empty");
        return false;
    }

    vector<string> v_key;
    v_key.push_back("ACCOUNT");
    v_key.push_back("PASSWORD");

    vector<string> v_value;
    v_value.push_back("'" + account + "'");
    v_value.push_back("'" + password + "'");
    
    return insert(TABLE_USER_ACCOUNT, v_key, v_value);
}

/**
 * 对外接口
 * 新增用户在线Session
**/
bool Database::addUserSession(int uid, string token){
    if (uid <= 0 || token.empty()) {
        LOGW("db | addUserSession | param is invalid");
        return false;
    }

    vector<string> v_key;
    v_key.push_back("UID");
    v_key.push_back("TOKEN");
    v_key.push_back("IS_ONLINE");
    stringstream ss;
    ss<<uid; 

    vector<string> v_value;
    v_value.push_back(ss.str());
    v_value.push_back("'" + token + "'");
    v_value.push_back("1");

    return insert(TABLE_USER_SESSION, v_key, v_value);
}

/**
 * 对外接口
 * 更新用户在线Session
**/
bool Database::updateUserSession(int uid, string token, bool isOnline){
    if (uid <= 0 || token.empty()) {
        LOGW("db | updateUserSession | param is invalid");
        return false;
    }

    vector<string> v_key;
    v_key.push_back("TOKEN");
    v_key.push_back("IS_ONLINE");

    vector<string> v_value;
    v_value.push_back("'" + token + "'");
    v_value.push_back(isOnline?"1":"0");
    
    vector<string> where_key;
    where_key.push_back("UID");

    vector<string> where_value;
    stringstream ss;
    ss<<uid; 
    where_value.push_back(ss.str());

    return update(TABLE_USER_SESSION, v_key, v_value, where_key, where_value);
}

/**
 * 对外接口
 * 根据用户账号
 * 判断用户是否存在
**/
bool Database::isUserExist(string account){
    if (account.empty()) {
        LOGW("db | isUserExist | param is empty");
        return false;
    }

    string str_sql = "select count(*) from " + TABLE_USER_ACCOUNT + " where ACCOUNT = '" + account + "';";
    return isExist(str_sql);
}

/**
 * 对外接口
 * 根据用户账号
 * 获得用户信息
**/
UserAccount Database::queryUserAccountByAccount(string o_account){
    if (o_account.empty()) {
        LOGW("db | queryUserAccountByAccount | param is empty");
        return UserAccount(-1, "", "");
    }

    int uid = -1;
    string account;
    string password;

    //获得SQL语句
    string str_sql = "SELECT ID , ACCOUNT , PASSWORD FROM " + TABLE_USER_ACCOUNT + "  WHERE ACCOUNT = '" + o_account + "' ;";
    const char *sqlSentence = str_sql.c_str();

    //stmt语句句柄
    sqlite3_stmt *stmt = NULL;
    
    //检查SQL语句是否合法，-1代表系统会自动计算SQL语句的长度
    int result = sqlite3_prepare_v2(sql, sqlSentence, -1, &stmt, NULL);

    //执行SQL语句
    if (result == SQLITE_OK) {
        // 每调一次sqlite3_step()函数，stmt语句句柄就会指向下一条记录
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            //这里理论上只会有一条用户数据
            uid = sqlite3_column_int(stmt, 0);
            account = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            password = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        }
    } else {
        LOGW("sql is invalid !");
    }

    //清理语句句柄，准备执行下一个语句
    sqlite3_finalize(stmt);

    return UserAccount(uid, account, password);
}

/**
 * 对外接口
 * 根据用户Uid
 * 获得用户Session
**/
UserSession Database::queryUserSessionByUid(int o_uid){
    if (o_uid <= 0) {
        LOGW("db | queryUserSessionByUid | param is invalid");
        return UserSession(-1, "", false);
    }

    int uid = -1;
    string token;
    bool isOnline = false;

    stringstream ss;
    ss<<o_uid;

    //获得SQL语句
    string str_sql = "SELECT UID , TOKEN , IS_ONLINE FROM " + TABLE_USER_SESSION + "  WHERE UID = '" + ss.str() + "' ;";
    const char *sqlSentence = str_sql.c_str();

    //stmt语句句柄
    sqlite3_stmt *stmt = NULL;
    
    //检查SQL语句是否合法，-1代表系统会自动计算SQL语句的长度
    int result = sqlite3_prepare_v2(sql, sqlSentence, -1, &stmt, NULL);

    //执行SQL语句
    if (result == SQLITE_OK) {
        // 每调一次sqlite3_step()函数，stmt语句句柄就会指向下一条记录
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            //这里理论上只会有一条用户Session
            uid = sqlite3_column_int(stmt, 0);
            token = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            isOnline = sqlite3_column_int(stmt, 2)==1;
        }
    } else {
        LOGW("sql is invalid !");
    }

    //清理语句句柄，准备执行下一个语句
    sqlite3_finalize(stmt);

    return UserSession(uid, token, isOnline);
}
