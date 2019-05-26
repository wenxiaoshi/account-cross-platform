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
        static string GenToken(const unsigned long uid, const string account);
        static string DecryptToken(string token);

        static string EncryptPwd(string account, string password);

        static void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c);
        
    private:
        static string GenRandomStr();
        const static long TOKEN_TIMEOUT;
        static unsigned char* AES_KEY;
        static AesEncryptor* aesEncryptor;
    };

    /**
     *  参数校验工具类
     **/
    class ParamUtils{
    public:
        static bool CheckAccountValid(string account,string & errorMsg);
        static bool CheckPasswordValid(string password,string & errorMsg);
        static bool CheckTokenValid(string token,string & errorMsg);

    private:
        static bool PatternMatch(string pattern, string source_str);
    };

}
