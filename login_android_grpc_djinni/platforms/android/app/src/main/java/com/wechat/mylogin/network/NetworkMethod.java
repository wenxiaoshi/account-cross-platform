package com.wechat.mylogin.network;

import com.alibaba.fastjson.JSONObject;
import com.wechat.mylogin.constant.Constants;
import com.wechat.mylogin.constant.ResultCode;

import java.io.InputStream;

import io.grpc.ManagedChannel;
import io.grpc.melon.account.AccountGrpc;
import io.grpc.melon.account.CodeReply;
import io.grpc.melon.account.ConnectRequest;
import io.grpc.melon.account.LoginRequest;
import io.grpc.melon.account.LogoutRequest;
import io.grpc.melon.account.SignRequest;

import static com.wechat.mylogin.network.NetworkController.SERVER_HOST;
import static com.wechat.mylogin.network.NetworkController.SERVER_PORT;

public class NetworkMethod {

    private InputStream mIsReqPem;
    private ManagedChannel channel;

    NetworkMethod(InputStream reqISPem){
        mIsReqPem = reqISPem;
    }

    private void buildManagedChannel(){
        String AUTHORITY_OVERRIDE = "wechat-login";
        channel = GrpcChannelBuilder.build(SERVER_HOST,SERVER_PORT, AUTHORITY_OVERRIDE,true,mIsReqPem,null);
    }

    public String handleLogin(final String account, final String password,final String deviceId) {
//        ManagedChannel channel = ManagedChannelBuilder.forAddress(SERVER_HOST, SERVER_PORT).usePlaintext().build();

        if (mIsReqPem == null){
            JSONObject object = new JSONObject();
            object.put(Constants.KEY_CODE,ResultCode.REQ_PEM_IS_NULL.getValue());
            return object.toJSONString();
        }

        if (channel == null){
            buildManagedChannel();
        }

        AccountGrpc.AccountBlockingStub stub = AccountGrpc.newBlockingStub(channel);
        LoginRequest request = LoginRequest.newBuilder()
                .setPhone(account)
                .setPassword(password)
                .setDeviceId(deviceId)
                .build();
        try{
            CodeReply reply = stub.requestUserLogin(request);
            JSONObject object = new JSONObject();
            object.put(Constants.KEY_CODE,reply.getCode());
            object.put(Constants.KEY_MSG,reply.getMsg());
            return object.toJSONString();
        }catch (Exception e){
            JSONObject object = new JSONObject();
            object.put(Constants.KEY_CODE,ResultCode.FAIL.getValue());
            return object.toJSONString();
        }
    }

    public String handleSign(String account, String password,final String deviceId) {
//        ManagedChannel channel = ManagedChannelBuilder.forAddress(SERVER_HOST, SERVER_PORT).usePlaintext().build();

        if (mIsReqPem == null){
            JSONObject object = new JSONObject();
            object.put(Constants.KEY_CODE,ResultCode.REQ_PEM_IS_NULL.getValue());
            return object.toJSONString();
        }

        if (channel == null){
            buildManagedChannel();
        }

        AccountGrpc.AccountBlockingStub stub = AccountGrpc.newBlockingStub(channel);
        SignRequest request = SignRequest.newBuilder()
                .setPhone(account)
                .setPassword(password)
                .setDeviceId(deviceId)
                .build();
        try{
            CodeReply reply = stub.requestUserSign(request);
            JSONObject object = new JSONObject();
            object.put(Constants.KEY_CODE,reply.getCode());
            object.put(Constants.KEY_MSG,reply.getMsg());
            return object.toJSONString();
        }catch (Exception e){
            JSONObject object = new JSONObject();
            object.put(Constants.KEY_CODE,ResultCode.FAIL.getValue());
            return object.toJSONString();
        }
    }

    public String checkConnect(final String deviceId) {
//        ManagedChannel channel = ManagedChannelBuilder.forAddress(SERVER_HOST, SERVER_PORT).usePlaintext().build();

        if (mIsReqPem == null){
            JSONObject object = new JSONObject();
            object.put(Constants.KEY_CODE,ResultCode.REQ_PEM_IS_NULL.getValue());
            return object.toJSONString();
        }

        if (channel == null){
            buildManagedChannel();
        }

        AccountGrpc.AccountBlockingStub stub = AccountGrpc.newBlockingStub(channel);
        ConnectRequest request = ConnectRequest.newBuilder()
                .setDeviceId(deviceId)
                .build();

        try{
            CodeReply reply = stub.checkConnect(request);
            JSONObject object = new JSONObject();
            object.put(Constants.KEY_CODE,reply.getCode());
            return object.toJSONString();
        }catch (Exception e){
            JSONObject object = new JSONObject();
            object.put(Constants.KEY_CODE,ResultCode.FAIL.getValue());
            return object.toJSONString();
        }
    }

    public String handleLogout(final String account) {
//        ManagedChannel channel = ManagedChannelBuilder.forAddress(SERVER_HOST, SERVER_PORT).usePlaintext().build();

        if (mIsReqPem == null){
            JSONObject object = new JSONObject();
            object.put(Constants.KEY_CODE,ResultCode.REQ_PEM_IS_NULL.getValue());
            return object.toJSONString();
        }

        if (channel == null){
            buildManagedChannel();
        }

        AccountGrpc.AccountBlockingStub stub = AccountGrpc.newBlockingStub(channel);
        LogoutRequest request = LogoutRequest.newBuilder()
                .setPhone(account)
                .build();
        try{
            CodeReply reply = stub.requestLogout(request);
            JSONObject object = new JSONObject();
            object.put(Constants.KEY_CODE,reply.getCode());
            return object.toJSONString();
        }catch (Exception e){
            JSONObject object = new JSONObject();
            object.put(Constants.KEY_CODE,ResultCode.FAIL.getValue());
            return object.toJSONString();
        }
    }

}
