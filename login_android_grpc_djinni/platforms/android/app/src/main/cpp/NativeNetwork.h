//
// Created by melon on 2019/2/2.
//

#ifndef ANDROID_NATIVENETWORK_H
#define ANDROID_NATIVENETWORK_H

#endif //ANDROID_NATIVENETWORK_H

#include "string"

namespace native_network{

class NativeNetwork{
        public:
            NativeNetwork();
            std::string reqLogin(const std::string account,const std::string password, const std::string deviceId);
            std::string reqSign(const std::string account,const std::string password, const std::string deviceId);
            std::string checkConnect(const std::string deviceId);
            std::string reqLogout(const std::string account);
        private:

    };

}

