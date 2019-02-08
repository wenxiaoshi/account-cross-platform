package com.wechat.mylogin.utils;

import java.util.UUID;

public class DeviceUtils {

    public static String getDeviceId(){
        UUID uuid = UUID.randomUUID();
        return uuid.toString();
    }



}
