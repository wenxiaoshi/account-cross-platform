//
// Created by melon on 2019/1/22.
//

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
#include <hiredis/hiredis.h>
#include "source/libs/json/json.h"

#include "source/cpp/manager/conf/server_conf.h"
#include "source/cpp/manager/db/db_manager.h"
#include "source/cpp/manager/redis/redis_manager.h"

#include "../utils/common_utils.h"
#include "../utils/param_utils.h"
#include "../utils/log_utils.h"

#include "../constant/my_constant.h"

#include "source/protos/account.grpc.pb.h"

#define LOGD(msg) utils::LogUtil::LOGD(msg);
#define LOGW(msg) utils::LogUtil::LOGW(msg);
#define LOGI(msg) utils::LogUtil::LOGI(msg);
#define LOGE(msg) utils::LogUtil::LOGE(msg);
#define LOGM(bean) utils::LogUtil::LOGM(bean);

using namespace std;
using namespace utils;
using namespace manager;
using namespace my_model;
using namespace constants;

using account::Account;
using account::CodeReply;
using account::ConnectRequest;
using account::LoginRequest;
using account::LogoutRequest;
using account::SignRequest;
using account::RefreshRequest;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using utils::LogMBean;

class HandleResult
{
private:
  int code = 0;
  std::string msg = "";
  std::string data = "";

public:
  int getCode()
  {
    return code;
  };
  void setCode(int co)
  {
    code = co;
  };
  std::string getMsg()
  {
    return msg;
  };
  void setMsg(std::string message)
  {
    msg = message;
  };
  std::string getData()
  {
    return data;
  };
  void setData(std::string da)
  {
    data = da;
  };
};

class LoginDatabase
{
public:

  /*
  主要功能：
  判断是否已经注册

  入口参数
  account： 	      用户账号
 
  出口参数：
  bool ：           true表示已经注册；false表示未注册
  */
  bool isHadSign(string account)
  {
    return Database::getDatabase()->isUserExist(account);
  };

  /**
  主要功能：
  注册用户账号

  入口参数
  account： 	      用户账号
  encrypt_pwd：     用户密码

  出口参数：
  bool ：         true表示注册成功；false表示注册失败
  **/
  bool signAccount(string account, string encrypt_pwd)
  {
    return Database::getDatabase()->addUserAccount(account, encrypt_pwd);
  }

  /**
  主要功能：
  获取用户信息

  入口参数
  account： 	      用户账号

  出口参数：
  UserAccount：     用户账号信息
  **/
  UserAccount getUserAccount(string account)
  {
    return Database::getDatabase()->queryUserAccountByAccount(account);
  }

private:
};

class LoginRedis
{
public:

  /**
  主要功能：
  更新用户token

  入口参数
  uid： 	          用户UID
  token： 	        用户Token
  refreshToken：    用户RefreshToken

  出口参数：
  bool ：           true表示成功；false表示失败
  **/
  bool updateToken(int uid, string token, string refreshToken)
  {
    stringstream ssToken;
    ssToken << uid << "_token";
    stringstream ssRToken;
    ssRToken << uid << "_refresh_token";
    return redis.setString(ssToken.str(), token) && redis.setString(ssRToken.str(), refreshToken);
  }

  /**
  主要功能：
  判断用户Token是否正确

  入口参数
  uid： 	          用户UID
  token： 	        用户Token

  出口参数：
  bool ：           true表示正确；false表示不正确
  **/
  bool isTokenRight(int uid, string token)
  {
    stringstream ssToken;
    ssToken << uid << "_token";
    string redisToken;
    redis.getString(ssToken.str(), redisToken);
    return CommonUtils::isEqual(redisToken, token);
  }

  /**
  主要功能：
  判断用户Token是否正确

  入口参数
  uid： 	          用户UID

  出口参数：
  bool ：           true表示成功；false表示失败
  **/
  bool cleanToken(int uid)
  {
    stringstream ssToken;
    ssToken << uid << "_token";
    stringstream ssRToken;
    ssRToken << uid << "_refresh_token";
    return redis.setString(ssToken.str(), "") && redis.setString(ssRToken.str(), "");
  }

  /**
  主要功能：
  获取用户Token

  入口参数
  uid： 	          用户UID

  出口参数：
  string ：         用户Token
  **/
  string getUserToken(int uid)
  {
    stringstream ssToken;
    ssToken << uid << "_token";
    string token;
    redis.getString(ssToken.str(), token);
    return token;
  }

private:
  Redis redis = *Redis::getRedis();
};

