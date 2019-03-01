
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

bool Database::init() {
	if (isHadInit) {
		cout << "warning : database had init !" << endl;
		return true;
	}
	isHadInit = true;

	const char * path = "source/db/user_sys.db";//某个sql文件的路径

    // 根据文件路径打开数据库连接。如果数据库不存在，则创建。
    // 数据库文件的路径必须以C字符串传入。
    int result = sqlite3_open_v2(path, &sql, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE, NULL);

    if (result == SQLITE_OK) {
        cout << "打开数据库连接成功" << endl;
        return true;
    } else {
        cout << "打开数据库连接失败" << endl;
        return false;
    }

}

int Database::callback(void* NotUsed, int nCount, char** pValue, char** pName) {
	string s;
	for(int i=0;i<nCount;i++) {
        s+=pName[i];
		s+=":";
		s+=pValue[i];
		s+="\n";
	}
	cout<<s<<endl;
	return 0;
}


bool Database::create() {

	const char* sqlSentence = "CREATE TABLE USER_ACCOUNT(    \
	   ID 		INTEGER PRIMARY KEY AUTOINCREMENT	NOT NULL,  		 \
	   PHONE        CHAR(50)    				NOT NULL,           	 \
	   PASSWORD     CHAR(50)     				NOT NULL           );";

	/* Execute SQL statement */
    ret = sqlite3_exec(sql, sqlSentence, NULL, NULL, &zErrMsg);

    if( ret != SQLITE_OK ){
        cout << "error : create table USER_ACCOUNT fail" << zErrMsg <<  endl;
        sqlite3_free(zErrMsg);
	return false;
    }
    
    cout << "info : create table USER_ACCOUNT success" << endl;
    
	return true;
}
bool Database::insertAccount(string account, string password){
	string tableName = "USER_ACCOUNT";
	
	vector<string> v_key;
	v_key.push_back("PHONE");
	v_key.push_back("PASSWORD");

	vector<string> v_value;
        v_value.push_back("'" + account + "'");
        v_value.push_back("'" + password + "'");
	
	return insert(tableName, v_key, v_value);
}


bool Database::insert(string tabls_name, std::vector<string> v_key, std::vector<string> v_value){
	if (tabls_name.empty() || v_key.size() == 0 || v_value.size() == 0) {
	 	cout << "error : db insert , param is invalid !" << endl;
	 	return false;
	}

	string str_key = v_key[0];
	for (unsigned int i = 1; i < v_key.size(); i++) {
	   str_key += "," + v_key[i];
	}

	string str_value = v_value[0];
	for (unsigned int i = 1; i < v_value.size(); i++) {
	   str_value += "," + v_value[i];
	}

	const int bufSize = 1024;
        char c_sql[bufSize];

      try  
     {  
         snprintf(c_sql,bufSize,"INSERT INTO %s(%s) VALUES(%s);",tabls_name.c_str(),str_key.c_str(),str_value.c_str());
         cout << "info : insert sql " << c_sql << endl;
     }  
     catch (exception& e)  
     {  
         cout << "error : db insert , sqlSentence create fail " << e.what() << endl;  
     	 return false;
     }

     const char *sqlSentence = c_sql;        //SQL语句
//     sqlite3_stmt *stmt = NULL;        //stmt语句句柄

 //    //进行插入前的准备工作——检查语句合法性
 //    //-1代表系统会自动计算SQL语句的长度
 //    int result = sqlite3_prepare_v2(sql, sqlSentence, -1, &stmt, NULL);
    int result =  sqlite3_exec(sql, sqlSentence, NULL, NULL, &zErrMsg);
     if (result == SQLITE_OK) {
         cout << "info : insert success" << endl;
         //执行该语句
 //        sqlite3_step(stmt);
     }
     else {
         cout << "error : insert fail " << zErrMsg << endl;
         return false;
     }
 //    //清理语句句柄，准备执行下一个语句
 //    sqlite3_finalize(stmt);
	sqlite3_free(zErrMsg);
	return true;
}

bool Database::update(){
	return true;
}

bool Database::isTableExist(string tableName){
	string str_sql = "select count(*) from sqlite_master where type='table' and name='"+ tableName +"';";
	cout << str_sql << endl;
        const char *sqlSentence = str_sql.c_str();    //SQL语句
    sqlite3_stmt *stmt = NULL;    // stmt语句句柄

    //进行查询前的准备工作——检查语句合法性
    //-1代表系统会自动计算SQL语句的长度
    
    bool isExist = false;

    int result = sqlite3_prepare_v2(sql, sqlSentence, -1, &stmt, NULL);

    if (result == SQLITE_OK) {

             while (sqlite3_step(stmt) == SQLITE_ROW) {
                isExist = true;
            }
    }
    else {
        cout << "查询语句有问题" << endl;
    }
    //清理语句句柄，准备执行下一个语句
    sqlite3_finalize(stmt);
    return isExist;
}



bool Database::queryAccountId(string account){
	string str_sql = "SELECT ID FROM USER_ACCOUNT  WHERE PHONE = '" + account + "' ;";
	cout << str_sql << endl;
	const char *sqlSentence = str_sql.c_str();    //SQL语句
    sqlite3_stmt *stmt = NULL;    // stmt语句句柄

    //进行查询前的准备工作——检查语句合法性
    //-1代表系统会自动计算SQL语句的长度
    int result = sqlite3_prepare_v2(sql, sqlSentence, -1, &stmt, NULL);

    if (result == SQLITE_OK) {

            // 每调一次sqlite3_step()函数，stmt语句句柄就会指向下一条记录
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // 取出第0列字段的值
                int id = sqlite3_column_int(stmt, 0);
                //输出相关查询的数据
                cout << "uid = " << id << endl;
            }
    }
    else {
        cout << "查询语句有问题" << endl;
    }
    //清理语句句柄，准备执行下一个语句
    sqlite3_finalize(stmt);
}
