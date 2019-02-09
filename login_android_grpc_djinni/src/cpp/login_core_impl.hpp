#pragma once
 
#include "login_core.hpp"
#include "network_result.h"

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

        void getCodeByResult(ReqResult & result,std::string response);

        bool checkAccountValid(std::string account);
        bool checkPasswordValid(std::string password);

    private:
        std::shared_ptr<LoginListener> m_listener;

    };


}