class LoginCore
{
public:

  /**
  用户模块-手机号登录 
  
  入口参数
  account： 	      用户账号（手机号）
  password：        用户密码（明文）

  出口参数：
  token：           用户Token（用来接口请求）
  refresh_token：   用户Token（用来刷新Token）
  **/
  HandleResult handleUserLogin(string account, string password)
  {
    HandleResult result;

    LoginDatabase login_db;
    LoginRedis login_redis;

    UserAccount userAccount = login_db.getUserAccount(account);

    //查询账号是否存在
    if (userAccount.getUid() <= 0)
    {
      result.setCode(ResultCode::UserLogin_AccountNotExist);
      result.setMsg(MsgTip::UserLogin_AccountNotExist);
      return result;
    }

    //获得加密后password
    string encrypt_password = CommonUtils::EncryptPwd(account, password);
    if (encrypt_password.empty())
    {
      result.setCode(ResultCode::UserLogin_PasswordInitFail);
      result.setMsg(MsgTip::UserLogin_PasswordInitFail);
      return result;
    }

    //查询密码是否正确
    string db_password = userAccount.getPassword();
    if (!CommonUtils::isEqual(db_password, encrypt_password))
    {
      result.setCode(ResultCode::UserLogin_PasswordError);
      result.setMsg(MsgTip::UserLogin_PasswordError);
      return result;
    }

    //获取用户UID
    int uid = userAccount.getUid();

    //生成Token、refreshToken
    string token = CommonUtils::GenToken(uid, account);
    string refreshToken = CommonUtils::GenRefreshToken(uid, account);

    //更新Token到redis
    if (!login_redis.updateToken(uid, token, refreshToken))
    {
      result.setCode(ResultCode::UserLogin_UpdateTokenFail);
      result.setMsg(MsgTip::UserLogin_UpdateTokenFail);
      return result;
    }

    //返回Token
    result.setCode(ResultCode::SUCCESS);
    Json::Value root;
    root["token"] = token;
    root["refresh_token"] = refreshToken;
    Json::FastWriter fw;
    result.setData(fw.write(root));
    return result;
  };

  /**
  用户模块-手机号注册
  
  入口参数
  account： 	      用户账号（手机号）
  password：        用户密码（明文）
  
  出口参数：
  token：           用户Token（用来接口请求）
  refresh_token：   用户Token（用来刷新Token）
  **/
  HandleResult handleUserSign(string account, string password)
  {
    HandleResult result;

    LoginDatabase login_db;
    LoginRedis login_redis;

    //查询账号是否存在
    if (login_db.isHadSign(account))
    {
      result.setCode(ResultCode::UserSign_AccountHadExist);
      result.setMsg(MsgTip::UserSign_AccountHadExist);
      return result;
    }

    //获得加密后password
    string encrypt_password = CommonUtils::EncryptPwd(account, password);
    if (encrypt_password.empty())
    {
      result.setCode(ResultCode::UserSign_PasswordInitFail);
      result.setMsg(MsgTip::UserSign_PasswordInitFail);
      return result;
    }

    //新增用户，插入账号信息到数据库
    if (!login_db.signAccount(account, encrypt_password))
    {
      result.setCode(ResultCode::UserSign_CreateAccountFail);
      result.setMsg(MsgTip::UserSign_CreateAccountFail);
      return result;
    }

    //获得用户信息
    UserAccount userAccount = login_db.getUserAccount(account);
    if (userAccount.getUid() <= 0)
    {
      result.setCode(ResultCode::UserSign_GetAccountInfoFail);
      result.setMsg(MsgTip::UserSign_GetAccountInfoFail);
      return result;
    }

    //获得用户UID
    int uid = userAccount.getUid();

    //生成Token、refreshToken
    string token = CommonUtils::GenToken(uid, account);
    string refreshToken = CommonUtils::GenRefreshToken(uid, account);

    //更新Token到redis
    if (!login_redis.updateToken(uid, token, refreshToken))
    {
      result.setCode(ResultCode::UserSign_CreateSeesionFail);
      result.setMsg(MsgTip::UserSign_CreateSeesionFail);
      return result;
    }

    //返回Token
    result.setCode(ResultCode::SUCCESS);
    Json::Value root;
    root["token"] = token;
    root["refresh_token"] = refreshToken;
    Json::FastWriter fw;
    result.setData(fw.write(root));
    return result;
  };

