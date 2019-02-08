package com.wechat.mylogin.module;

interface ILoginController {

    void actionLoginIn(String userAccount, String userPassword);

    void actionSignIn(String userAccount, String userPassword);

    void actionLogout();

}
