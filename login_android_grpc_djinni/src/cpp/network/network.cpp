//
// Created by dw on 19-2-26.
//

#include <string>
//#include <jni.h>

#include <grpc++/grpc++.h>

#include "account.grpc.pb.h"
#include "../model/network_result.h"
#include "../utils/network_utils.h"
#include "../utils/log_utils.h"
#include "network.h"
#include "../model/proj_constants.h"

#define LOGD(msg)  utils::LogUtil::LOGD(msg);
#define LOGE(msg)  utils::LogUtil::LOGE(msg);

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using account::Account;
using account::LoginRequest;
using account::SignRequest;
using account::LogoutRequest;
using account::ConnectRequest;
using account::RefreshRequest;
using account::CodeReply;

using namespace demo;
using namespace project_constants;

namespace network{

    class AccountClient {
    public:
        AccountClient(std::shared_ptr<Channel> channel)
                : stub_(Account::NewStub(channel)) {}

        // Assembles the client's payload, sends it and presents the response back
        // from the server.
        CodeReply requestUserLogin(const std::string& account, const std::string& password) {
            // Data we are sending to the server.
            LoginRequest request;
            request.set_account(account);
            request.set_password(password);

            // Container for the data we expect from the server.
            CodeReply reply;

            // Context for the client. It could be used to convey extra information to
            // the server and/or tweak certain RPC behaviors.
            ClientContext context;
            
            try{
                // The actual RPC.
                Status status = stub_->requestUserLogin(&context, request, &reply);
                
                if (status.ok()) {
                    return reply;
                } else {
                    //todo 网络错误吗转换
                    reply.set_code(-1);
                    reply.set_msg(ToastTip::TOAST_ERROR_NETWORK_UNVALAIBLE);
                    LOGD("[network.requestUserLogin] " + status.error_message());
                    return reply;
                }
            }catch(...){
                reply.set_code(-2);
                reply.set_msg(ToastTip::TOAST_ERROR_NETWORK_UNVALAIBLE);
                LOGE("[network.requestUserLogin] catch requestUserLogin network error");
                return reply;
            }
            
        }

        CodeReply requestUserSign(const std::string& account, const std::string& password) {
            // Data we are sending to the server.
            SignRequest request;
            request.set_account(account);
            request.set_password(password);

            // Container for the data we expect from the server.
            CodeReply reply;

            // Context for the client. It could be used to convey extra information to
            // the server and/or tweak certain RPC behaviors.
            ClientContext context;
            
            try{
                // The actual RPC.
                Status status = stub_->requestUserSign(&context, request, &reply);
                
                if (status.ok()) {
                    return reply;
                } else {
                    //todo 网络错误吗转换
                    reply.set_code(-1);
                    reply.set_msg(ToastTip::TOAST_ERROR_NETWORK_UNVALAIBLE);
                    LOGD("[network.requestUserSign] " + status.error_message());
                    return reply;
                }
            }catch(...){
                reply.set_code(-2);
                reply.set_msg(ToastTip::TOAST_ERROR_NETWORK_UNVALAIBLE);
                LOGE("[network.requestUserSign] catch requestUserSign network error");
                return reply;
            }
            
        }

        CodeReply requestLogout(const std::string& token) {
            // Data we are sending to the server.
            LogoutRequest request;
            request.set_token(token);

            // Container for the data we expect from the server.
            CodeReply reply;

            // Context for the client. It could be used to convey extra information to
            // the server and/or tweak certain RPC behaviors.
            ClientContext context;
            
            try{
                // The actual RPC.
                Status status = stub_->requestLogout(&context, request, &reply);
                
                if (status.ok()) {
                    return reply;
                } else {
                    //todo 网络错误吗转换
                    reply.set_code(-1);
                    reply.set_msg(ToastTip::TOAST_ERROR_NETWORK_UNVALAIBLE);
                    LOGD("[network.requestLogout] " + status.error_message());
                    return reply;
                }
            }catch(...){
                reply.set_code(-2);
                reply.set_msg(ToastTip::TOAST_ERROR_NETWORK_UNVALAIBLE);
                LOGE("[network.requestLogout] catch requestLogout network error");
                return reply;
            }
            
        }

