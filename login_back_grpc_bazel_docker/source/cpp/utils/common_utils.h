//
// Created by melon on 2019/2/28.
//

#ifndef ANDROID_TOKEN_UTILS_H
#define ANDROID_TOKEN_UTILS_H

#endif //ANDROID_TOKEN_UTILS_H

#include <string>
#include <vector>
#include "../../libs/cipher/aes_encryptor.h"
#include "../../libs/cipher/md5.h"
#include "../../libs/cipher/sha256.h"

using namespace std;
using namespace cipher_center;

namespace utils{

    /**
     *  字符串操作工具类
     **/
    class CommonUtils{
    public:

        /*
        主要功能：
        生成用户Token

        入口参数
        uid：           用户的UID
        account：       用户的账号

       	出口参数：
       	string： 	    用户的Token
        */
        static string GenToken(const unsigned long uid, const string account);

        /*
        主要功能：
        生成用户RefreshToken

        入口参数
        uid：           用户的UID
        account：       用户的账号

       	出口参数：
       	string： 	    用户的RefreshToken
        */
        static string GenRefreshToken(const unsigned long uid, const string account);

        /*
        主要功能：
        解析Token成明文字符串

        入口参数
        token：         需要进行解析的Token

       	出口参数：
       	string： 	    Token对应的明文字符串
        */
        static string DecryptToken(string token);

        /*
        主要功能：
        对明文密码进行数字签名

        入口参数
        account：       用户的账号
        password：      用户的账号

       	出口参数：
       	string： 	    进行数字签名后的用户密码
        */
        static string EncryptPwd(string account, string password);

        /*
        主要功能：
        将数字字符串转换成数字

        入口参数
        str：           需要进行转换的字符串

       	出口参数：
       	int： 	        转换后的数字
        */
        static int getIntByString(string str);

        /*
        主要功能：
        分解字符串成数组

        入口参数
        str：           需要进行分解的字符串
        v：             存储分解后的数组
        c：             分解字符串
        */
        static void SplitString(const std::string& str, std::vector<std::string>& v, const std::string& c);

        /*
        主要功能：
        分解字符串成数组

        入口参数
        str：           需要进行替换的字符串
        old：           替换的旧字符
        reg：           替换的新字符
        */
        static void replaceAll(std::string& str,const std::string old,const std::string reg);

        /*
        主要功能：
        比较两个字符串是否相等

        入口参数
        origin：        进行比较的字符串A
        target：        进行比较的字符串B

        出口参数：
       	bool： 			true表示相等；false表示不相等
        */
        static bool isEqual(string origin,string target);

        //参与密码初始化的盐值
        static string PASSWORD_SALT;

    private:

        /*
        主要功能：
        生成随机字符串

        出口参数：
       	string： 	    随机字符串
        */
        static string GenRandomStr();

        //Token过期时间
        const static long TOKEN_TIMEOUT;

        //RefreshToken过期时间
        const static long REFRESH_TOKEN_TIMEOUT;

        //AES密钥字符串
        static unsigned char* AES_KEY;

        //AES加密工具类
        static AesEncryptor* aesEncryptor;

    };

}
