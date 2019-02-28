#include <cstdint>
#include <iostream>
#include <ctime>
#include <android/log.h>
#include <vector>

#include "NativeLoginListener.hpp"
#include "NativeNetwork.h"

#include "action_result.hpp"
#include "login_core_impl.hpp"
#include "model/proj_constants.h"
#include "utils/glob_utils.h"
#include <storage/share_preferences.h>
#include "utils/check_param_utils.h"

#include <iostream>
#include <exception>

#include "common_utils.h"

#define TAG    "com.wechat.mylogin"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)

using namespace demo;
using namespace project_constants;
using namespace utils;

namespace demo {

    const std::string TOKEN = "token";
    const std::string IS_CONNECT = "is_connect";
    const std::string USER_ACCOUNT = "user_account";

    bool LoginCoreImpl::isLogin = false;

    std::shared_ptr<LoginCore> LoginCore::create(const std::shared_ptr<LoginListener> & listener) {
        return std::make_shared<LoginCoreImpl>(listener);
    }
    
    LoginCoreImpl::LoginCoreImpl(const std::shared_ptr<LoginListener> & listener) {
        this->m_listener = listener;
    }

    void LoginCoreImpl::user_login(const std::string & account, const std::string & password) {

        string token = CommonUtils::GenToken(1000,"13533192332");

        token = CommonUtils::DecryptToken(token);

        //检查账号是否正确
        std::string accountErrorMsg;
        if(!CheckParam::checkAccountValid(account,accountErrorMsg)){
            this->m_listener->on_login_finish(ActionResult(ResultCode::LOGIN_FAIL_ACCOUNT_INVALID,accountErrorMsg,""));
            return;
        }

        //检查密码是否正确
        std::string pwdErrorMsg;
        if(!CheckParam::checkPasswordValid(password,pwdErrorMsg)){
            this->m_listener->on_login_finish(ActionResult(ResultCode::LOGIN_FAIL_PASSWORD_INVALID,pwdErrorMsg,""));
            return;
        }

        std::string deviceId = storage::SharePreferences::get(TOKEN);

        if (deviceId.empty()){
            //如果数据层中获取不到设备ID，则生成设备ID，并且保存到本地
            deviceId = utils::Device::getDeviceId();
            storage::SharePreferences::save(TOKEN,deviceId);
        }

        //调用登录接口
        native_network::NativeNetwork mNW;
        std::string response = mNW.reqLogin(account,password,deviceId);

        //获取返回数据的对象
        ReqResult result;

        utils::Network::getCodeByResult(result,response);

        //处理接口返回数据
        if (result.getCode() == ResultCode::SUCCESS){
            storage::SharePreferences::save(IS_CONNECT,"1");
            LoginCoreImpl::isLogin = true;
            storage::SharePreferences::save(USER_ACCOUNT,account);
        }

        //执行持久化操作
        storage::SharePreferences::execute();

        //回调原生接口
        this->m_listener->on_login_finish(ActionResult(result.getCode(),result.getMsg(),""));
    }

    void LoginCoreImpl::user_sign(const std::string & account, const std::string & password) {

        //检查账号是否正确
        std::string accountErrorMsg;
        if(!CheckParam::checkAccountValid(account,accountErrorMsg)){
            this->m_listener->on_login_finish(ActionResult(ResultCode::LOGIN_FAIL_ACCOUNT_INVALID,accountErrorMsg,""));
            return;
        }

        //检查密码是否正确
        std::string pwdErrorMsg;
        if(!CheckParam::checkPasswordValid(password,pwdErrorMsg)){
            this->m_listener->on_login_finish(ActionResult(ResultCode::LOGIN_FAIL_PASSWORD_INVALID,pwdErrorMsg,""));
            return;
        }

        std::string deviceId = storage::SharePreferences::get(TOKEN);

        if (deviceId == ""){
            //如果数据层中获取不到设备ID，则生成设备ID，并且保存到本地
            deviceId = utils::Device::getDeviceId();
            storage::SharePreferences::save(TOKEN,deviceId);
        }

        //调用注册接口
        native_network::NativeNetwork mNW;
        std::string response = mNW.reqSign(account,password,deviceId);

        //获取返回数据的对象
        ReqResult result;
        utils::Network::getCodeByResult(result,response);

        //处理接口返回
        if (result.getCode() == ResultCode::SUCCESS){
            storage::SharePreferences::save(IS_CONNECT,"1");
            LoginCoreImpl::isLogin = true;
            storage::SharePreferences::save(USER_ACCOUNT,account);
        }

        //执行持久化操作
        storage::SharePreferences::execute();

        //回调原生接口
        this->m_listener->on_login_finish(ActionResult(result.getCode(),"",""));

    }

