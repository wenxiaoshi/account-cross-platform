/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <stdexcept>
#include <stdlib.h>

#include <grpcpp/grpcpp.h>

#include "hash_map.h"
#include "common_utils.h"
#include <set>

#ifdef BAZEL_BUILD
#include "source/protos/account.grpc.pb.h"
#else
#include "account.grpc.pb.h"
#endif

// #include <boost/scoped_ptr.hpp>
// #include "source/mysql/jdbc/mysql_driver.h"
// #include "source/mysql/jdbc/mysql_connection.h"
// #include "source/mysql/jdbc/cppconn/resultset.h"
// #include "source/mysql/jdbc/cppconn/statement.h"

using namespace std;
using namespace my_struct;
using namespace utils;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using account::LoginRequest;
using account::SignRequest;
using account::ConnectRequest;
using account::LogoutRequest;
using account::CodeReply;
using account::Account;

class HandleResult{
private:
  int code = 0;
  std::string msg = "";
  std::string data = "";
public:
  int getCode(){
    return code;
  };
  void setCode(int co){
    code = co;
  };
  std::string getMsg(){
      return msg;
  };
  void setMsg(std::string message){
    msg = message;
  };
  std::string getData(){
    return data;
  };
  void setData(std::string da){
    data = da;
  };
};

class LoginDatabase {
public:

   /**
  * 这部分往后需要改用数据库存储，现抽离出来，便于往后改造不影响其他模块
  **/

  /**
  * 判断是否已经注册
  **/
  bool isHadSign(string phone){
    return !mMapPhoneAndPsw.find(phone).empty();
  };
  
  /**
  * 判断密码是否正确
  **/
  bool isPasswordCorrect(string phone,string password){
    return strncmp(mMapPhoneAndPsw.find(phone).c_str(),password.data(),mMapPhoneAndPsw.find(phone).size()) == 0;
  }

  /**
  * 更新账号对应的设备Id
  **/
  bool updateToken(string phone,string token){
    removeAccountDeviceId(phone);
    mMapPhoneAndDid.insert(phone, token);
    return true;
  }

  /**
  * 清除账号对应的设备Id
  **/
  bool removeAccountDeviceId(string phone){
    string token = mMapPhoneAndDid.find(phone);
    mMapPhoneAndDid.remove(phone);
    std::cout << "removeAccountToken " << token << std::endl;
    return true;
  }

  /**
  * 判断某设备是否联通
  **/
  bool judeDeviceIdOnline(string phone, string token){
    string tokenSys = mMapPhoneAndDid.find(phone);
    std::cout << "judeDeviceIdOnline" << tokenSys << "/" << token << std::endl;
    return isStringEqual(tokenSys,token);
  }

  /**
  * 新增账号
  **/
  bool insertAccountToDB(string phone,string password){
    mMapPhoneAndPsw.insert(phone, password);
    return true;
  }
private:
  static HashMap mMapPhoneAndPsw;
  static HashMap mMapPhoneAndDid;

  bool isStringEqual(string origin,string target){
    const char* oChar = origin.data();
    const char* tChar = target.data();
    if (origin.size() != target.size())
    {
      return false;
    }
    for (std::size_t i = 0; i < origin.size(); ++i)
    {
      if (oChar[i] != tChar[i])
      {
        return false;
      }
    }
    return true;
  }
};

HashMap LoginDatabase::mMapPhoneAndPsw(100);
HashMap LoginDatabase::mMapPhoneAndDid(100);

