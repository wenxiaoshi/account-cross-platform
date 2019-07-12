#include <stdio.h>
#include <string.h>
#include "mysql/mysql.h"
#include "source/libs/json/json.h"
#include "source/cpp/struct/read_write_lock.h"

using namespace std;
class DBBase
{
public:
       //变量
       MYSQL mysql;

       /*
       构造函数和稀构函数
       */
       DBBase();
       ~DBBase();

       /*
       主要的功能：
       初始化数据库
       连接数据库
       设置字符集
 
       入口参数：
       host ：MYSQL服务器IP
       port:数据库端口
       Db：数据库名称
       user：数据库用户
       passwd：数据库用户的密码
       charset：希望使用的字符集
       Msg:返回的消息，包括错误消息
 
       出口参数：
       int ：0表示成功；1表示失败
       */
       int connect(string host, int port, string Db, string user, string passwd, string charset, string &Msg);

       /*
       主要功能：
       关闭数据库连接
       */
       void CloseMySQLConn();

       /*
       主要的功能：
       根据用户账号返回用户信息
 
       入口参数：
       account：用户账号
       Msg:返回的消息，包括错误消息
 
       出口参数：
       Json::Value 准备放置返回的数据，JSON对象
       如果 返回的用户UID小于0，责表示无结果

       存储过程说明：
       create procedure queryaccount(
              in in_account        varchar(50),         #用户账号

              out out_id           int,                 #用户UID
              out out_account      varchar(50),         #用户账号
              out out_password     varchar(128),        #用户密码
              out out_pwd_salt     varchar(32)          #参与密码初始化的盐值
       )
       label_a:begin
              declare v_id         int;
              declare v_account    varchar(50);
              declare v_password   varchar(128);
              declare v_pwd_salt   varchar(32);

              #参数判断
              if (in_account is NULL) then
                     set out_id=-1;#account error
                     leave label_a;
              end if;
              
              SELECT ID,ACCOUNT,PASSWORD,PWD_SALT into v_id,v_account,v_password,v_pwd_salt from user_account where ACCOUNT = in_account limit 1;

              if v_account is NULL then
                     set out_id=-1;#don't found
                     leave label_a;
              end if;    
              
              set out_id=v_id;
              set out_account=v_account;
              set out_password=v_password;
              set out_pwd_salt=v_pwd_salt;
       end;
       */
       Json::Value selectUserAccountByAccount(string account, string &Msg);

       /*
       主要的功能：
       将用户数据插入数据库
 
       入口参数：
       account：用户账号
       password:用户密码
       Msg:返回的消息，包括错误消息
 
       出口参数：
       Json::Value 准备放置返回的数据，JSON对象
       如果 返回的用户UID小于0，责表示无结果

       存储过程说明：
       create or replace procedure insertaccount(
              in in_account        varchar(50),         #用户账号
              in in_password       varchar(128),        #用户密码
              in in_pwd_salt       varchar(32),         #参与密码初始化的随机盐
              out out_id           int                  #用户UID
       )
       label_a:begin
              declare v_id         int;
              declare v_account    varchar(50);

              #参数判断
              if (in_account is NULL) then
                     set out_id=-1;#account error
                     leave label_a;
              end if;

              if (in_password is NULL) then
                     set out_id=-2;#password error
                     leave label_a;
              end if;
              
              if (in_pwd_salt is NULL) then
                     set out_id=-3;#pwd_salt error
                     leave label_a;
              end if;

              INSERT INTO user_account(ACCOUNT,PASSWORD,PWD_SALT) VALUES(in_account,in_password,in_pwd_salt);
              SELECT ID,ACCOUNT into v_id,v_account from user_account where ACCOUNT = in_account limit 1;

              if v_account is NULL then
                     set out_id=-3;#don't found
                     leave label_a;
              end if;    
              
              set out_id=v_id;
       end;
       */
       Json::Value insertUserAccount(string account, string password, string pwdSalt, string &Msg);

       /*
       主要功能：
       判断sql查询内容是否存在

       入口参数
       SQL：查询的SQL语句
       columnsV:select语句的键数组
 
       出口参数：
       bool ：true表示存在；false表示不存在
       */
       bool isExist(string SQL, vector<string> columnsV);

       /*
       主要功能：
       判断sql查询内容是否存在

       入口参数
       SQL：查询的SQL语句
       tableName:select语句的表名
 
       出口参数：
       bool ：true表示存在；false表示不存在
       */
       bool isExist(string SQL, string tableName);

       /*
       主要功能：
       创建表

       入口参数
       SQL：创建的SQL语句
 
       出口参数：
       bool ：true表示存在；false表示不存在
       */
       bool createdbTable(const std::string &SQL);

private:

       //读写锁
       static ReadWriteLock* rwlock;

       /*
       主要功能：
       收集错误信息
       */
       void errorIntoMySQL();

       /*
       主要功能：
       查询对应表的所有键

       入口参数
       tableName：数据库表名
       Msg:返回的消息，包括错误消息

       出口参数：
       vector<string> ：表的键数组
       */
       vector<string> findColumns(string tableName, string &Msg);

       /*
       主要的功能：
       查询数据
 
       入口参数：
       SQL：查询的SQL语句
       tableName:查询的表名
       Msg:返回的消息，包括错误消息
 
       出口参数：
       Json::Value 准备放置返回的数据，JSON数组
       如果 返回的is_empty为true，责表示无结果
       */
       Json::Value selectData(const char *SQL, string tableName, string &Msg);

       /*
       主要的功能：
       查询数据
 
       入口参数：
       SQL：查询的SQL语句
       colunmsV:查询的列的KEY
       Msg:返回的消息，包括错误消息
 
       出口参数：
       Json::Value 准备放置返回的数据，JSON数组
       如果 返回的is_empty为true，责表示无结果
       */
       Json::Value selectData(const char *SQL, vector<string> colunmsV, string &Msg);

       /*
       主要功能：(暂无使用)
       修改数据
      
       入口参数
       SQL：查询的SQL语句
       Msg:返回的消息，包括错误消息
 
       出口参数：
       int ：0表示成功；1表示失败
       */
       int updateData(const char *SQL, string &Msg);

       /*
       主要功能：(暂无使用)
       删除数据
      
       入口参数
       SQL：查询的SQL语句
       Msg:返回的消息，包括错误消息
 
       出口参数：
       int ：0表示成功；1表示失败
       */
       int deleteData(char *SQL, string &Msg);
};