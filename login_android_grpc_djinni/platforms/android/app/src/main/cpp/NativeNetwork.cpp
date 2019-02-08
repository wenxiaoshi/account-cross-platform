//
// Created by melon on 2019/2/2.
//

#include <dlfcn.h>
#include "jni.h"
#include "NativeNetwork.h"
#include "NativeUtils.h"

namespace native_network{

    NativeNetwork::NativeNetwork(){

    }

    std::string NativeNetwork::reqLogin(const std::string account,const std::string password, const std::string deviceId){

        JNIEnv* env = utils::NativeUtils::dlsmgetenv();

        jclass C_NWC = env->FindClass("com/wechat/mylogin/network/NetworkController");
        jclass C_NWM = env->FindClass("com/wechat/mylogin/network/NetworkMethod");

        jmethodID M_GNWC = env->GetStaticMethodID(C_NWC,"getInstance","()Lcom/wechat/mylogin/network/NetworkController;");
        jmethodID M_GNWM = env->GetMethodID(C_NWC,"getNetworkMethod","()Lcom/wechat/mylogin/network/NetworkMethod;");
        jmethodID M_LOGIN = env->GetMethodID(C_NWM,"handleLogin","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

        //get java NetworkController
        jobject NWC = env->CallStaticObjectMethod(C_NWC,M_GNWC);

        //get java NetworkMethod
        jobject NWM = env->CallObjectMethod(NWC,M_GNWM);

        //do request
        jstring LOGIN_RESULT = static_cast<jstring>(env->CallObjectMethod(NWM, M_LOGIN, utils::NativeUtils::getJStringByStdString(env, account), utils::NativeUtils::getJStringByStdString(env, password), utils::NativeUtils::getJStringByStdString(env, deviceId)));

        return utils::NativeUtils::getStdStringByJString(env, LOGIN_RESULT);
    }

    std::string NativeNetwork::reqSign(const std::string account,const std::string password, const std::string deviceId){
        JNIEnv* env = utils::NativeUtils::dlsmgetenv();

        jclass C_NWC = env->FindClass("com/wechat/mylogin/network/NetworkController");
        jclass C_NWM = env->FindClass("com/wechat/mylogin/network/NetworkMethod");

        jmethodID M_GNWC = env->GetStaticMethodID(C_NWC,"getInstance","()Lcom/wechat/mylogin/network/NetworkController;");
        jmethodID M_GNWM = env->GetMethodID(C_NWC,"getNetworkMethod","()Lcom/wechat/mylogin/network/NetworkMethod;");
        jmethodID M_LOGIN = env->GetMethodID(C_NWM,"handleSign","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

        //get java NetworkController
        jobject NWC = env->CallStaticObjectMethod(C_NWC,M_GNWC);

        //get java NetworkMethod
        jobject NWM = env->CallObjectMethod(NWC,M_GNWM);

        //do request
        jstring LOGIN_RESULT = static_cast<jstring>(env->CallObjectMethod(NWM, M_LOGIN, utils::NativeUtils::getJStringByStdString(env, account), utils::NativeUtils::getJStringByStdString(env, password), utils::NativeUtils::getJStringByStdString(env, deviceId)));

        return utils::NativeUtils::getStdStringByJString(env, LOGIN_RESULT);
    }


    std::string NativeNetwork::checkConnect(const std::string deviceId){
        JNIEnv* env = utils::NativeUtils::dlsmgetenv();

        jclass C_NWC = env->FindClass("com/wechat/mylogin/network/NetworkController");
        jclass C_NWM = env->FindClass("com/wechat/mylogin/network/NetworkMethod");

        jmethodID M_GNWC = env->GetStaticMethodID(C_NWC,"getInstance","()Lcom/wechat/mylogin/network/NetworkController;");
        jmethodID M_GNWM = env->GetMethodID(C_NWC,"getNetworkMethod","()Lcom/wechat/mylogin/network/NetworkMethod;");
        jmethodID M_LOGIN = env->GetMethodID(C_NWM,"checkConnect","(Ljava/lang/String;)Ljava/lang/String;");

        //get java NetworkController
        jobject NWC = env->CallStaticObjectMethod(C_NWC,M_GNWC);

        //get java NetworkMethod
        jobject NWM = env->CallObjectMethod(NWC,M_GNWM);

        //do request
        jstring RESULT = static_cast<jstring>(env->CallObjectMethod(NWM, M_LOGIN, utils::NativeUtils::getJStringByStdString(env, deviceId)));

        return utils::NativeUtils::getStdStringByJString(env, RESULT);
    }

    std::string NativeNetwork::reqLogout(const std::string account){
        JNIEnv* env = utils::NativeUtils::dlsmgetenv();

        jclass C_NWC = env->FindClass("com/wechat/mylogin/network/NetworkController");
        jclass C_NWM = env->FindClass("com/wechat/mylogin/network/NetworkMethod");

        jmethodID M_GNWC = env->GetStaticMethodID(C_NWC,"getInstance","()Lcom/wechat/mylogin/network/NetworkController;");
        jmethodID M_GNWM = env->GetMethodID(C_NWC,"getNetworkMethod","()Lcom/wechat/mylogin/network/NetworkMethod;");
        jmethodID M_LOGIN = env->GetMethodID(C_NWM,"handleLogout","(Ljava/lang/String;)Ljava/lang/String;");

        //get java NetworkController
        jobject NWC = env->CallStaticObjectMethod(C_NWC,M_GNWC);

        //get java NetworkMethod
        jobject NWM = env->CallObjectMethod(NWC,M_GNWM);

        //do request
        jstring RESULT = static_cast<jstring>(env->CallObjectMethod(NWM, M_LOGIN, utils::NativeUtils::getJStringByStdString(env, account)));

        return utils::NativeUtils::getStdStringByJString(env, RESULT);
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