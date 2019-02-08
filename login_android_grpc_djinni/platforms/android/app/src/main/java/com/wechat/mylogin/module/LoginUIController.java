package com.wechat.mylogin.module;

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
