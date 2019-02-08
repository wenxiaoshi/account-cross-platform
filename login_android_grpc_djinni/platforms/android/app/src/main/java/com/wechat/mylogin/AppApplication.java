package com.wechat.mylogin;

import android.app.Application;
import android.content.Context;

import com.wechat.mylogin.network.NetworkController;

public class AppApplication extends Application {

    private static Context context;

    @Override
    public void onCreate() {
        super.onCreate();

        context = this;
    }

    public static Context getContext(){
        return context;
    }

}
