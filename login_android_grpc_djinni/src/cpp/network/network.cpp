//
// Created by dw on 19-2-26.
//

#include <string>
#include <jni.h>

#include <grpc++/grpc++.h>
//#include "../../../platforms/android/app/.externalNativeBuild/cmake/debug/armeabi-v7a/gens/account.grpc.pb.h"
#include "account.grpc.pb.h"
#include "../model/network_result.h"
#include "../utils/network_utils.h"

#include "network.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using account::Account;
using account::LoginRequest;
using account::SignRequest;
using account::LogoutRequest;
using account::ConnectRequest;
using account::CodeReply;

using namespace demo;

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
            // The actual RPC.
            Status status = stub_->requestUserLogin(&context, request, &reply);

            if (status.ok()) {
                return reply;
            } else {
                //todo 网络错误吗转换
                reply.set_code(-1);
                reply.set_msg(status.error_message());
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
            // The actual RPC.
            Status status = stub_->requestUserSign(&context, request, &reply);

            if (status.ok()) {
                return reply;
            } else {
                //todo 网络错误吗转换
                reply.set_code(-1);
                reply.set_msg(status.error_message());
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
            // The actual RPC.
            Status status = stub_->requestLogout(&context, request, &reply);

            if (status.ok()) {
                return reply;
            } else {
                //todo 网络错误吗转换
                reply.set_code(-1);
                reply.set_msg(status.error_message());
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
            // The actual RPC.
            Status status = stub_->checkConnect(&context, request, &reply);

            if (status.ok()) {
                return reply;
            } else {
                //todo 网络错误吗转换
                reply.set_code(-1);
                reply.set_msg(status.error_message());
                return reply;
            }
        }

    private:
        std::unique_ptr<Account::Stub> stub_;
    };

    ReqResult NetworkCore::reqLogin(const std::string account,const std::string password){

        AccountClient client(utils::NetworkUtils::getNetworkChannel());

        CodeReply reply = client.requestUserLogin(account,password);

        ReqResult result;
        result.setCode(reply.code());
        result.setMsg(reply.msg());
        result.setData(reply.data());

        return result;
    }

    ReqResult NetworkCore::reqSign(const std::string account,const std::string password){

        AccountClient client(utils::NetworkUtils::getNetworkChannel());

        CodeReply reply = client.requestUserSign(account,password);


        ReqResult result;
        result.setCode(reply.code());
        result.setMsg(reply.msg());
        result.setData(reply.data());

        return result;
    }

    ReqResult NetworkCore::reqLogout(const std::string token){

        AccountClient client(utils::NetworkUtils::getNetworkChannel());

        CodeReply reply = client.requestLogout(token);

        ReqResult result;
        result.setCode(reply.code());
        result.setMsg(reply.msg());
        result.setData(reply.data());

        return result;
    }

    ReqResult NetworkCore::checkConnect(const std::string token){

        AccountClient client(utils::NetworkUtils::getNetworkChannel());

        CodeReply reply = client.checkConnect(token);

        ReqResult result;
        result.setCode(reply.code());
        result.setMsg(reply.msg());
        result.setData(reply.data());

        return result;
    }


}