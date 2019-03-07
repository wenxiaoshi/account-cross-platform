package com.wechat.mylogin.module.login;

interface ILoginController {

    void actionLoginIn(String userAccount, String userPassword);

    void actionSignIn(String userAccount, String userPassword);

    void actionLogout();

}
