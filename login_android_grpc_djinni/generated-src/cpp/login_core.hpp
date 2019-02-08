// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from demo.djinni

#pragma once

#include <memory>
#include <string>

namespace demo {

class LoginListener;

class LoginCore {
public:
    virtual ~LoginCore() {}

    static std::shared_ptr<LoginCore> create(const std::shared_ptr<LoginListener> & listener);

    virtual void user_login(const std::string & account, const std::string & password) = 0;

    virtual void user_sign(const std::string & account, const std::string & password) = 0;

    virtual void user_logout() = 0;

    virtual void check_login_status() = 0;

    virtual void check_connection() = 0;
};

}  // namespace demo
