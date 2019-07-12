package com.wechat.mylogin.utils;

import android.content.Context;
import android.widget.Toast;

public class CommonUtils {

    public static void showToast(Context context,int resId,int type){
        showToast(context,context.getString(resId),type);
    }

    public static void showToast(Context context,String content,int type){
        Toast.makeText(context,content,type).show();
    }

}
