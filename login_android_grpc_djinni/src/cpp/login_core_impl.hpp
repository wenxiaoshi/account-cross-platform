#pragma once
 
#include "login_core.hpp"

namespace demo {
    
    class LoginCoreImpl : public demo::LoginCore {
        
    public:
        static bool isLogin;
        // Constructor
        LoginCoreImpl(const std::shared_ptr<LoginListener> & listener);

        // Our method that returns a striNativeNetworkCoreng
        // std::string get_hello_world();
        virtual void user_login(const std::string & account, const std::string & password) override;
  		virtual void user_sign(const std::string & account, const std::string & password) override;
        virtual void check_connection() override ;
        virtual void user_logout() override ;
        virtual void check_login_status() override ;

    private:
        std::shared_ptr<LoginListener> m_listener;

        void cleanUserInfo();

        void updateUserInfo(std::string account,std::string token,std::string isConnect);
    };


}