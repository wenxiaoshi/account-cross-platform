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

#include <grpcpp/grpcpp.h>

#include "hash_map.h"
#include <set>

#ifdef BAZEL_BUILD
#include "source/protos/account.grpc.pb.h"
#else
#include "account.grpc.pb.h"
#endif

using namespace std;

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
    return strncmp(mMapPhoneAndPsw[phone].c_str(),"NULL",4) != 0;
  };
  
  /**
  * 判断密码是否正确
  **/
  bool isPasswordCorrect(string phone,string password){
    return strncmp(mMapPhoneAndPsw[phone].c_str(),password.data(),mMapPhoneAndPsw[phone].length()) == 0;
  }

  /**
  * 更新账号对应的设备Id
  **/
  bool updateDeviceId(string phone,string deviceId){
    removeAccountDeviceId(phone);
    mMapPhoneAndDid.HMInsert(phone, deviceId);
    mSetDeviceId.insert(deviceId);
    return true;
  }

  /**
  * 清除账号对应的设备Id
  **/
  bool removeAccountDeviceId(string phone){
    string deviceId = mMapPhoneAndDid[phone].c_str();
    mMapPhoneAndDid.HMDelete(phone);
    std::cout << "removeAccountDeviceId " << mSetDeviceId.size() << deviceId << std::endl;
    mSetDeviceId.erase(deviceId);
    std::cout << "removeAccountDeviceId " << mSetDeviceId.size() << deviceId << std::endl;
    return true;
  }

  /**
  * 判断某设备是否联通
  **/
  bool judeDeviceIdOnline(string deviceId){
    std::cout << "judeDeviceIdOnline" << mSetDeviceId.size() << deviceId << std::endl;
    return mSetDeviceId.find(deviceId) != mSetDeviceId.end();
  }

  /**
  * 新增账号
  **/
  bool insertAccountToDB(string phone,string password){
    mMapPhoneAndPsw.HMInsert(phone, password);
    return true;
  }
private:
  static HashMap mMapPhoneAndPsw;
  static HashMap mMapPhoneAndDid;
  static set<string> mSetDeviceId;
};

HashMap LoginDatabase::mMapPhoneAndPsw(100);
HashMap LoginDatabase::mMapPhoneAndDid(100);
set<string> LoginDatabase::mSetDeviceId;

class LoginCore{
public:
  HandleResult handleUserLogin(std::string phone,std::string password,std::string deviceId){
    HandleResult result;
    
    LoginDatabase db;

    //查询账号是否存在
    if(!db.isHadSign(phone)){
      result.setCode(2000);
      result.setMsg("该账号不存在");
      return result;
    }

     //查询密码是否正确
     if(!db.isPasswordCorrect(phone,password)){
      result.setCode(2001);
      result.setMsg("密码错误");
      return result;
    }

    //更新账号对应的设备ID
    if (!db.updateDeviceId(phone,deviceId))
    {
      result.setCode(2003);
      result.setMsg("更新账号的sessionId失败");
      return result;
    }

    result.setCode(0);

    
    return result;
  };
   HandleResult handleUserSign(std::string phone,std::string password,std::string deviceId){
    HandleResult result;
    
    LoginDatabase db;
    
    //查询账号是否存在
    if(db.isHadSign(phone)){
      result.setCode(2002);
      result.setMsg("该账号已注册");
      return result;
    }

     
    //新增账号
    if (!db.insertAccountToDB(phone,password)){
      result.setCode(2004);
      result.setMsg("新增账号失败");
      return result;
    }

    //更新账号对应的设备ID
    if (!db.updateDeviceId(phone,deviceId)){
      result.setCode(2003);
      result.setMsg("更新账号的sessionId失败");
      return result;
    }

    result.setCode(0);

    
    return result;
  };


   HandleResult handleUserLogout(std::string phone){
    HandleResult result;
    
    LoginDatabase db;
    
    result.setCode(0);

    //查询账号是否存在
    if(!db.isHadSign(phone)){
      result.setMsg("账号不存在");
      return result;
    }

    //清除账号对应的设备ID
    if (!db.removeAccountDeviceId(phone)){
      result.setMsg("更新账号的sessionId失败");
      return result;
    }

    
    return result;
  };
 HandleResult handleUserCheckConnect(std::string deviceId){
    HandleResult result;
  
    
    LoginDatabase db;

    //检核设备是否在线
    if (!db.judeDeviceIdOnline(deviceId)){

      result.setCode(2005);
      result.setMsg("该设备不在线");
      return result;
    }

    result.setCode(0);


    return result;
  };

 

};



// Logic and data behind the server's behavior.
class AccountServiceImpl final : public Account::Service {

  Status requestUserLogin(ServerContext* context, const LoginRequest* request,
                  CodeReply* reply) override {
    std::cout << "登录请求-入口" << std::endl;
    
    LoginCore loginCore;
    HandleResult result = loginCore.handleUserLogin(request->phone(),request->password(),request->deviceid());
    
    reply->set_code(result.getCode());
    reply->set_msg(result.getMsg());
  
    std::cout << "登录请求-结束" << std::endl;
    return Status::OK;
  }
  Status requestUserSign(ServerContext* context, const SignRequest* request,
                  CodeReply* reply) override {
    std::cout << "注册请求-入口" << std::endl;
        
    LoginCore loginCore;
    HandleResult result = loginCore.handleUserSign(request->phone(),request->password(),request->deviceid());
    
    reply->set_code(result.getCode());
    reply->set_msg(result.getMsg());
    
    std::cout << "注册请求-结束" << std::endl;
    return Status::OK;
  }
  Status requestLogout(ServerContext* context, const LogoutRequest* request,
                  CodeReply* reply) override {
    std::cout << "下线请求-入口" << std::endl;
        
    LoginCore loginCore;
    HandleResult result = loginCore.handleUserLogout(request->phone());
    
    reply->set_code(result.getCode());
    reply->set_msg(result.getMsg());
    
    std::cout << "下线请求-结束" << std::endl;
    return Status::OK;
  }
  Status checkConnect(ServerContext* context, const ConnectRequest* request,
                  CodeReply* reply) override {
    std::cout << "连线检测请求-入口" << std::endl;

    LoginCore loginCore;
    HandleResult result = loginCore.handleUserCheckConnect(request->deviceid());
    
    reply->set_code(result.getCode());
    reply->set_msg(result.getMsg());

    std::cout << "连线检测请求-结束" << std::endl;
    return Status::OK;
  }
};



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
