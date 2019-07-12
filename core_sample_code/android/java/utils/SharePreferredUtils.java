package com.wechat.mylogin.utils;

import android.content.Context;
import android.text.TextUtils;

import com.wechat.mylogin.AppApplication;

public class SharePreferredUtils {

    private static final String PREFE_NAME = "global_prefe";

    public static String getString(String key){
        if (TextUtils.isEmpty(key)){
            return "";
        }
        return AppApplication.getContext().getSharedPreferences(PREFE_NAME,Context.MODE_PRIVATE).getString(key,"");
    }

    public static boolean putString(String key,String value){
        if (TextUtils.isEmpty(key) || value == null){
            return false;
        }
        return AppApplication.getContext().getSharedPreferences(PREFE_NAME,Context.MODE_PRIVATE).edit().putString(key,value).commit();
    }


}
