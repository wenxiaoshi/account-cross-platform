// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from demo.djinni

package com.wechat.clibs;

public abstract class LoginListener {
    public abstract void onLoginFinish(int code);

    public abstract void onSignFinish(int code);

    public abstract void onLogoutFinish(int code);

    public abstract void onCheckStatusFinish(int code, String account);

    public abstract void onDisconnect();

    public abstract void toast(String content);
}
