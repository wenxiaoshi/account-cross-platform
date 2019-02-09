#include <cstdint>
#include <iostream>
#include <ctime>

#include "login_core_impl.hpp"
#include <proj_constants.h>

#include "NativeLoginListener.hpp"
#include "NativeNetwork.h"
#include "glob_utils.h"

#include <iostream>
#include <exception>

using namespace demo;
using namespace project_constants;

namespace demo {

    const std::string DEVICE_KEY = "device_id";
    const std::string IS_CONNECT = "is_connect";
    const std::string USER_ACCOUNT = "user_account";

    const std::string TOAST_ACCOUNT_ERROR = "账号不能为空，请重新输入";
    const std::string TOAST_PASSWORD_ERROR = "密码不能为空，请重新输入";

    bool LoginCoreImpl::isLogin = false;

    std::shared_ptr<LoginCore> LoginCore::create(const std::shared_ptr<LoginListener> & listener) {
        return std::make_shared<LoginCoreImpl>(listener);
    }
    
    LoginCoreImpl::LoginCoreImpl(const std::shared_ptr<LoginListener> & listener) {
        this->m_listener = listener;
    }

    void LoginCoreImpl::user_login(const std::string & account, const std::string & password) {


//        utils::Device::sss.insert("1");
//        utils::Device::sss.insert("1");
//        utils::Device::sss.insert("1");
//        utils::Device::sss.insert("2");
//        utils::Device::sss.insert("3");
//        string ss = *utils::Device::sss.find("1");
//        ss = *utils::Device::sss.find("2");
//
//        string ss = "sas";
//        if (strncmp("",ss.data(),ss.length()) == 0){
//            ss = "as";
//        } else{
//            ss = "fa";
//        }


//        utils::Device::ss.HMInsert("Hello", "World");
//        utils::Device::ss.HMInsert("why", "dream");
//        utils::Device::ss.HMInsert("c++", "good");
//        utils::Device::ss.HMInsert("welcome", "haha");
//        utils::Device::ss.HMInsert("welcome", "asas");
//
//        string ss = "after insert:";
//        ss = hashmap.HMFind("welcome").c_str();
//        ss = hashmap.HMFind("welcome1").c_str();
//        ss = hashmap.HMFind("c++").c_str();
//        ss = hashmap["why"].c_str();
//        ss = hashmap["hello"].c_str();
        
        //检查账号密码是否正确
        if(!checkAccountValid(account)){
            this->m_listener->toast(TOAST_ACCOUNT_ERROR);
            return;
        }

        if(!checkPasswordValid(password)){
            this->m_listener->toast(TOAST_PASSWORD_ERROR);
            return;
        }

        std::string deviceId = utils::Storage::getData(DEVICE_KEY);

        if (deviceId == ""){
            //如果数据层中获取不到设备ID，则生成设备ID，并且保存到本地
            deviceId = utils::Device::getDeviceId();
            utils::Storage::saveData(DEVICE_KEY,deviceId);
        }

        //调用登录接口
        native_network::NativeNetwork mNW;
        std::string response = mNW.reqLogin(account,password,deviceId);

        printf("111111\n");
        fflush(stdout);

        //获取返回数据的对象
        ReqResult result = getCodeByResult(response);

        printf("222222\n");
        fflush(stdout);

        //处理接口返回数据
        if (result.getCode() == ResultCode::SUCCESS){
            utils::Storage::saveData(IS_CONNECT,"1");
            LoginCoreImpl::isLogin = true;
            utils::Storage::saveData(USER_ACCOUNT,account);
        }

        printf("333333\n");
        fflush(stdout);

        //回调原生接口
        this->m_listener->on_login_finish(result.getCode());


    }

    void LoginCoreImpl::user_sign(const std::string & account, const std::string & password) {
        //检查账号密码是否正确
        if(!checkAccountValid(account)){
            this->m_listener->toast(TOAST_ACCOUNT_ERROR);
            return;
        }

        if(!checkPasswordValid(password)){
            this->m_listener->toast(TOAST_PASSWORD_ERROR);
            return;
        }

        std::string deviceId = utils::Storage::getData(DEVICE_KEY);

        if (deviceId == ""){
            //如果数据层中获取不到设备ID，则生成设备ID，并且保存到本地
            deviceId = utils::Device::getDeviceId();
            utils::Storage::saveData(DEVICE_KEY,deviceId);
        }

        //调用注册接口
        native_network::NativeNetwork mNW;
        std::string response = mNW.reqSign(account,password,deviceId);

        //获取返回数据的对象
        ReqResult result = getCodeByResult(response);

        //处理接口返回
        if (result.getCode() == ResultCode::SUCCESS){
            utils::Storage::saveData(IS_CONNECT,"1");
            LoginCoreImpl::isLogin = true;
            utils::Storage::saveData(USER_ACCOUNT,account);
        }

        //回调原生接口
        this->m_listener->on_sign_finish(result.getCode());


    }

