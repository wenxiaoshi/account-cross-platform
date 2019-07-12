#include <cstdint>
#include <iostream>
#include <sstream>
#include <ctime>
#include <vector>
#include <exception>

# define APP_PLATFORM ANDROID

#include "NativeLoginListener.hpp"
//#include "SCLoginListener.h"

#include "action_result.hpp"
#include "login_core_impl.hpp"
#include "model/proj_constants.h"
#include "utils/glob_utils.h"
#include "utils/check_param_utils.h"
#include "network/network.h"
#include "storage/share_preferences.h"
#include "json.hpp"
#include "./utils/log_utils.h"

#define LOGD(msg)  utils::LogUtil::LOGD(msg);
#define LOGE(msg)  utils::LogUtil::LOGE(msg);

using namespace demo;
using namespace project_constants;
using namespace utils;
using json = nlohmann::json;

namespace demo {

    std::shared_ptr<LoginCore> LoginCore::create(const std::shared_ptr<LoginListener> & listener) {
        return std::make_shared<LoginCoreImpl>(listener);
    }

    LoginCoreImpl::LoginCoreImpl(const std::shared_ptr<LoginListener> & listener) {
        this->m_listener = listener;
    }

    bool LoginCoreImpl::isLogin = false;

    /**
     * 执行用户登录
     */
    void LoginCoreImpl::user_login(const std::string & account, const std::string & password) {

        //检查账号是否正确
        std::string accountErrorMsg;
        if(!ParamUtils::checkAccountValid(account,accountErrorMsg)){
            LOGD("account is invalid");
            this->m_listener->on_login_finish(ActionResult(ClientCode::LOGIN_FAIL_ACCOUNT_INVALID,accountErrorMsg,""));
            return;
        }

        //检查密码是否正确
        std::string pwdErrorMsg;
        if(!ParamUtils::checkPasswordValid(password,pwdErrorMsg)){
            LOGD("password is invalid");
            this->m_listener->on_login_finish(ActionResult(ClientCode::LOGIN_FAIL_PASSWORD_INVALID,pwdErrorMsg,""));
            return;
        }

        //调用登录接口
        //获取返回数据的对象
        network::NetworkCore mNW;
        ReqResult result = mNW.reqLogin(account,password);

        //处理接口返回数据
        if (result.getCode() == ResultCode::SUCCESS){
            LoginCoreImpl::isLogin = true;
            auto j = json::parse(result.getData());
            string token = j[Constants::TOKEN];
            string refresh_token = j[Constants::REFRESH_TOKEN];
            int32_t token_expiration_time = j[Constants::TOKEN_EXPIRATION_TIME];
            LoginCoreImpl::updateUserInfo(account,token,refresh_token,token_expiration_time,"1");
            LOGD("login success");
        }

        //回调上层Java接口
        this->m_listener->on_login_finish(ActionResult(result.getCode(),result.getMsg(),""));
    }

