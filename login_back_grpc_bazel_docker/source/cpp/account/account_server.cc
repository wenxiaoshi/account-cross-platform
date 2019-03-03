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
#include <vector>
#include <set>

#include <grpcpp/grpcpp.h>

#include "hash_map.h"
#include "common_utils.h"
#include "my_log.h"
#include "source/db/my_db.h"

#ifdef BAZEL_BUILD
#include "source/protos/account.grpc.pb.h"
#else
#include "account.grpc.pb.h"
#endif

using namespace std;
using namespace my_struct;
using namespace utils;
using namespace log_utils;
using namespace db_utils;
using namespace my_model;

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
  * 数据库工具类，对数据库进行操作
  **/

  /**
  * 判断是否已经注册
  **/
  bool isHadSign(string account){
    return Database::database->isUserExist(account);
  };
  
  /**
  * 新增账号
  **/
  bool insertAccountToDB(string account,string encrypt_password){
    return Database::database->addUserAccount(account, encrypt_password);
  }

  /**
  * 根据账号获取用户信息
  **/
  UserAccount getUserAccount(string account){
    return Database::database->queryUserAccountByAccount(account);
  }

  /**
  * 新增账号Session，创建账号时使用
  **/
  bool createUserSession(int uid, string token){
    return Database::database->addUserSession(uid, token);
  }

  /**
  * 更新账号Session，登录时更新
  **/
  bool onlineUserSession(int uid, string token){
    return Database::database->updateUserSession(uid, token, true);
  }

  /**
  * 更新账号Session，退出登录时更新
  **/
  bool offlineUserSession(int uid, string token){
    return Database::database->updateUserSession(uid, token, false);
  }

  /**
  * 判断某账号是否在线
  **/
  bool checkUserIdOnline(int uid, string token){
    UserSession userSession = Database::database->queryUserSessionByUid(uid);
    if (!userSession.isOnline()) {
      return false;
    }
    string db_token = userSession.getToken();
    return isEqual(token, db_token);
  }

 /**
  * 判断字符串是否相等
  **/
  bool isEqual(string origin,string target) {
    const char* oChar = origin.data();
    const char* tChar = target.data();
    if (origin.size() != target.size()) {
      return false;
    }
    for (unsigned int i = 0; i < origin.size(); i++) {
      if (oChar[i] != tChar[i]) {
        return false;
      }
    }
    return true;
  }

private:

  
};


class LoginCore{
public:
  HandleResult handleUserLogin(std::string account,std::string password){
    HandleResult result;
    
    LoginDatabase login_db;

    UserAccount userAccount = login_db.getUserAccount(account);

    //查询账号是否存在
    if(userAccount.getUid() <= 0) {
      result.setCode(2000);
      result.setMsg("该账号不存在");
      return result;
    }

    //查询密码是否正确
    //todo 获得加密后密码，进行校验
    string encrypt_password = password;
    string db_password = userAccount.getPassword();
    if(!login_db.isEqual(db_password, encrypt_password)) {
      result.setCode(2001);
      result.setMsg("密码错误");
      return result;
    }

    //获取用户UID
    int uid = userAccount.getUid(); 
    //生成Token
    string token = CommonUtils::GenToken(uid,account);

    //更新Token到数据库
    if (!login_db.onlineUserSession(uid,token)) {
      result.setCode(2003);
      result.setMsg("更新账号的token失败");
      return result;
    }

    //返回Token
    result.setCode(0);
    result.setData(token);
    return result;
  };

  HandleResult handleUserSign(std::string account,std::string password){
    HandleResult result;
    
    LoginDatabase login_db;
    
    //查询账号是否存在
    if(login_db.isHadSign(account)){
      result.setCode(2002);
      result.setMsg("该账号已注册");
      return result;
    }

    //todo 密码初始化，进行加密，再保存数据库
    string encrypt_password = password;

    //新增账号
    if (!login_db.insertAccountToDB(account,encrypt_password)){
      result.setCode(2004);
      result.setMsg("新增账号失败");
      return result;
    }

    //获得用户信息
    UserAccount userAccount = login_db.getUserAccount(account);
    if (userAccount.getUid() <= 0) {
      result.setCode(2004);
      result.setMsg("获取账号信息失败");
      return result;
    }

    //获得用户UID
    int uid = userAccount.getUid(); 
    
    //生成Token
    string token = CommonUtils::GenToken(uid, account);

    //新增账号Session
    if (!login_db.createUserSession(uid,token)){
      result.setCode(2003);
      result.setMsg("新增账号Session失败");
      return result;
    }

    //返回Token
    result.setCode(0);
    result.setData(token);
    
    return result;
  };


   HandleResult handleUserLogout(string token){
    HandleResult result;
  
    //解密Token
    string decodeToken = CommonUtils::DecryptToken(token);
    if (decodeToken.empty()) {
      result.setMsg("token不合法");
      return result;
    }
    
    //解析Token，获取用户信息
    vector<string> vToken;
    CommonUtils::splitString(decodeToken, vToken, ":");
    if (vToken.size() != 5) {
      result.setMsg("token不合法");
      return result;
    }

    //获得账号UID
    string str_uid = vToken[0];
    stringstream ss;
    ss<<str_uid;
    int uid;
    ss>>uid;

    //获取数据库操作类
    LoginDatabase login_db;

    //更新账号对应的Session信息
    if (!login_db.offlineUserSession(uid, token)){
      result.setMsg("更新账号的token失败");
      return result;
    }
    
    result.setCode(0);
    return result;
  };

HandleResult handleUserCheckConnect(std::string token){
    HandleResult result;
  
    //解密Token
    string decodeToken = CommonUtils::DecryptToken(token);
    if (decodeToken.empty()) {
      result.setCode(2006);
      result.setMsg("用户token不合法");
      return result;
    }
   
    //解析Token，获取用户信息
    std::vector<string> vToken;
    CommonUtils::splitString(decodeToken, vToken, ":");
    if (vToken.size() != 5) {
      result.setCode(2006);
      result.setMsg("用户token不合法");
      return result;
    }

    ///获得账号UID
    string str_uid = vToken[0];
    stringstream ss;
    ss<<str_uid;
    int uid;
    ss>>uid;

    //获取数据库操作类
    LoginDatabase login_db;

    //检查账号是否在线
    if (!login_db.checkUserIdOnline(uid, token)){
      result.setCode(2005);
      result.setMsg("该账号不在线");
      return result;
    }

    result.setCode(0);
    return result;
  };

private:

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

void RunDb() {

  if (!Database::database->init()) {
     cout << "error : database run fail !" << endl;
  }

}

void RunServer() {

  std::string server_address("0.0.0.0:50051");
  AccountServiceImpl service;

  ServerBuilder builder;
  
  std::ifstream skey("mypem/server.key");
  std::string strServerKey((std::istreambuf_iterator<char>(skey)),std::istreambuf_iterator<char>());
  //std::cout << "key: " <<strServerKey << std::endl;
  std::ifstream sCrt("mypem/server.crt");  
  std::string strServerCert((std::istreambuf_iterator<char>(sCrt)),std::istreambuf_iterator<char>());
  //std::cout << "crt: " << strServerCert << std::endl;
  
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
   
    // LOG::error("test");

   RunDb();
   RunServer();
   return 0;
}
