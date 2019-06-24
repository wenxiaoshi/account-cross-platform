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
        
        /*
         主要功能
         请求登录操作
         
         入口参数
         account：           用户账号
         password：          用户密码
         
         出口参数
         ReqResult：         接口返回数据
         */
        ReqResult reqLogin(const std::string account,const std::string password);
        
        /*
         主要功能：
         请求注册操作
         
         入口参数
         account：           用户账号
         password：          用户密码
         
         出口参数
         ReqResult：         接口返回数据
         */
        ReqResult reqSign(const std::string account,const std::string password);
        
        /*
         主要功能：
         检查连接状态操作
         
         入口参数
         token：             用户凭证Token
         
         出口参数
         ReqResult：         接口返回数据
         */
        ReqResult checkConnect(const std::string token);
        
        /*
         主要功能：
         请求退出登录操作
         
         入口参数
         token：             用户凭证Token
         
         出口参数
         ReqResult：         接口返回数据
         */
        ReqResult reqLogout(const std::string token);
        
        /*
         主要功能：
         刷新凭证操作
         
         入口参数
         token：             用户凭证Token
         refreshToken：      用户凭证RedreshToken
         
         出口参数
         ReqResult：         接口返回数据
         */
        ReqResult refreshToken(const std::string token,const std::string refreshToken);
    private:

    };

}
