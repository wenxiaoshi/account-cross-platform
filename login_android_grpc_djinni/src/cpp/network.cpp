//
// Created by dw on 19-2-26.
//

#include <string>
#include <jni.h>

#include <grpc++/grpc++.h>
#include "account.grpc.pb.h"
#include "model/network_result.h"

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

        std::string requestUserSign(const std::string& account, const std::string& password) {
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
                return reply.data();
            } else {
                return status.error_message();
            }
        }

        std::string requestLogout(const std::string& token) {
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
                return reply.data();
            } else {
                return status.error_message();
            }
        }

        std::string checkConnect(const std::string& token) {
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
                return reply.data();
            } else {
                return status.error_message();
            }
        }

    private:
        std::unique_ptr<Account::Stub> stub_;
    };

    ReqResult NetworkCore::reqLogin(const std::string account,const std::string password){

        std::string host("98.142.128.182");
        int port = 50051;

        const int host_port_buf_size = 1024;
        char host_port[host_port_buf_size];
        snprintf(host_port, host_port_buf_size, "%s:%d", host.c_str(), port);

        AccountClient client(
                grpc::CreateChannel(host_port, grpc::InsecureChannelCredentials()));

        CodeReply reply = client.requestUserLogin(account,password);

//        return env->NewStringUTF(reply.c_str());


        ReqResult result;
        result.setCode(reply.code());
        result.setMsg(reply.msg());
        result.setData(reply.data());

        return result;
    }

    ReqResult NetworkCore::reqSign(const std::string account,const std::string password){

        std::string host("98.142.128.182");
        int port = 50051;

        const int host_port_buf_size = 1024;
        char host_port[host_port_buf_size];
        snprintf(host_port, host_port_buf_size, "%s:%d", host.c_str(), port);

        AccountClient client(
                grpc::CreateChannel(host_port, grpc::InsecureChannelCredentials()));

        std::string reply = client.requestUserSign(account,password);


        return ReqResult();
    }


    ReqResult NetworkCore::checkConnect(const std::string token){

        std::string host("98.142.128.182");
        int port = 50051;

        const int host_port_buf_size = 1024;
        char host_port[host_port_buf_size];
        snprintf(host_port, host_port_buf_size, "%s:%d", host.c_str(), port);

        AccountClient client(
                grpc::CreateChannel(host_port, grpc::InsecureChannelCredentials()));

        std::string reply = client.checkConnect(token);

        return ReqResult();
    }

    ReqResult NetworkCore::reqLogout(const std::string token){

        std::string host("98.142.128.182");
        int port = 50051;

        const int host_port_buf_size = 1024;
        char host_port[host_port_buf_size];
        snprintf(host_port, host_port_buf_size, "%s:%d", host.c_str(), port);

        AccountClient client(
                grpc::CreateChannel(host_port, grpc::InsecureChannelCredentials()));

        std::string reply = client.requestLogout(token);

        return ReqResult();
    }

//    extern "C"{
//        JNIEXPORT void JNICALL Java_com_wechat_mylogin_module_LoginActivity_test(JNIEnv *env,jobject thiz){
//            jclass tclss = env->FindClass("com/wechat/mylogin/network/NetworkController");
//            jmethodID getNWC = env->GetStaticMethodID(tclss,"getInstance","()Lcom/wechat/mylogin/network/NetworkController;");
//
//            //get java NetworkController
//            jobject NWC = env->CallStaticObjectMethod(tclss,getNWC);
//
//            jmethodID getNWM = env->GetMethodID(tclss,"handleLogin","(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
//
//
////        jmethodID getNWM = env->GetMethodID(tclss,"getNetworkMethod","()Lcom/wechat/mylogin/network/NetworkMethod;");
//
//            //get java NetworkMethod
//            env->CallVoidMethod(NWC,getNWM);
//
////        jclass C_NWM = env->FindClass("com/wechat/mylogin/network/NetworkMethod");
////        jmethodID M_LOGIN = env->GetMethodID(C_NWM,"handleLogin","(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
//
////        jobject LOGIN_RESULT = env->CallObjectMethod(NWM,M_LOGIN);
//        }
//    }



}