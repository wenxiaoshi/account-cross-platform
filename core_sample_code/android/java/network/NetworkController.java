package com.wechat.mylogin.network;

import com.wechat.mylogin.AppApplication;

import java.io.IOException;
import java.io.InputStream;

public class NetworkController {

    public static final String SERVER_HOST = "98.142.128.182"; //国外-Ubuntu
//    public static final String SERVER_HOST = "47.92.196.45"; //阿里云-Ubuntu

    public static final int SERVER_PORT = 50051;

    private static NetworkController mNetworkController;

    private static InputStream reqISPem = null;

    public static NetworkController getInstance(){
        if (mNetworkController == null){
            synchronized (NetworkController.class){
                if (mNetworkController == null){
                    mNetworkController = new NetworkController();
                }
            }
        }
        return mNetworkController;
    }

    private NetworkMethod mNetworkMethod;


    private NetworkController(){
        try {
            reqISPem = AppApplication.getContext().getResources().getAssets().open("server.pem");
        } catch (IOException e) {
            e.printStackTrace();
        }
        mNetworkMethod = new NetworkMethod(reqISPem);
    }

    public NetworkMethod getNetworkMethod(){
        return mNetworkMethod;
    }



}