    /**
     * 开启链接状态检测
     */
    void LoginCoreImpl::check_connection(){
        //todo 循环检测链接状态
        std::string deviceId = utils::Storage::getData(DEVICE_KEY);
        if(deviceId == ""){
            this->m_listener->on_disconnect();
            return;
        }

        native_network::NativeNetwork mNW;

        int lastTime = 0;
        while(true && LoginCoreImpl::isLogin){
            int nowTime = static_cast<int>(clock()/CLOCKS_PER_SEC);
            if (nowTime - lastTime > 5){
                std::string response = mNW.checkConnect(deviceId);
                //获取返回数据的对象
                ReqResult result = getCodeByResult(response);
                if (result.getCode() == ResultCode::DEVICE_OFFLINE){
                    utils::Storage::saveData(DEVICE_KEY,"");
                    //如果处于登录状态，则踢下登录
                    if(LoginCoreImpl::isLogin){
                        utils::Storage::saveData(IS_CONNECT,"0");
                        LoginCoreImpl::isLogin = false;
                        this->m_listener->on_disconnect();
                    }
                    break;
                }
                lastTime = nowTime;
            }
        }

    }

    /**
     * 退出登录
     */
    void LoginCoreImpl::user_logout(){
        //获取账号，发起退出登录请求
        std::string account = utils::Storage::getData(USER_ACCOUNT);
        if (account == ""){
            utils::Storage::saveData(IS_CONNECT,"0");
            LoginCoreImpl::isLogin = false;
            this->m_listener->on_logout_finish(0);
            return;
        }

        //清楚本地数据
        utils::Storage::saveData(IS_CONNECT,"0");
        LoginCoreImpl::isLogin = false;
        utils::Storage::saveData(USER_ACCOUNT,"");

        //回调通知UI层
        this->m_listener->on_logout_finish(0);

        //执行退出登录
        native_network::NativeNetwork mNW;
        std::string response = mNW.reqLogout(account);

    }

    /**
     * 判断当前是否在线
     */
    void LoginCoreImpl::check_login_status(){
        std::string deviceId = utils::Storage::getData(DEVICE_KEY);
        std::string account = utils::Storage::getData(USER_ACCOUNT);

        //清空登录状态
        if(deviceId == "" || account == ""){
            utils::Storage::saveData(DEVICE_KEY,"");
            utils::Storage::saveData(USER_ACCOUNT,"");
            utils::Storage::saveData(IS_CONNECT,"0");
            LoginCoreImpl::isLogin = false;
            this->m_listener->on_check_status_finish(-1,"");
            return;
        }

        native_network::NativeNetwork mNW;
        std::string response = mNW.checkConnect(deviceId);
        //获取返回数据的对象
        ReqResult result = getCodeByResult(response);
        if (result.getCode() == ResultCode::SUCCESS){
            utils::Storage::saveData(IS_CONNECT,"1");
            LoginCoreImpl::isLogin = true;
            this->m_listener->on_check_status_finish(0,account);
            return;
        } else {
            utils::Storage::saveData(DEVICE_KEY,"");
            utils::Storage::saveData(USER_ACCOUNT,"");
            utils::Storage::saveData(IS_CONNECT,"0");
            LoginCoreImpl::isLogin = false;
            this->m_listener->on_check_status_finish(-1,account);
            return;
        }
    }

    bool checkAccountValid(std::string account){
        if (account == ""){
            return false;
        }
        return true;
    }

    bool checkPasswordValid(std::string password){
        if (password == ""){
            return false;
        }
        return true;
    }

    ReqResult getCodeByResult(std::string result){
        ReqResult reqResult;
        utils::String::replaceAll(result,"{","");
        utils::String::replaceAll(result,"}","");
        utils::String::replaceAll(result,"\"","");
        std::vector<std::string> v;
        utils::String::splitString(result, v,",");
        for(std::string map : v){
            std::vector<std::string> kv;
            utils::String::splitString(map, kv,":");
            if(kv[0] == "code"){
                reqResult.setCode(kv[1]);
            }else if(kv[0] == "msg"){
                reqResult.setMsg(kv[1]);
            }else if(kv[0] == "data"){
                reqResult.setData(kv[1]);
            }
        }
        return reqResult;
    }

}