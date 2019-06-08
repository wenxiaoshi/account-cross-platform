
#include "stdio.h"
#include "db_base.h"
 
#include "source/cpp/utils/log_utils.h"

#define LOGD(msg) utils::LogUtil::LOGD(msg);
#define LOGW(msg) utils::LogUtil::LOGW(msg);
#define LOGI(msg) utils::LogUtil::LOGI(msg);
#define LOGE(msg) utils::LogUtil::LOGE(msg);

DBBase::DBBase()
{
}
 
DBBase::~DBBase()
{
}
 
//初始化数据
int DBBase::connect(string host,int port ,string Db,string user,string passwd,string charset,string & Msg)
{
       if( mysql_init(&mysql) == NULL )
       {
              Msg = "inital mysql handle error";
              return 1;
       }    
 
       if (mysql_real_connect(&mysql,host.c_str(),user.c_str(),passwd.c_str(),Db.c_str(),port,NULL,0) == NULL)
       {
              Msg = "Failed to connect to database: Error";
              return 1;
       }    
 
       if(mysql_set_character_set(&mysql,"UTF8") != 0)
       {
              Msg = "mysql_set_character_set Error";
              return 1;
       }
       return 0;
}
 
//创建表
bool DBBase::createdbTable(const std::string& query)
{
    if (0 == mysql_query(&mysql,query.c_str()))
    {
        return true;
    }
    DBBase::errorIntoMySQL();
    return false;
}

//查询数据
Json::Value DBBase::selectData(const char * SQL,string tableName,string & Msg){
       vector<string> columnsV = DBBase::findColumns(tableName, Msg);
       if(columnsV.empty()){
              Msg = "select columns Error";
              return "";
       }
       return DBBase::selectData(SQL,columnsV,Msg);
}

//查询数据
Json::Value DBBase::selectData(const char * SQL,vector<string> columnsV,string & Msg)
{
       if(SQL == NULL){
              Msg = "sql is NULL";
              return "";
       }
	LOGD("sql : " + (string)SQL);

       MYSQL_ROW m_row;
       MYSQL_RES *m_res;
       char sql[2048];
       sprintf(sql,SQL);

       if(mysql_query(&mysql,sql) != 0)
       {
              Msg = "select ps_info Error";
              return "";
       }
       m_res = mysql_store_result(&mysql);
 
       if(m_res==NULL)
       {
              Msg = "select username Error";
              return "";
       }
       
       Json::Value root;
       while(m_row = mysql_fetch_row(m_res))
       {
              Json::Value data;
              for(uint i = 0;i < columnsV.size();i++)
              {
		      data[columnsV[i]] = m_row[i];
              }
              root["data_array"].append(data);
       }
 
       mysql_free_result(m_res);

       // Json::FastWriter fw;
       // LOGD(fw.write(root));

       return root;
}

//查询表的键
vector<string> DBBase::findColumns(string tableName,string & Msg){
       vector<string> columns;
       MYSQL_ROW m_row;
       MYSQL_RES *m_res;
       string sql_columns_find = "show columns from " + tableName + ";";
       if(mysql_query(&mysql,sql_columns_find.c_str()) != 0)
       {
              Msg = "select columns Error";
              return columns;
       }

       m_res = mysql_store_result(&mysql);
 
       if(m_res==NULL)
       {
              Msg = "select columns Error";
              return columns;
       }

       string str("");
       while(m_row = mysql_fetch_row(m_res))
       {
              columns.push_back(m_row[0]);
       }
       mysql_free_result(m_res);
       return columns;
}

//插入数据
int DBBase::insertData(char * SQL,string & Msg)
{
       if(SQL == NULL){
              Msg = "sql is NULL";
              return 1;
       }

       if(mysql_query(&mysql,SQL) != 0)
       {
              Msg = "Insert Data Error";
              return 1;
       }
       return 0;
}

//更新数据
int DBBase::updateData(const char * SQL,string & Msg)
{
       if(SQL == NULL){
              Msg = "sql is NULL";
              return 1;
       }

       if(mysql_query(&mysql,SQL) != 0)
       {
              Msg = "Update Data Error";
              return 1;
       }
       return 0;
}

//删除数据
int DBBase::DeleteData(char * SQL,string & Msg)
{
       if(SQL == NULL){
              Msg = "sql is NULL";
              return 1;
       }

       if(mysql_query(&mysql,SQL) != 0)
       {
              Msg = "Delete Data error";
              return 1;
       }
       return 0;
}

//错误信息打印
 void DBBase::errorIntoMySQL()
{
    //int errorNum = mysql_errno(&mysql);
    string errorInfo = mysql_error(&mysql);
    LOGE(errorInfo);
}

//关闭数据库连接
void DBBase::CloseMySQLConn()
{
       mysql_close(&mysql);
}

//判断SQL查询结果是否存在
bool DBBase::isExist(string str_sql,string tableName)
{
    string msg;
    Json::Value data = selectData(str_sql.c_str(), tableName, msg);

       Json::FastWriter fw;
       LOGD(fw.write(data));

    return data!="";
}

//判断SQL查询结果是否存在
bool DBBase::isExist(string str_sql,vector<string> columnsV)
{
    string msg;
    Json::Value data = selectData(str_sql.c_str(), columnsV, msg);
    return data!="";
}
