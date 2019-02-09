//
// Created by melon on 2019/2/3.
//

#include <stdio.h>
#include <network_result.h>
#include <sstream>
#include <android/log.h>

#define TAG    "com.wechat.mylogin"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)

namespace demo{

    int ReqResult::getCode(){
        return this->code;
    }
    void ReqResult::setCode(std::string code){
//        std::string s = "17";
//        std::stringstream ss;
//        ss<<s;
//         int i;
//       ss>>i;
//        this->code = i;
        LOGD("aaaaaaa");
        std::string::size_type sz;   // alias of size_t
        LOGD("bbbbbb");
        int i_dec = std::stoi (code,&sz);
        LOGD("cccccc");
        this->code = i_dec;
        LOGD("ddddddd");

    }
    std::string ReqResult::getMsg(){
        return this->msg;
    }
    void ReqResult::setMsg(std::string msg){
        this->msg = msg;
    }
    std::string ReqResult::getData(){
        return this->data;
    }

    void ReqResult::setData(std::string data){
        this->data = data;
    }

}