    /**
     * 开启链接状态检测
     */
    void LoginCoreImpl::check_connection(){
        //todo 循环检测链接状态
        std::string deviceId = storage::SharePreferences::get(TOKEN);
        if(deviceId == ""){
            this->m_listener->on_disconnect(ActionResult(ResultCode::DEVICE_OFFLINE,MsgTip::TOAST_ACCOUNT_OUT_OFF_LINE,""));
            return;
        }

        native_network::NativeNetwork mNW;

        int lastTime = 0;
        while(true && LoginCoreImpl::isLogin){
            int nowTime = static_cast<int>(clock()/CLOCKS_PER_SEC);
            if (nowTime - lastTime > 5){
                std::string response = mNW.checkConnect(deviceId);
                //获取返回数据的对象
                ReqResult result;
                utils::Network::getCodeByResult(result,response);
                if (result.getCode() == ResultCode::DEVICE_OFFLINE){
                    storage::SharePreferences::save(TOKEN,"");
                    //如果处于登录状态，则踢下登录
                    if(LoginCoreImpl::isLogin){
                        storage::SharePreferences::save(IS_CONNECT,"0");
                        LoginCoreImpl::isLogin = false;
                        this->m_listener->on_disconnect(ActionResult(ResultCode::DEVICE_OFFLINE,MsgTip::TOAST_ACCOUNT_OUT_OFF_LINE,""));
                    }
                    //执行持久化操作
                    storage::SharePreferences::execute();
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
        std::string account = storage::SharePreferences::get(USER_ACCOUNT);
        if (account == ""){
            storage::SharePreferences::saveNow(IS_CONNECT,"0");
            LoginCoreImpl::isLogin = false;
            this->m_listener->on_logout_finish(ActionResult(ResultCode::SUCCESS,"",""));
            return;
        }

        //清楚本地数据
        storage::SharePreferences::save(IS_CONNECT,"0");
        LoginCoreImpl::isLogin = false;
        storage::SharePreferences::save(USER_ACCOUNT,"");
        storage::SharePreferences::execute();
        //回调通知UI层
        this->m_listener->on_logout_finish(ActionResult(ResultCode::SUCCESS,"",""));

        //执行退出登录
        native_network::NativeNetwork mNW;
        std::string response = mNW.reqLogout(account);

    }

    /**
     * 判断当前是否在线
     */
    void LoginCoreImpl::check_login_status(){
        std::string deviceId = storage::SharePreferences::get(TOKEN);
        std::string account = storage::SharePreferences::get(USER_ACCOUNT);

        //清空登录状态
        if(deviceId == "" || account == ""){
            storage::SharePreferences::save(TOKEN,"");
            storage::SharePreferences::save(USER_ACCOUNT,"");
            storage::SharePreferences::save(IS_CONNECT,"0");
            storage::SharePreferences::execute();
            LoginCoreImpl::isLogin = false;
            this->m_listener->on_check_status_finish(ActionResult(ResultCode::USER_IS_NOT_ONLINE,"",""));
            return;
        }

        native_network::NativeNetwork mNW;
        std::string response = mNW.checkConnect(deviceId);
        //获取返回数据的对象
        ReqResult result;
        utils::Network::getCodeByResult(result,response);
        if (result.getCode() == ResultCode::SUCCESS){
            storage::SharePreferences::saveNow(IS_CONNECT,"1");
            LoginCoreImpl::isLogin = true;
            this->m_listener->on_check_status_finish(ActionResult(ResultCode::SUCCESS,"",account));
            return;
        } else {
            storage::SharePreferences::save(TOKEN,"");
            storage::SharePreferences::save(USER_ACCOUNT,"");
            storage::SharePreferences::save(IS_CONNECT,"0");
            storage::SharePreferences::execute();
            LoginCoreImpl::isLogin = false;
            this->m_listener->on_check_status_finish(ActionResult(ResultCode::USER_IS_NOT_ONLINE,"",""));
            return;
        }
    }



}