#pragma once
 
#include "login_core.hpp"
#include "./model/network_result.h"

using namespace std;

namespace demo {
    
    class LoginCoreImpl : public demo::LoginCore {
        
    public:
        static bool isLogin;
        // Constructor
        LoginCoreImpl(const shared_ptr<LoginListener> & listener);

        // Our method that returns a striNativeNetworkCoreng
        // string get_hello_world();
        virtual void user_login(const string & account, const string & password) override;
  		virtual void user_sign(const string & account, const string & password) override;
        virtual void check_connection() override ;
        virtual void user_logout() override ;
        virtual void check_login_status() override ;
        ReqResult refresh_token();

    private:
        shared_ptr<LoginListener> m_listener;

        void cleanUserInfo();

        void updateUserInfo(string account,string token,string refreshToken,string isConnect);
    };


}