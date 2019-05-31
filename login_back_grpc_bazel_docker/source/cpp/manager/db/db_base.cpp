
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
string DBBase::selectData(const char * SQL,int Cnum,string & Msg)
{
       MYSQL_ROW m_row;
    MYSQL_RES *m_res;
    char sql[2048];
    sprintf(sql,SQL);
       int rnum = 0;
       char rg = 0x06;//行隔开
       char cg = {0x05};//字段隔开
 
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
       string str("");
       while(m_row = mysql_fetch_row(m_res))
       {
              for(int i = 0;i < Cnum;i++)
              {
                     str += m_row[i];
                     str += rg;
              }
              str += rg;             
              rnum++;
       }
 
       mysql_free_result(m_res);
 
       return str;
}

int DBBase::insertData(char * SQL,string & Msg)
{
       char sql[2048];
    sprintf(sql,SQL);
       if(mysql_query(&mysql,sql) != 0)
       {
              Msg = "Insert Data Error";
              return 1;
       }
       return 0;
}

int DBBase::updateData(const char * SQL,string & Msg)
{
       char sql[2048];
    sprintf(sql,SQL);
       if(mysql_query(&mysql,sql) != 0)
       {
              Msg = "Update Data Error";
              return 1;
       }
       return 0;
}

int DBBase::DeleteData(char * SQL,char * Msg)
{
       char sql[2048];
    sprintf(sql,SQL);
       if(mysql_query(&mysql,sql) != 0)
       {
              Msg = "Delete Data error";
              return 1;
       }
       return 0;
}


 void DBBase::errorIntoMySQL()
{
    int errorNum = mysql_errno(&mysql);
    string errorInfo = mysql_error(&mysql);
    LOGE(errorInfo);
}

void DBBase::CloseMySQLConn()
{
       mysql_close(&mysql);
}

bool DBBase::isExist(string str_sql,int num)
{
    string msg;
    string data = selectData(str_sql.c_str(), num, msg);
    return data!="";
}