#include <cstdint>
#include <iostream>
#include <ctime>
#include <android/log.h>
#include <vector>

#include "NativeLoginListener.hpp"

#include "action_result.hpp"
#include "login_core_impl.hpp"
#include "model/proj_constants.h"
#include "utils/glob_utils.h"
#include "utils/check_param_utils.h"
#include "network/network.h"
#include "storage/share_preferences.h"

#include <iostream>
#include <exception>

#define TAG    "com.wechat.mylogin"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)

using namespace demo;
using namespace project_constants;
using namespace utils;

namespace demo {

    std::shared_ptr<LoginCore> LoginCore::create(const std::shared_ptr<LoginListener> & listener) {
        return std::make_shared<LoginCoreImpl>(listener);
    }
    
    LoginCoreImpl::LoginCoreImpl(const std::shared_ptr<LoginListener> & listener) {
        this->m_listener = listener;
    }

    bool LoginCoreImpl::isLogin = false;

    void LoginCoreImpl::user_login(const std::string & account, const std::string & password) {

        //检查账号是否正确
        std::string accountErrorMsg;
        if(!ParamUtils::checkAccountValid(account,accountErrorMsg)){
            this->m_listener->on_login_finish(ActionResult(ResultCode::LOGIN_FAIL_ACCOUNT_INVALID,accountErrorMsg,""));
            return;
        }

        //检查密码是否正确
        std::string pwdErrorMsg;
        if(!ParamUtils::checkPasswordValid(password,pwdErrorMsg)){
            this->m_listener->on_login_finish(ActionResult(ResultCode::LOGIN_FAIL_PASSWORD_INVALID,pwdErrorMsg,""));
            return;
        }

        //调用登录接口
        //获取返回数据的对象
        network::NetworkCore mNW;
        ReqResult result = mNW.reqLogin(account,password);

        //处理接口返回数据
        if (result.getCode() == ResultCode::SUCCESS){
            LoginCoreImpl::isLogin = true;
            LoginCoreImpl::updateUserInfo(account,result.getData(),"1");
        }

        //回调上层Java接口
        this->m_listener->on_login_finish(ActionResult(result.getCode(),result.getMsg(),""));
    }

    void LoginCoreImpl::user_sign(const std::string & account, const std::string & password) {

        //检查账号是否正确
        std::string accountErrorMsg;
        if(!ParamUtils::checkAccountValid(account,accountErrorMsg)){
            this->m_listener->on_login_finish(ActionResult(ResultCode::LOGIN_FAIL_ACCOUNT_INVALID,accountErrorMsg,""));
            return;
        }

        //检查密码是否正确
        std::string pwdErrorMsg;
        if(!ParamUtils::checkPasswordValid(password,pwdErrorMsg)){
            this->m_listener->on_login_finish(ActionResult(ResultCode::LOGIN_FAIL_PASSWORD_INVALID,pwdErrorMsg,""));
            return;
        }

        //调用登录接口
        //获取返回数据的对象
        network::NetworkCore mNW;
        ReqResult result = mNW.reqSign(account,password);

        //处理接口返回
        if (result.getCode() == ResultCode::SUCCESS){
            LoginCoreImpl::isLogin = true;
            LoginCoreImpl::updateUserInfo(account,result.getData(),"1");
        }

        //回调原生接口
        this->m_listener->on_login_finish(ActionResult(result.getCode(),result.getMsg(),""));
    }

    /**
     * 开启链接状态检测
     */
    void LoginCoreImpl::check_connection(){
        std::string token = storage::SharePreferences::get(Constants::TOKEN);
        std::string account = storage::SharePreferences::get(Constants::USER_ACCOUNT);

        if(token == "" || account == ""){

            //清除本地用户状态
            cleanUserInfo();

            this->m_listener->on_disconnect(ActionResult(ResultCode::DEVICE_OFFLINE,MsgTip::TOAST_ACCOUNT_OUT_OFF_LINE,""));
            return;
        }

        //获取返回数据的对象
        network::NetworkCore mNW;

        int lastTime = 0;
        while(true && LoginCoreImpl::isLogin){
            int nowTime = static_cast<int>(clock()/CLOCKS_PER_SEC);
            if (nowTime - lastTime > 5){

                //调研检查是否在线接口
                //获取返回数据的对象
                ReqResult result = mNW.checkConnect(token);

                if (result.getCode() != ResultCode::SUCCESS){

                    //如果处于登录状态，则踢下登录
                    if(LoginCoreImpl::isLogin){
                        if (result.getCode() == ResultCode::DEVICE_OFFLINE) {
                            this->m_listener->on_disconnect(ActionResult(ResultCode::DEVICE_OFFLINE,MsgTip::TOAST_ACCOUNT_OUT_OFF_LINE,""));
                        } else {
                            this->m_listener->on_disconnect(ActionResult(result.getCode(),result.getMsg(),""));
                        }
                    }

                    //清除本地用户状态
                    cleanUserInfo();

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
        std::string token = storage::SharePreferences::get(Constants::TOKEN);
        if (token == ""){
            LoginCoreImpl::cleanUserInfo();
            this->m_listener->on_logout_finish(ActionResult(ResultCode::SUCCESS,"",""));
            return;
        }

        //清除本地用户状态
        cleanUserInfo();

        //回调通知UI层
        this->m_listener->on_logout_finish(ActionResult(ResultCode::SUCCESS,"",""));

        //执行退出登录接口，结果不处理
        network::NetworkCore mNW;
        ReqResult result = mNW.reqLogout(token);

    }

    /**
     * 判断当前是否在线
     */
    void LoginCoreImpl::check_login_status(){
        std::string token = storage::SharePreferences::get(Constants::TOKEN);
        std::string account = storage::SharePreferences::get(Constants::USER_ACCOUNT);

        //用户未登录，清除登录状态
        if(token == "" || account == ""){
            cleanUserInfo();
            this->m_listener->on_check_status_finish(ActionResult(ResultCode::USER_IS_NOT_ONLINE,"",""));
            return;
        }

        //执行检查登录状态接口
        //获取返回数据的对象
        network::NetworkCore mNW;
        ReqResult result = mNW.checkConnect(token);

        if (result.getCode() == ResultCode::SUCCESS){
            //用户在线，更新本地用户状态
            LoginCoreImpl::isLogin = true;
            LoginCoreImpl::updateUserInfo(account,token,"1");
            this->m_listener->on_check_status_finish(ActionResult(ResultCode::SUCCESS,"",account));
        } else {
            //用户不在线，清除本地用户状态
            LoginCoreImpl::cleanUserInfo();
            this->m_listener->on_check_status_finish(ActionResult(ResultCode::USER_IS_NOT_ONLINE,"",""));
        }
    }

    /**
     * 清除本地用户信息
     */
    void LoginCoreImpl::cleanUserInfo(){
        LoginCoreImpl::isLogin = false;
        LoginCoreImpl::updateUserInfo("","","0");
    }

    /**
     * 更新本地用户信息
     */
    void LoginCoreImpl::updateUserInfo(std::string account,std::string token,std::string isConnect){

        //重置本地用户状态
        storage::SharePreferences::save(Constants::TOKEN,token);
        storage::SharePreferences::save(Constants::USER_ACCOUNT,account);
        storage::SharePreferences::save(Constants::IS_CONNECT,isConnect);

        //执行持久化操作
        storage::SharePreferences::execute();
    }

}