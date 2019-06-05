
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <exception>
#include <vector>
#include <sstream>
#include <sstream>
#include <mysql/mysql.h>

#include "db_manager.h"
#include "source/cpp/utils/log_utils.h"

#define LOGD(msg) utils::LogUtil::LOGD(msg);
#define LOGW(msg) utils::LogUtil::LOGW(msg);
#define LOGI(msg) utils::LogUtil::LOGI(msg);
#define LOGE(msg) utils::LogUtil::LOGE(msg);

using namespace manager;
using namespace std;
using namespace my_model;

Database *Database::database = new Database();

Database *Database::getDatabase()
{
    return Database::database;
}

Database::Database()
{
    
}

/**
 * 数据库初始化
**/
void Database::connect(ServerConfig _conf)
{
    if (isHadInit)
    {
        LOGW("database had init !");
        return;
    }
    isHadInit = true;

    // 根据配置文件打开数据库连接。如果数据库不存在，则创建。
    string msg;

    if (Database::db_base->connect(
            _conf.getMySqlHost(),
            _conf.getMySqlPort(),
            _conf.getMySqlDBName(),
            _conf.getMySqlUser(),
            _conf.getMySqlPassword(),
            _conf.getMySqlCharset(),
            msg) == 0)
    {
        LOGD("db connect success !");
        //打开数据库成功，检查表是否存在
        Database::checkAndCreateTable(Database::TABLE_USER_ACCOUNT);
    }
    else
    {
        LOGE("db connect fail");
        LOGD(msg);
    }
}

/**
 * 插入数据
**/
bool Database::insertDbAccount(string tabls_name, std::vector<string> v_key, std::vector<string> v_value)
{

    //检查数据是否正确
    if (tabls_name.empty() || v_key.size() == 0 || v_value.size() == 0)
    {
        LOGW("db insert , param is invalid !");
        return false;
    }

    //封装数据
    string str_key = v_key[0];
    for (unsigned int i = 1; i < v_key.size(); i++)
    {
        str_key += "," + v_key[i];
    }

    string str_value = v_value[0];
    for (unsigned int i = 1; i < v_value.size(); i++)
    {
        str_value += "," + v_value[i];
    }

    //获得插入SQL语句
    const int bufSize = 1024;
    char c_sql[bufSize];
    try
    {
        snprintf(c_sql, bufSize, "INSERT INTO %s(%s) VALUES(%s);", tabls_name.c_str(), str_key.c_str(), str_value.c_str());
    }
    catch (exception &e)
    {
        LOGW("db insert , sqlSentence create fail");
        return false;
    }

    string msg;
    int result = db_base->insertData(c_sql,msg);
    if(result != 0){
        LOGE(msg);
    }

    return result == 0;
}

/**
 * 更新数据
**/
bool Database::updateDbAccount(string tabls_name, std::vector<string> v_key, std::vector<string> v_value, std::vector<string> where_key, std::vector<string> where_value)
{
    //检查数据是否正确
    if (tabls_name.empty() || v_key.size() == 0 || v_value.size() == 0)
    {
        LOGW("db update fail, param is invalid !");
        return false;
    }

    if (v_key.size() != v_value.size() || where_key.size() != where_value.size())
    {
        LOGW("db update fail, param is invalid !");
        return false;
    }

    //封装数据
    string str_key = v_key[0];
    for (unsigned int i = 1; i < v_key.size(); i++)
    {
        str_key += "," + v_key[i];
    }

    string str_value = v_value[0];
    for (unsigned int i = 1; i < v_value.size(); i++)
    {
        str_value += "," + v_value[i];
    }

    //获得更新SQL语句
    string str_sql = "UPDATE " + tabls_name + " SET ";

    //set值组装
    str_sql += v_key[0] + " = " + v_value[0];
    for (unsigned int i = 1; i < v_key.size(); i++)
    {
        str_sql += ", " + v_key[i] + " = " + v_value[i];
    }

    //where条件组装
    if (where_key.size() != 0)
    {
        str_sql += " WHERE " + where_key[0] + " = " + where_value[0];
    }
    for (unsigned int i = 1; i < where_key.size(); i++)
    {
        str_sql += "and " + where_key[i] + " = " + where_value[i];
    }

    str_sql += ";";

    string msg;
    bool isSuccess = db_base->updateData(str_sql.c_str(),msg);
    if(!isSuccess){
        LOGE(msg);
    }else{
        LOGD("update sql success");
    }
    return isSuccess;
}

void Database::checkAndCreateTable(string tableName)
{

    string str_sql_table = "SELECT table_name FROM information_schema.TABLES WHERE table_name ='"+tableName+"';";
    if (!db_base->isExist(str_sql_table, 3,tableName))
    {
        string str_sql = "CREATE TABLE " + tableName + "(           \
            ID          INT PRIMARY KEY AUTO_INCREMENT   NOT NULL,   \
            ACCOUNT     CHAR(50)                            NOT NULL,   \
            PASSWORD    CHAR(50)                            NOT NULL    \
            );";
        
        if(db_base->createdbTable(str_sql)){
            LOGD("create tables success");
        }else{
            LOGE("create tables fail");
        }
    }
}

/***************************以下是对外的接口**************************/


bool Database::addUserAccount(string account, string password)
{
    if (account.empty() || password.empty())
    {
        LOGW("db | addUserAccount | param is empty");
        return false;
    }

    vector<string> v_key;
    v_key.push_back("ACCOUNT");
    v_key.push_back("PASSWORD");

    vector<string> v_value;
    v_value.push_back("'" + account + "'");
    v_value.push_back("'" + password + "'");

    return insertDbAccount(TABLE_USER_ACCOUNT, v_key, v_value);
}

bool Database::isUserExist(string account)
{
    if (account.empty())
    {
        LOGW("db | isUserExist | param is empty");
        return false;
    }

    string str_sql = "select * from " + TABLE_USER_ACCOUNT + " where ACCOUNT = '" + account + "';";
    return Database::db_base->isExist(str_sql,3,TABLE_USER_ACCOUNT);
}

UserAccount Database::queryUserAccountByAccount(string o_account)
{
    if (o_account.empty())
    {
        LOGW("db | queryUserAccountByAccount | param is empty");
        return UserAccount(-1, "", "");
    }

    int uid = -1;
    string account;
    string password;

    //获得SQL语句
    string str_sql = "SELECT ID , ACCOUNT , PASSWORD FROM " + TABLE_USER_ACCOUNT + "  WHERE ACCOUNT = '" + o_account + "' ;";
    
    string msg;
    Json::Value data = db_base->selectData(str_sql.c_str(),3,TABLE_USER_ACCOUNT,msg);
    if(data.empty()){
        LOGE("select user_account fail | account = " + o_account);
    }else{
        Json::FastWriter fw;
        LOGD(fw.write(data));
    }

    return UserAccount(uid, account, password);
}