  /**
  用户模块-退出登录
  
  入口参数
  token： 	        用户Token
  **/
  HandleResult handleUserLogout(string token)
  {
    HandleResult result;

    LoginRedis login_redis;

    //解密Token
    string decodeToken = CommonUtils::DecryptToken(token);
    if (decodeToken.empty())
    {
      result.setCode(ResultCode::UserLogout_TokenNotValid);
      result.setMsg(MsgTip::UserLogout_TokenNotValid);
      return result;
    }

    //解析Token，获取用户信息
    vector<string> vToken;
    CommonUtils::SplitString(decodeToken, vToken, ":");
    if (vToken.size() != 5)
    {
      result.setCode(ResultCode::UserLogout_TokenNotValid);
      result.setMsg(MsgTip::UserLogout_TokenNotValid);
      return result;
    }

    //获得账号UID
    int uid = getIntByString(vToken[0]);

    //token是否正确
    if (!login_redis.isTokenRight(uid, token))
    {
      result.setCode(ResultCode::UserLogout_TokenNotExist);
      result.setMsg(MsgTip::UserLogout_TokenNotExist);
      return result;
    }

    //清除token
    if (!login_redis.cleanToken(uid))
    {
      result.setCode(ResultCode::UserLogout_UpdateSessionFail);
      result.setMsg(MsgTip::UserLogout_UpdateSessionFail);
      return result;
    }

    result.setCode(ResultCode::SUCCESS);
    return result;
  };

  /**
  用户模块-检查连接状态
  
  入口参数
  token： 	        用户Token
  **/
  HandleResult handleUserCheckConnect(string token)
  {
    HandleResult result;

    //解密Token
    string decodeToken = CommonUtils::DecryptToken(token);
    if (decodeToken.empty())
    {
      result.setCode(ResultCode::CheckConnect_TokenNotValid);
      result.setMsg(MsgTip::CheckConnect_TokenNotValid);
      return result;
    }

    std::vector<string> vToken;
    CommonUtils::SplitString(decodeToken, vToken, ":");
    if (vToken.size() != 5)
    {
      result.setCode(ResultCode::CheckConnect_TokenNotValid);
      result.setMsg(MsgTip::CheckConnect_TokenNotValid);
      return result;
    }

    //获取Token过期时间
    string token_end_time = vToken[4];
    int end_time = getIntByString(token_end_time);

    //判断Token是否过期
    if (isTimeExpired(end_time))
    {
      result.setCode(ResultCode::CheckConnect_TokenHadExpire);
      result.setMsg(MsgTip::CheckConnect_TokenHadExpire);
      return result;
    }

    //获得账号UID
    string str_uid = vToken[0];
    int uid = getIntByString(str_uid);

    //获取redis中用户Token
    LoginRedis login_redis;
    if (!CommonUtils::isEqual(token, login_redis.getUserToken(uid)))
    {
      result.setCode(ResultCode::CheckConnect_AccountTokenNotEqual);
      result.setMsg(MsgTip::CheckConnect_AccountTokenNotEqual);
      return result;
    }

    result.setCode(ResultCode::SUCCESS);
    return result;
  };