class LoginCore{
public:
  HandleResult handleUserLogin(std::string account,std::string password){
    HandleResult result;
    
    LoginDatabase db;

    //查询账号是否存在
    if(!db.isHadSign(account)){
      result.setCode(2000);
      result.setMsg("该账号不存在");
      return result;
    }

     //查询密码是否正确
     if(!db.isPasswordCorrect(account,password)){
      result.setCode(2001);
      result.setMsg("密码错误");
      return result;
    }

    //get from database
    unsigned long uid = 1000; 
    //create token
    string token = CommonUtils::GenToken(1000,account);

    //更新账号对应的设备ID
    if (!db.updateToken(account,token))
    {
      result.setCode(2003);
      result.setMsg("更新账号的token失败");
      return result;
    }

    result.setCode(0);

    
    return result;
  };
   HandleResult handleUserSign(std::string account,std::string password){
    HandleResult result;
    
    LoginDatabase db;
    
    //查询账号是否存在
    if(db.isHadSign(account)){
      result.setCode(2002);
      result.setMsg("该账号已注册");
      return result;
    }

     
    //新增账号
    if (!db.insertAccountToDB(account,password)){
      result.setCode(2004);
      result.setMsg("新增账号失败");
      return result;
    }

    //get from database
    unsigned long uid = 1000; 
    //create token
    string token = CommonUtils::GenToken(1000,account);

    //更新账号对应的设备ID
    if (!db.updateToken(account,token)){
      result.setCode(2003);
      result.setMsg("更新账号的token失败");
      return result;
    }

    result.setCode(0);

    
    return result;
  };


   HandleResult handleUserLogout(string token){
    HandleResult result;
  
    //check token is valid or not
    //is invalid ,then return
    string decodeToken = CommonUtils::DecryptToken(token);
    if (decodeToken.empty())
    {
      result.setMsg("token不合法");
      return result;
    }
    
    std::vector<string> vToken;
    CommonUtils::splitString(decodeToken, vToken, ":");
    if (vToken.size() != 5)
    {
      result.setMsg("token不合法");
      return result;
    }

    //get account by token
    string uid = vToken[0]
    string account = vToken[1];

    //todo check this account is valid or not

    LoginDatabase db;

    //查询账号是否存在
    if(!db.isHadSign(account)){
      result.setMsg("账号不存在");
      return result;
    }

    //清除账号对应的设备ID
    if (!db.removeAccountDeviceId(account)){
      result.setMsg("更新账号的token失败");
      return result;
    }
    
    return result;
  };
 HandleResult handleUserCheckConnect(std::string token){
    HandleResult result;
  
    //check token is valid or not
    //is invalid ,then return
    string decodeToken = CommonUtils::DecryptToken(token);
    if (decodeToken.empty())
    {
      result.setMsg("token不合法");
      return result;
    }
    
    std::vector<string> vToken;
    CommonUtils::splitString(decodeToken, vToken, ":");
    if (vToken.size() != 5)
    {
      result.setMsg("token不合法");
      return result;
    }

    //get account by token
    string uid = vToken[0]
    string account = vToken[1];

    LoginDatabase db;

    //检核设备是否在线
    if (!db.judeDeviceIdOnline(account, token)){
      result.setCode(2005);
      result.setMsg("该设备不在线");
      return result;
    }

    result.setCode(0);


    return result;
  };

 

};



class AccountServiceImpl final : public Account::Service {

  Status requestUserLogin(ServerContext* context, const LoginRequest* request,
                  CodeReply* reply) override {
    std::cout << "登录请求-入口" << std::endl;
    
    LoginCore loginCore;
    HandleResult result = loginCore.handleUserLogin(request->account(),request->password());
    
    reply->set_code(result.getCode());
    reply->set_msg(result.getMsg());
    reply->set_data(result.getData());
    std::cout << "登录请求-结束" << std::endl;
    return Status::OK;
  }

  Status requestUserSign(ServerContext* context, const SignRequest* request,
                  CodeReply* reply) override {
    std::cout << "注册请求-入口" << std::endl;
        
    LoginCore loginCore;
    HandleResult result = loginCore.handleUserSign(request->account(),request->password());
    
    reply->set_code(result.getCode());
    reply->set_msg(result.getMsg());
    reply->set_data(result.getData());
    std::cout << "注册请求-结束" << std::endl;
    return Status::OK;
  }

