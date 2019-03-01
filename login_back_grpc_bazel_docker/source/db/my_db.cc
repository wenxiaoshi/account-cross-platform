
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <exception>

#include "my_db.h"

using namespace db_utils;
using namespace std;

Database* Database::database = new Database();; 

sqlite3* Database::sql = NULL;

bool Database::isHadInit = false;
char* Database::zErrMsg = NULL;
int Database::ret = 0;

Database::Database(){
	init();
}

Database::~Database(){
	delete sql;
}

bool Database::init(){
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

int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i = 0;
    for(i=0; i < argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    
    return 0;
}

bool Database::create(){

	char* sqlSentence = "CREATE TABLE USER_ACCOUNT(
	   ID INT PRIMARY KEY     		NOT NULL,
	   PHONE          CHAR(50)    	NOT NULL,
	   PASSWORD       CHAR(50)     	NOT NULL,
   	);"

	/* Execute SQL statement */
    ret = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( ret != SQLITE_OK ){
        sqlite3_free(zErrMsg);
        cout << "error : create table USER_ACCOUNT fail" << endl;
        return false;
    }
    
    cout << "info : create table USER_ACCOUNT success" << endl;
    
	return true;
}

bool Database::insert(string tabls_name, std::vector<string> v_key, std::vector<string> v_value){
	// if (tabls_name.empty || v_key.size() == 0 || v_value.size() == 0) {
	// 	cout << "error : db insert , param is invalid !" << endl;
	// 	return false;
	// }

	// string str_key = v_key[0];
	// for (int i = 1; i < v_key.size(); ++i) {
	// 	str_key += "," + v_key[0];
	// }

	// string str_value = v_value[0];
	// for (int i = 1; i < v_value.size(); ++i) {
	// 	str_value += "," + v_value[0];
	// }

	// const int bufSize = 1024;
 //    char c_sql[bufSize];

 //    try  
 //    {  
 //        snprintf(c_sql,bufSize,"INSERT INTO %s(%s) VALUES(%s);",tabls_name.c_str(),str_key.c_str(),str_value.c_str());
 //    }  
 //    catch (exception& e)  
 //    {  
 //        cout << "error : db insert , sqlSentence create fail " << e.what() << endl;  
 //    }

	// const char *sqlSentence = c_sql;        //SQL语句
 //    sqlite3_stmt *stmt = NULL;        //stmt语句句柄

 //    //进行插入前的准备工作——检查语句合法性
 //    //-1代表系统会自动计算SQL语句的长度
 //    int result = sqlite3_prepare_v2(sql, sqlSentence, -1, &stmt, NULL);

 //    if (result == SQLITE_OK) {
 //        cout << "info : insert success" << endl;
 //        //执行该语句
 //        sqlite3_step(stmt);
 //    }
 //    else {
 //        cout << "error : insert fail" << endl;
 //    }
 //    //清理语句句柄，准备执行下一个语句
 //    sqlite3_finalize(stmt);

	return true;
}

bool Database::update(){
	return true;
}

bool Database::query(){

	char* sqlSentence = "SELETE * from USER_ACCOUNT;"

	/* Execute SQL statement */
    ret = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( ret != SQLITE_OK ){
        sqlite3_free(zErrMsg);
        cout << "error : SELETE table USER_ACCOUNT fail" << endl;
        return false;
    }
    
    cout << "info : SELETE table USER_ACCOUNT success" << endl;

	return true;
}