        CodeReply checkConnect(const std::string& token) {
            // Data we are sending to the server.
            ConnectRequest request;
            request.set_token(token);

            // Container for the data we expect from the server.
            CodeReply reply;

            // Context for the client. It could be used to convey extra information to
            // the server and/or tweak certain RPC behaviors.
            ClientContext context;
            
            try{
                // The actual RPC.
                Status status = stub_->checkConnect(&context, request, &reply);
                
                if (status.ok()) {
                    return reply;
                } else {
                    //todo 网络错误吗转换
                    reply.set_code(-1);
                    reply.set_msg(ToastTip::TOAST_ERROR_NETWORK_UNVALAIBLE);
                    LOGD("[network.checkConnect] " + status.error_message());
                    return reply;
                }
            }catch(...){
                reply.set_code(-2);
                reply.set_msg(ToastTip::TOAST_ERROR_NETWORK_UNVALAIBLE);
                LOGE("[network.checkConnect] catch checkConnect network error");
                return reply;
            }
            
        }

        CodeReply refreshToken(const std::string& token, const std::string& refreshToken) {
            // Data we are sending to the server.
            RefreshRequest request;
            request.set_token(token);
            request.set_refresh_token(refreshToken);
            // Container for the data we expect from the server.
            CodeReply reply;

            // Context for the client. It could be used to convey extra information to
            // the server and/or tweak certain RPC behaviors.
            ClientContext context;
            
            try{
                // The actual RPC.
                Status status = stub_->refreshToken(&context, request, &reply);
                
                if (status.ok()) {
                    return reply;
                } else {
                    //todo 网络错误吗转换
                    reply.set_code(-1);
                    reply.set_msg(ToastTip::TOAST_ERROR_NETWORK_UNVALAIBLE);
                    LOGD("[network.refreshToken] " + status.error_message());
                    return reply;
                }
            }catch(...){
                reply.set_code(-2);
                reply.set_msg(ToastTip::TOAST_ERROR_NETWORK_UNVALAIBLE);
                LOGE("[network.refreshToken] catch refreshToken network error");
                return reply;
            }
            
        }

    private:
        std::unique_ptr<Account::Stub> stub_;
    };

    
    /*
     请求登录操作
     */
    ReqResult NetworkCore::reqLogin(const std::string account,const std::string password){

        AccountClient client(utils::NetworkUtils::getNetworkChannel());

        CodeReply reply = client.requestUserLogin(account,password);

        ReqResult result;
        result.setCode(reply.code());
        result.setMsg(reply.msg());
        result.setData(reply.data());

        return result;
    }

    /*
     请求注册操作
     */
    ReqResult NetworkCore::reqSign(const std::string account,const std::string password){

        AccountClient client(utils::NetworkUtils::getNetworkChannel());

        CodeReply reply = client.requestUserSign(account,password);


        ReqResult result;
        result.setCode(reply.code());
        result.setMsg(reply.msg());
        result.setData(reply.data());

        return result;
    }

    /*
     退出登录操作
     */
    ReqResult NetworkCore::reqLogout(const std::string token){

        AccountClient client(utils::NetworkUtils::getNetworkChannel());

        CodeReply reply = client.requestLogout(token);

        ReqResult result;
        result.setCode(reply.code());
        result.setMsg(reply.msg());
        result.setData(reply.data());

        return result;
    }

    /*
     检查连接状态操作
     */
    ReqResult NetworkCore::checkConnect(const std::string token){

        AccountClient client(utils::NetworkUtils::getNetworkChannel());

        CodeReply reply = client.checkConnect(token);

        ReqResult result;
        result.setCode(reply.code());
        result.setMsg(reply.msg());
        result.setData(reply.data());

        return result;
    }

    /*
     刷新用户凭证Token操作
     */
    ReqResult NetworkCore::refreshToken(const std::string token,const std::string refreshToken){
        
        AccountClient client(utils::NetworkUtils::getNetworkChannel());

        CodeReply reply = client.refreshToken(token, refreshToken);

        ReqResult result;
        result.setCode(reply.code());
        result.setMsg(reply.msg());
        result.setData(reply.data());

        return result;
    }

}
