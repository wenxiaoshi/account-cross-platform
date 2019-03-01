
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <exception>
#include <vector>

#include "my_db.h"

using namespace db_utils;
using namespace std;

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
		cout << "warning : database had init !" << endl;
		return true;
	}
	isHadInit = true;

    //sql文件的路径
	const char * path = "source/db/user_sys.db";

    // 根据文件路径打开数据库连接。如果数据库不存在，则创建。
    // 数据库文件的路径必须以C字符串传入。
    int result = sqlite3_open_v2(path, &sql, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE, NULL);

    if (result == SQLITE_OK) {
        cout << "info : open db success !" << endl;
        //打开数据库成功，检查表是否存在
        checkAndCreateTable();
        return true;
    } else {
        cout << "error : open db fail !" << endl;
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
            cout << "error : create table USER_ACCOUNT fail" << zErrMsg <<  endl;
            sqlite3_free(zErrMsg);
        }

        cout << "info : create table USER_ACCOUNT success" << endl;
    }
    
}

/**
 * 插入语句
**/
bool Database::insert(string tabls_name, std::vector<string> v_key, std::vector<string> v_value){
    
    //检查数据是否正确
    if (tabls_name.empty() || v_key.size() == 0 || v_value.size() == 0) {
        cout << "error : db insert , param is invalid !" << endl;
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
         cout << "info : insert sql " << c_sql << endl;
    } catch (exception& e) {  
         cout << "error : db insert , sqlSentence create fail " << e.what() << endl;  
         return false;
    }
    const char *sqlSentence = c_sql;
    
    //执行插入语句
    int result =  sqlite3_exec(sql, sqlSentence, NULL, NULL, &zErrMsg);
    if (result == SQLITE_OK) {
        cout << "info : insert success" << endl;
    } else {
        cout << "error : insert fail " << zErrMsg << endl;
        return false;
    }

    sqlite3_free(zErrMsg);
    return true;
}


bool Database::isTableExist(string tableName){
    string str_sql = "select count(*) from sqlite_master where type='table' and name='"+ tableName +"';";
    return isExist(str_sql);
}


bool Database::update(){
	return true;
}

/**
 * 根据SQL判断是否存在
**/
bool Database::isExist(string str_sql){
    cout << str_sql << endl;
    
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
            cout << count << endl;
            if(count > 0) {
	    	isExist = true;
	    }
	    
        }
    } else {
        cout << "error : sql is invalid !" << endl;
    }

    //清理语句句柄，准备执行下一个语句
    sqlite3_finalize(stmt);

    return isExist;
}

/**
 * 对外接口
 * 新增用户账号
**/
bool Database::addUserAccount(string account, string password){
    
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
 * 根据用户账号
 * 判断用户是否存在
**/
bool Database::isUserExist(string account){
    string str_sql = "select count(*) from "+ TABLE_USER_ACCOUNT +" where ACCOUNT = '" + account + "';";
    return isExist(str_sql);
}

/**
 * 对外接口
 * 根据用户账号
 * 获得用户UID
**/
int Database::queryAccountId(string account){
	int id = -1;

    //获得SQL语句
    string str_sql = "SELECT ID FROM " + TABLE_USER_ACCOUNT + "  WHERE ACCOUNT = '" + account + "' ;";
    const char *sqlSentence = str_sql.c_str();
    cout << "info : sql queryAccountId | " << str_sql << endl;

    //stmt语句句柄
    sqlite3_stmt *stmt = NULL;
    
    //检查SQL语句是否合法，-1代表系统会自动计算SQL语句的长度
    int result = sqlite3_prepare_v2(sql, sqlSentence, -1, &stmt, NULL);

    //执行SQL语句
    if (result == SQLITE_OK) {
        // 每调一次sqlite3_step()函数，stmt语句句柄就会指向下一条记录
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            //这里理论上只会有一条用户数据
            id = sqlite3_column_int(stmt, 0);
        }
    } else {
        cout << "error : sql is invalid !" << endl;
    }

    //清理语句句柄，准备执行下一个语句
    sqlite3_finalize(stmt);

    return id;
}