  Status requestLogout(ServerContext* context, const LogoutRequest* request,
                  CodeReply* reply) override {
    std::cout << "下线请求-入口" << std::endl;
        
    LoginCore loginCore;
    HandleResult result = loginCore.handleUserLogout(request->token());
    
    reply->set_code(result.getCode());
    reply->set_msg(result.getMsg());
    reply->set_data(result.getData());
    std::cout << "下线请求-结束" << std::endl;
    return Status::OK;
  }

  Status checkConnect(ServerContext* context, const ConnectRequest* request,
                  CodeReply* reply) override {
    std::cout << "连线检测请求-入口" << std::endl;

    LoginCore loginCore;
    HandleResult result = loginCore.handleUserCheckConnect(request->token());
    
    reply->set_code(result.getCode());
    reply->set_msg(result.getMsg());
    reply->set_data(result.getData());
    std::cout << "连线检测请求-结束" << std::endl;
    return Status::OK;
  }
};


// int RunServer2() {
//   const char   *url = ("tcp://127.0.0.1");
//   const string user("root");
//   const string pass("");
//   const string database("test");

//    cout << endl;
//   cout << "Connector/C++ standalone program example..." << endl;
//   cout << endl;
//   try {
//     sql::Driver * driver = sql::mysql::get_driver_instance();
//     /* Using the Driver to create a connection */
//     cout << "Creating session on " << url << " ..."
//          << endl << endl;
//     boost::scoped_ptr< sql::Connection >
//       con(driver->connect(url, user, pass));
//     con->setSchema(database);
//     boost::scoped_ptr< sql::Statement > stmt(con->createStatement());
//     boost::scoped_ptr< sql::ResultSet >
//       res(stmt->executeQuery("SELECT 'Welcome to Connector/C++' AS _message"));
//     cout << "\t... running 'SELECT 'Welcome to Connector/C++' AS _message'"
//          << endl;
//     while (res->next())
//     {
//       cout << "\t... MySQL replies: " << res->getString("_message") << endl;
//       cout << "\t... say it again, MySQL" << endl;
//       cout << "\t....MySQL replies: " << res->getString(1) << endl;
//     }
//   }
//   catch (sql::SQLException &e)
//   {
    /*
      The JDBC API throws three different exceptions:
    - sql::MethodNotImplementedException (derived from sql::SQLException)
    - sql::InvalidArgumentException (derived from sql::SQLException)
    - sql::SQLException (derived from std::runtime_error)
    */
//     cout << "# ERR: SQLException in " << __FILE__;
//     cout << "(" << "EXAMPLE_FUNCTION" << ") on line " << __LINE__ << endl;
//     /* Use what() (derived from std::runtime_error) to fetch the error message */
//     cout << "# ERR: " << e.what();
//     cout << " (MySQL error code: " << e.getErrorCode();
//     cout << ", SQLState: " << e.getSQLState() << " )" << endl;
//     return EXIT_FAILURE;
//   }
//   cout << endl;
//   cout << "... find more at http://www.mysql.com" << endl;
//   cout << endl;
//   return EXIT_SUCCESS;

// }

void RunServer() {

  std::string server_address("0.0.0.0:50051");
  AccountServiceImpl service;

  ServerBuilder builder;
  
  std::ifstream skey("mypem/server.key");
  std::string strServerKey((std::istreambuf_iterator<char>(skey)),std::istreambuf_iterator<char>());
  std::cout << "key: " <<strServerKey << std::endl;
  std::ifstream sCrt("mypem/server.crt");  
  std::string strServerCert((std::istreambuf_iterator<char>(sCrt)),std::istreambuf_iterator<char>());
  std::cout << "crt: " << strServerCert << std::endl;
  
  grpc::SslServerCredentialsOptions::PemKeyCertPair pkcp ={strServerKey.c_str(),strServerCert.c_str()};
  
  grpc::SslServerCredentialsOptions ssl_opts;
  ssl_opts.pem_root_certs="";
  ssl_opts.pem_key_cert_pairs.push_back(pkcp);
  std::shared_ptr<grpc::ServerCredentials> creds = grpc::SslServerCredentials(ssl_opts);

  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, creds);
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
   RunServer();
   return 0;
}