  /**
  用户模块-刷新用户Token
  
  入口参数
  token：           用户Token（旧）
  refresh_token：   用户Token（旧）
  
  出口参数：
  token：           用户Token（新）
  refresh_token：   用户Token（新）
  **/
  HandleResult handleRefreshToken(string token, string refreshToken)
  {
    HandleResult result;

    //解密Token
    string decodeToken = CommonUtils::DecryptToken(token);
    if (decodeToken.empty())
    {
      result.setCode(ResultCode::RefreshToken_TokenNotValid);
      result.setMsg(MsgTip::RefreshToken_TokenNotValid);
      return result;
    }

    std::vector<string> vToken;
    CommonUtils::SplitString(decodeToken, vToken, ":");
    if (vToken.size() != 5)
    {
      result.setCode(ResultCode::RefreshToken_TokenNotValid);
      result.setMsg(MsgTip::RefreshToken_TokenNotValid);
      return result;
    }

    //解密refreshToken
    string decodeRefreshToken = CommonUtils::DecryptToken(refreshToken);
    if (decodeRefreshToken.empty())
    {
      result.setCode(ResultCode::RefreshToken_RefreshTokenNotValid);
      result.setMsg(MsgTip::RefreshToken_RefreshTokenNotValid);
      return result;
    }

    std::vector<string> vRefreshToken;
    CommonUtils::SplitString(decodeRefreshToken, vRefreshToken, ":");
    if (vRefreshToken.size() != 5)
    {
      result.setCode(ResultCode::RefreshToken_RefreshTokenNotValid);
      result.setMsg(MsgTip::RefreshToken_RefreshTokenNotValid);
      return result;
    }

    //对比token与refreshToken是否来自同一用户
    int uid = getIntByString(vToken[0]);
    if(uid != getIntByString(vRefreshToken[0])){
      result.setCode(ResultCode::RefreshToken_RefreshATokenNotEqual);
      result.setMsg(MsgTip::RefreshToken_RefreshATokenNotEqual);
      return result;
    }

    //判断refreshToken是否过期
    string token_end_time = vRefreshToken[4];
    int end_time = getIntByString(token_end_time);
    if (isTimeExpired(end_time))
    {
      result.setCode(ResultCode::RefreshToken_RefreshTokenHadExpire);
      result.setMsg(MsgTip::RefreshToken_RefreshTokenHadExpire);
      return result;
    }

    //生成token、refreshToken
    string account = vToken[1];
    string new_token = CommonUtils::GenToken(uid, account);
    string new_refreshToken = CommonUtils::GenRefreshToken(uid, account);

    //更新token、refreshToken到redis
    LoginRedis login_redis;
    if (!login_redis.updateToken(uid, new_token, new_refreshToken))
    {
      result.setCode(ResultCode::RefreshToken_CreateSeesionFail);
      result.setMsg(MsgTip::RefreshToken_CreateSeesionFail);
      return result;
    }

    //返回token、refreshToken
    result.setCode(ResultCode::SUCCESS);
    Json::Value root;
    root["token"] = new_token;
    root["refresh_token"] = new_refreshToken;
    Json::FastWriter fw;
    result.setData(fw.write(root));
    return result;

  }
private:
  /**
  * 将string转换成int
  **/
  int getIntByString(string str)
  {
    stringstream ss;
    ss << str;
    int i_data;
    ss >> i_data;
    return i_data;
  }

  /**
  * 判断时间是否过期
  **/
  bool isTimeExpired(int end_time)
  {
    time_t now_time = time(NULL);
    return now_time > end_time;
  }
};

class AccountServiceImpl final : public Account::Service
{

  /**
   * 用户模块-手机号登录 
   **/
  Status requestUserLogin(ServerContext *context, const LoginRequest *request,
                          CodeReply *reply) override
  {

    string account = request->account();
    string password = request->password();

    LogMBean log_bean("requestUserLogin");

    bool isParamValid = true;
    string error_msg;

    //校验用户账号
    if (!ParamUtils::CheckAccountValid(account, error_msg))
    {
      reply->set_code(ResultCode::ReqParamError);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("account is not valid");
    };

    //校验用户密码
    if (isParamValid && !ParamUtils::CheckPasswordValid(password, error_msg))
    {
      reply->set_code(ResultCode::ReqParamError);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("password is not valid");
    };

    //参数正确，执行请求
    if (isParamValid)
    {
      LoginCore loginCore;
      HandleResult result = loginCore.handleUserLogin(account, password);
      reply->set_code(result.getCode());
      reply->set_msg(result.getMsg());
      reply->set_data(result.getData());
    }

    //打印接口日志
    log_bean.addParam("code", reply->code());
    log_bean.addParam("account", account);
    log_bean.addParam("password", password);
    LOGM(log_bean);
    return Status::OK;
  }

  /**
   * 用户模块-手机号注册
   **/
  Status requestUserSign(ServerContext *context, const SignRequest *request,
                         CodeReply *reply) override
  {

    LogMBean log_bean("requestUserSign");

    string account = request->account();
    string password = request->password();

    bool isParamValid = true;
    string error_msg;

    //校验用户账号
    if (!ParamUtils::CheckAccountValid(account, error_msg))
    {
      reply->set_code(ResultCode::ReqParamError);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("account is not valid");
    };

    //校验用户密码
    if (isParamValid && !ParamUtils::CheckPasswordValid(password, error_msg))
    {
      reply->set_code(ResultCode::ReqParamError);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("password is not valid");
    };

    //参数正确，执行请求
    if (isParamValid)
    {
      LoginCore loginCore;
      HandleResult result = loginCore.handleUserSign(account, password);
      reply->set_code(result.getCode());
      reply->set_msg(result.getMsg());
      reply->set_data(result.getData());
    }

    //打印接口日志
    log_bean.addParam("code", reply->code());
    log_bean.addParam("account", account);
    log_bean.addParam("password", password);
    LOGM(log_bean);

    return Status::OK;
  }

