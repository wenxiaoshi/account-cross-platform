//
// Created by dw on 19-2-28.
//

#ifndef ANDROID_NETWORK_UTILS_H
#define ANDROID_NETWORK_UTILS_H

#endif //ANDROID_NETWORK_UTILS_H

#include <grpc++/grpc++.h>

namespace utils{

    class NetworkUtils{

    public:
        static std::shared_ptr<grpc::Channel> getNetworkChannel();

    private:
        static std::shared_ptr<grpc::Channel> channel;
    };

}