package com.wechat.mylogin.module.login;

interface LoginUIController {

    void performLoginSuccess();

    void performSignSuccess();

    void performLoginFail();

    void performSignFail();

    void performLogout();

    void performUserOnline(String account);

    void toastMsg(String content);

    void disconnect();

}