  /**
   * 用户模块-用户退出登录
   **/
  Status requestLogout(ServerContext *context, const LogoutRequest *request,
                       CodeReply *reply) override
  {

    LogMBean log_bean("requestLogout");

    string token = request->token();

    //执行请求
    LoginCore loginCore;
    HandleResult result = loginCore.handleUserLogout(token);

    reply->set_code(result.getCode());
    reply->set_msg(result.getMsg());
    reply->set_data(result.getData());

    //打印接口日志
    log_bean.addParam("code", reply->code());
    log_bean.addParam("token", token);
    LOGM(log_bean);

    return Status::OK;
  }

  /**
   * 用户模块-检查用户连接状态
   **/
  Status checkConnect(ServerContext *context, const ConnectRequest *request,
                      CodeReply *reply) override
  {

    LogMBean log_bean("checkConnect");

    string token = request->token();

    bool isParamValid = true;
    string error_msg;

    //校验用户token
    if (!ParamUtils::CheckStringValid(token, error_msg))
    {
      reply->set_code(ResultCode::ReqParamError);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("token is empty");
    };

    //参数正确，执行请求
    if (isParamValid)
    {
      LoginCore loginCore;
      HandleResult result = loginCore.handleUserCheckConnect(token);
      reply->set_code(result.getCode());
      reply->set_msg(result.getMsg());
      reply->set_data(result.getData());
    }

    //打印接口日志
    log_bean.addParam("code", reply->code());
    log_bean.addParam("token", token);
    LOGM(log_bean);

    return Status::OK;
  }

  /**
   * 用户模块-刷新用户Token，更新refreshToken
   **/
  Status refreshToken(ServerContext *context, const RefreshRequest *request,
                      CodeReply *reply) override
  {
    LogMBean log_bean("refreshToken");

    string token = request->token();
    string refreshToken = request->refresh_token();

    bool isParamValid = true;
    string error_msg;

    //校验用户token
    if (!ParamUtils::CheckStringValid(token, error_msg))
    {
      reply->set_code(ResultCode::ReqParamError);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("token is empty");
    };

    //校验用户refresh_token
    if (!ParamUtils::CheckStringValid(refreshToken, error_msg))
    {
      reply->set_code(ResultCode::ReqParamError);
      reply->set_msg(error_msg);
      isParamValid = false;
      LOGW("refresh_token is empty");
    };

    //参数正确，执行请求
    if (isParamValid)
    {
      LoginCore loginCore;
      HandleResult result = loginCore.handleRefreshToken(token,refreshToken);
      reply->set_code(result.getCode());
      reply->set_msg(result.getMsg());
      reply->set_data(result.getData());
    }

    //打印接口日志
    log_bean.addParam("code", reply->code());
    log_bean.addParam("token", token);
    log_bean.addParam("refresh_token", refreshToken);
    LOGM(log_bean);

    return Status::OK;
    
  }
};

void RunServer()
{

  std::string server_address("0.0.0.0:50051");
  AccountServiceImpl service;

  ServerBuilder builder;

  std::ifstream skey("source/pem/server.key");
  std::string strServerKey((std::istreambuf_iterator<char>(skey)), std::istreambuf_iterator<char>());
  //std::cout << "key: " <<strServerKey << std::endl;
  std::ifstream sCrt("source/pem/server.crt");
  std::string strServerCert((std::istreambuf_iterator<char>(sCrt)), std::istreambuf_iterator<char>());
  //std::cout << "crt: " << strServerCert << std::endl;

  grpc::SslServerCredentialsOptions::PemKeyCertPair pkcp = {strServerKey.c_str(), strServerCert.c_str()};

  grpc::SslServerCredentialsOptions ssl_opts;
  ssl_opts.pem_root_certs = "";
  ssl_opts.pem_key_cert_pairs.push_back(pkcp);
  std::shared_ptr<grpc::ServerCredentials> creds = grpc::SslServerCredentials(ssl_opts);

  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, creds);
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  LOGD("Server listening on " + server_address);

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char **argv)
{
  manager::ServerConfig conf;
  Redis::getRedis()->connect(conf);
  Database::getDatabase()->connect(conf);
  RunServer();
  return 0;
}
