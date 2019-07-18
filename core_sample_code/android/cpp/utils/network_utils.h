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
        
        /*
         主要功能：
         获取网络请求工具对象
         
         出口参数
         std::shared_ptr<grpc::Channel>：    网络请求Channel对象
         */
        static std::shared_ptr<grpc::Channel> getNetworkChannel();

    private:
        static std::shared_ptr<grpc::Channel> channel;
    };

}
