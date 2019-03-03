//
// Created by melon on 2019/2/28.
//

#ifndef ANDROID_TOKEN_UTILS_H
#define ANDROID_TOKEN_UTILS_H

#endif //ANDROID_TOKEN_UTILS_H

#include <string>
#include <vector>
#include "aes_encryptor.h"
#include "md5.h"

using namespace std;
using namespace cipher_center;

namespace utils{

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

}
