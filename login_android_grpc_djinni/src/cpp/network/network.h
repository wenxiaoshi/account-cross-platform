//
// Created by dw on 19-2-26.
//

#ifndef ANDROID_NETWORK_H
#define ANDROID_NETWORK_H

#endif //ANDROID_NETWORK_H

using namespace demo;

namespace network{

    class NetworkCore{
    public:
        ReqResult reqLogin(const std::string account,const std::string password);
        ReqResult reqSign(const std::string account,const std::string password);
        ReqResult checkConnect(const std::string token);
        ReqResult reqLogout(const std::string token);
    private:

    };

}