    /**
     * 执行用户注册
     */
    void LoginCoreImpl::user_sign(const std::string & account, const std::string & password) {

        //检查账号是否正确
        std::string accountErrorMsg;
        if(!ParamUtils::checkAccountValid(account,accountErrorMsg)){
            LOGD("account is invalid");
            this->m_listener->on_login_finish(ActionResult(ClientCode::LOGIN_FAIL_ACCOUNT_INVALID,accountErrorMsg,""));
            return;
        }

        //检查密码是否正确
        std::string pwdErrorMsg;
        if(!ParamUtils::checkPasswordValid(password,pwdErrorMsg)){
            LOGD("password is invalid");
            this->m_listener->on_login_finish(ActionResult(ClientCode::LOGIN_FAIL_PASSWORD_INVALID,pwdErrorMsg,""));
            return;
        }

        //调用登录接口
        //获取返回数据的对象
        network::NetworkCore mNW;
        ReqResult result = mNW.reqSign(account,password);

        //处理接口返回
        if (result.getCode() == ResultCode::SUCCESS){
            LoginCoreImpl::isLogin = true;
            auto j = json::parse(result.getData());
            string token = j[Constants::TOKEN];
            string refresh_token = j[Constants::REFRESH_TOKEN];
            int32_t token_expiration_time = j[Constants::TOKEN_EXPIRATION_TIME];
            LoginCoreImpl::updateUserInfo(account,token,refresh_token,token_expiration_time,"1");
            LOGD("login success");
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
            LOGD("token or account is empty");
            //清除本地用户状态
            cleanUserInfo();

            this->m_listener->on_disconnect(ActionResult(ClientCode::USER_ACCOUNT_FO_EMPTY,ToastTip::TOAST_ACCOUNT_INFO_EMPTY,""));
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

                if (result.getCode() == ResultCode::CheckConnect_TokenHadExpire){
                    LOGD("exec refresh user token");
                    //token失效，调用刷新token接口
                    ReqResult refreshResult = LoginCoreImpl::refresh_token();
                    if(refreshResult.getCode() != ResultCode::SUCCESS){
                        LOGD("refresh user token fail");
                        //如果处于登录状态，则踢下登录
                        if(LoginCoreImpl::isLogin){
                            this->m_listener->on_disconnect(ActionResult(result.getCode(),result.getMsg(),""));
                        }
                        //清除本地用户状态
                        cleanUserInfo();
                        break;
                    }else{
                        LOGD("refresh user token success");
                        token = storage::SharePreferences::get(Constants::TOKEN);
                    }
                } else if (result.getCode() == ResultCode::CheckConnect_AccountTokenNotEqual){
                    LOGD("user may be login on other device");
                    //token与服务器段token不匹配，判断用户被挤下线
                    //如果处于登录状态，则踢下登录
                    if(LoginCoreImpl::isLogin){
                        this->m_listener->on_disconnect(ActionResult(ClientCode::USER_IS_OUT_OF_LINE,ToastTip::TOAST_ACCOUNT_OUT_OFF_LINE,""));
                    }
                    //清除本地用户状态
                    cleanUserInfo();
                    break;
                }else if(result.getCode() != ResultCode::SUCCESS){
                    LOGD("keep heart beat fail");
                    //如果处于登录状态，则踢下登录
                    if(LoginCoreImpl::isLogin){
                        this->m_listener->on_disconnect(ActionResult(result.getCode(),result.getMsg(),""));
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
            LOGD("token is empty");
            LoginCoreImpl::cleanUserInfo();
            this->m_listener->on_logout_finish(ActionResult(ClientCode::SUCCESS,"",""));
            return;
        }

        //清除本地用户状态
        cleanUserInfo();

        //回调通知UI层
        this->m_listener->on_logout_finish(ActionResult(ClientCode::SUCCESS,"",""));

        //执行退出登录接口，结果不处理
        network::NetworkCore mNW;
        ReqResult result = mNW.reqLogout(token);
        LOGD("user logout success");
    }

    /**
     * 判断当前是否在线
     */
    void LoginCoreImpl::check_login_status(){
        std::string token = storage::SharePreferences::get(Constants::TOKEN);
        const std::string refreshToken = storage::SharePreferences::get(Constants::REFRESH_TOKEN);
        std::string account = storage::SharePreferences::get(Constants::USER_ACCOUNT);
        int32_t tokenExpirationTime = storage::SharePreferences::getInt32(Constants::TOKEN_EXPIRATION_TIME);


        //用户未登录，清除登录状态
        if(token == "" || account == ""){
            LOGD("token or account is empty");
            cleanUserInfo();
            this->m_listener->on_check_status_finish(ActionResult(ClientCode::USER_IS_NOT_ONLINE,"",""));
            return;
        }

        //执行检查登录状态接口
        //获取返回数据的对象
        network::NetworkCore mNW;
        ReqResult result = mNW.checkConnect(token);

        if (result.getCode() == ResultCode::CheckConnect_TokenHadExpire){
            LOGD("exec refresh user token");
            //token失效，调用刷新token接口
            ReqResult refreshResult = LoginCoreImpl::refresh_token();
            if(refreshResult.getCode() != ResultCode::SUCCESS){
                LOGD("refresh user token fail");
                //用户不在线，清除本地用户状态
                LoginCoreImpl::cleanUserInfo();
                this->m_listener->on_check_status_finish(ActionResult(ClientCode::USER_IS_NOT_ONLINE,"",""));
            }else{
                LOGD("refresh user token success");
                //用户在线，更新本地用户状态
                LoginCoreImpl::isLogin = true;
                this->m_listener->on_check_status_finish(ActionResult(ClientCode::SUCCESS,"",account));
            }
        } else if (result.getCode() == ClientCode::SUCCESS){
            LOGD("user is online");
            //用户在线，更新本地用户状态
            LoginCoreImpl::isLogin = true;
            LoginCoreImpl::updateUserInfo(account,token,refreshToken,tokenExpirationTime,"1");
            this->m_listener->on_check_status_finish(ActionResult(ClientCode::SUCCESS,"",account));
        } else {
            LOGD("user is offline");
            //用户不在线，清除本地用户状态
            LoginCoreImpl::cleanUserInfo();
            this->m_listener->on_check_status_finish(ActionResult(ClientCode::USER_IS_NOT_ONLINE,"",""));
        }
    }

    /**
     * 请求服务端刷新本地Token
     */
    ReqResult LoginCoreImpl::refresh_token(){
        std::string token = storage::SharePreferences::get(Constants::TOKEN);
        std::string refreshToken = storage::SharePreferences::get(Constants::REFRESH_TOKEN);
        std::string account = storage::SharePreferences::get(Constants::USER_ACCOUNT);

        //用户未登录，清除登录状态
        if(token == "" || refreshToken == ""){
            LOGD("token or refresh_token is empty");
            return ReqResult();
        }

        //执行刷新用户Token接口
        //获取返回数据的对象
        network::NetworkCore mNW;
        ReqResult result = mNW.refreshToken(token, refreshToken);
        if (result.getCode() == ResultCode::SUCCESS){
            LOGD("refresh user token success");
            LoginCoreImpl::isLogin = true;
            auto j = json::parse(result.getData());
            string token = j[Constants::TOKEN];
            string refresh_token = j[Constants::REFRESH_TOKEN];
            int32_t token_expiration_time = j[Constants::TOKEN_EXPIRATION_TIME];
            LoginCoreImpl::updateUserInfo(account,token,refresh_token,token_expiration_time,"1");
        }else{
            LOGD("refresh user token fail");
        }

        return result;
    }

    /**
     * 清除本地用户信息
     */
    void LoginCoreImpl::cleanUserInfo(){
        LOGD("clean user info");
        LoginCoreImpl::isLogin = false;
        LoginCoreImpl::updateUserInfo("","","",-1,"0");
    }

    /**
     * 更新本地用户信息
     */
    void LoginCoreImpl::updateUserInfo(std::string account,std::string token,std::string refreshToken,int32_t tokenExpirationTime, std::string isConnect){
        LOGD("update user info");

        stringstream ss;
        ss << tokenExpirationTime;
        string tokenExpirationTimeStr;
        ss >> tokenExpirationTimeStr;

        //重置本地用户状态
        storage::SharePreferences::save(Constants::TOKEN,token);
        storage::SharePreferences::save(Constants::REFRESH_TOKEN,refreshToken);
        storage::SharePreferences::save(Constants::TOKEN_EXPIRATION_TIME,tokenExpirationTimeStr);
        storage::SharePreferences::save(Constants::USER_ACCOUNT,account);
        storage::SharePreferences::save(Constants::IS_CONNECT,isConnect);

        //执行持久化操作
        storage::SharePreferences::execute();
    }

}
