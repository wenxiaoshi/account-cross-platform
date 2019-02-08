//
// Created by melon on 2019/2/3.
//

#ifndef ANDROID_PROJ_CONSTANTS_H
#define ANDROID_PROJ_CONSTANTS_H

#endif //ANDROID_PROJ_CONSTANTS_H

namespace project_constants{
    class ResultCode{
    public:
        const static int SUCCESS;
        const static int LOGIN_FAIL_PASSWORD_ERROR;
        const static int LOGIN_FAIL_ACCOUNT_NO_EXIST;
        const static int SIGN_FAIL_ALREADY_EXIST;
        const static int DEVICE_OFFLINE;
    };
}