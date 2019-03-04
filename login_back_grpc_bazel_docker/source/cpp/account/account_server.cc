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

#include "source/db/my_db.h"
#include "hash_map.h"
#include "common_utils.h"
#include "my_log.h"
#include "my_constant.h"

#ifdef BAZEL_BUILD
#include "source/protos/account.grpc.pb.h"
#else
#include "account.grpc.pb.h"
#endif

#define LOGD(msg)  utils::LogUtil::LOGD(msg);
#define LOGW(msg)  utils::LogUtil::LOGW(msg);
#define LOGI(msg)  utils::LogUtil::LOGI(msg);
#define LOGE(msg)  utils::LogUtil::LOGE(msg);

using namespace std;
using namespace my_struct;
using namespace utils;
using namespace db_utils;
using namespace my_model;
using namespace constants;

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
  * 根据UID获取用户Session信息
  **/
  UserSession getUserSession(int uid){
    return Database::database->queryUserSessionByUid(uid);
  }

private:

  
};


class LoginCore{
public:
  HandleResult handleUserLogin(string account,string password){
    HandleResult result;
    
    LoginDatabase login_db;

    UserAccount userAccount = login_db.getUserAccount(account);

    //查询账号是否存在
    if(userAccount.getUid() <= 0) {
      result.setCode(2000);
      result.setMsg("该账号不存在");
      return result;
    }

    //获得加密后password
    string encrypt_password = CommonUtils::EncryptPwd(account, password);
    if (encrypt_password.empty()) {
      result.setCode(2007);
      result.setMsg("初始化密码失败");
      return result;
    }

    //查询密码是否正确
    string db_password = userAccount.getPassword();
    if(!isEqual(db_password, encrypt_password)) {
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

  HandleResult handleUserSign(string account,string password){
    HandleResult result;
    
    LoginDatabase login_db;
    
    //查询账号是否存在
    if(login_db.isHadSign(account)){
      result.setCode(2002);
      result.setMsg("该账号已注册");
      return result;
    }

    //获得加密后password
    string encrypt_password = CommonUtils::EncryptPwd(account, password);
    if (encrypt_password.empty()) {
      result.setCode(2007);
      result.setMsg("初始化密码失败");
      return result;
    }

    //新增用户，插入账号信息到数据库
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
    CommonUtils::SplitString(decodeToken, vToken, ":");
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

    LOGI("check_connect token is " + decodeToken);

    std::vector<string> vToken;
    CommonUtils::SplitString(decodeToken, vToken, ":");
    if (vToken.size() != 5) {
      result.setCode(2006);
      result.setMsg("用户token不合法");
      return result;
    }

    //获取Token过期时间
    string token_end_time = vToken[4];
    int end_time = getIntByString(token_end_time);

    //判断Token是否过期
    if (isTimeExpired(end_time)) {
      result.setCode(2008);
      result.setMsg("用户token已过期");
      return result;
    }

    //获得账号UID
    string str_uid = vToken[0];
    int uid = getIntByString(str_uid);

    //获取数据库操作类
    LoginDatabase login_db;

    //检查账号是否在线
    UserSession userSession = login_db.getUserSession(uid);
    if (!userSession.isOnline()){
      result.setCode(2009);
      result.setMsg("该账号不在线");
      return result;
    }
    if (!isEqual(token, userSession.getToken())){
      result.setCode(2005);
      result.setMsg("该账号在另一端登录");
      return result;
    }

    result.setCode(0);
    return result;
  };

private:

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

 /**
  * 将string转换成int
  **/
  int getIntByString(string str) {
    stringstream ss;
    ss<<str;
    int i_data;
    ss>>i_data;
    return i_data;
  }

 /**
  * 判断时间是否过期
  **/
  bool isTimeExpired(int end_time){
    time_t now_time = time(NULL);
    return now_time > end_time;
  }

};



class AccountServiceImpl final : public Account::Service {

  Status requestUserLogin(ServerContext* context, const LoginRequest* request,
                  CodeReply* reply) override {

    string account = request->account();
    string password = request->password();

    LOGI("requestUserLogin | account = " + account + ", password = " + password);

    bool isParamValid = true;
    string error_msg;
    
    //校验用户账号
    if (!ParamUtils::CheckAccountValid(account, error_msg)) {
      reply->set_code(ResultCode::ERROR_PARAM_IS_INVALID);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("account is not valid");
    };

    //校验用户密码
    if (isParamValid && !ParamUtils::CheckPasswordValid(password, error_msg)) {
      reply->set_code(ResultCode::ERROR_PARAM_IS_INVALID);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("password is not valid");
    };

    //参数正确，执行请求
    if (isParamValid) {
      LoginCore loginCore;
      HandleResult result = loginCore.handleUserLogin(account,password);
      reply->set_code(result.getCode());
      reply->set_msg(result.getMsg());
      reply->set_data(result.getData());
    }
    
    return Status::OK;
  }

  Status requestUserSign(ServerContext* context, const SignRequest* request,
                  CodeReply* reply) override {

    string account = request->account();
    string password = request->password();

    LOGI("requestUserSign | account = " + account + ", password = " + password);

    bool isParamValid = true;
    string error_msg;
    
    //校验用户账号
    if (!ParamUtils::CheckAccountValid(account, error_msg)) {
      reply->set_code(ResultCode::ERROR_PARAM_IS_INVALID);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("account is not valid");
    };

    //校验用户密码
    if (isParamValid && !ParamUtils::CheckPasswordValid(password, error_msg)) {
      reply->set_code(ResultCode::ERROR_PARAM_IS_INVALID);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("password is not valid");
    };

    //参数正确，执行请求
    if (isParamValid) {
      LoginCore loginCore;
      HandleResult result = loginCore.handleUserSign(account,password);
      reply->set_code(result.getCode());
      reply->set_msg(result.getMsg());
      reply->set_data(result.getData());
    } 
   
    return Status::OK;
  }

  Status requestLogout(ServerContext* context, const LogoutRequest* request,
                  CodeReply* reply) override {

    string token = request->token();

    LOGI("requestLogout | token = " + token);

    LoginCore loginCore;
    HandleResult result = loginCore.handleUserLogout(token);
    
    reply->set_code(result.getCode());
    reply->set_msg(result.getMsg());
    reply->set_data(result.getData());
    return Status::OK;
  }

  Status checkConnect(ServerContext* context, const ConnectRequest* request,
                  CodeReply* reply) override {

    string token = request->token();

    LOGI("checkConnect | token = " + token);

    bool isParamValid = true;
    string error_msg;
    
    //校验用户token
    if (!ParamUtils::CheckTokenValid(token, error_msg)) {
      reply->set_code(ResultCode::ERROR_PARAM_IS_INVALID);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("token is not valid");
    };

    //参数正确，执行请求
    if (isParamValid) {
      LoginCore loginCore;
      HandleResult result = loginCore.handleUserCheckConnect(token);  
      reply->set_code(result.getCode());
      reply->set_msg(result.getMsg());
      reply->set_data(result.getData());
    }
    
    return Status::OK;
  }
};

void RunDb() {

  if (!Database::database->init()) {
     LOGE("database run fail !");
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
  LOGI("Server listening on " + server_address);

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
