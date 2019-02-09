package com.wechat.mylogin.module;


import android.os.Handler;
import android.os.Looper;

import com.wechat.clibs.LoginCore;
import com.wechat.mylogin.constant.ResultCode;

import java.lang.ref.WeakReference;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;


public class LoginController implements ILoginController {

    private WeakReference<LoginActivity> pageActivity;
    private WeakReference<LoginUIController> mUiController;
    private LoginCore mLoginCore;
    private LoginListener mLoginListener;

    private ExecutorService executorCache = Executors.newFixedThreadPool(3);
    private ExecutorService executorFix = Executors.newFixedThreadPool(1);

    private Handler mMainHandler = new Handler(Looper.getMainLooper());

    public LoginController(LoginActivity activity, LoginUIController controller){
        pageActivity = new WeakReference<>(activity);
        mUiController = new WeakReference<>(controller);

        mLoginListener = new LoginListener();
        mLoginCore = LoginCore.create(mLoginListener);

        //进入页面，判断是否用户在线
        executorCache.execute(new Runnable() {
            @Override
            public void run() {
                mLoginCore.checkLoginStatus();
            }
        });
    }

    /**
     * 发起登录操作
     * @param userAccount
     * @param userPassword
     */
    @Override
    public void actionLoginIn(final String userAccount, final String userPassword){
//        mLoginCore.userLogin(userAccount,userPassword);
        executorCache.execute(new Runnable() {
            @Override
            public void run() {
                mLoginCore.userLogin(userAccount,userPassword);
            }
        });
    }

    /**
     * 发起注册操作
     * @param userAccount
     * @param userPassword
     */
    @Override
    public void actionSignIn(final String userAccount, final String userPassword){
        executorCache.execute(new Runnable() {
            @Override
            public void run() {
                mLoginCore.userSign(userAccount,userPassword);
            }
        });
    }

    /**
     * 发起退出登录操作
     */
    @Override
    public void actionLogout() {
        executorCache.execute(new Runnable() {
            @Override
            public void run() {
                mLoginCore.userLogout();
            }
        });
    }

    /**
     * 发起退出登录操作
     */
    private void checkConnect(){
        executorFix.execute(new Runnable() {
            @Override
            public void run() {
                mLoginCore.checkConnection();
            }
        });
    }

    /**
     * 处理登录完成后的UI方法调用
     * @param code
     */
    private void handleLoginResult(int code){
        if (isPageRecycle()){
            return;
        }
        if (code == ResultCode.SUCCESS.getValue()){
            mUiController.get().performLoginSuccess();
            checkConnect();
        }else {
            mUiController.get().performLoginFail();
        }
    }

    /**
     * 处理注册完成后的UI方法调用
     * @param code
     */
    private void handleSignResult(int code){
        if (isPageRecycle()){
            return;
        }
        if (code == ResultCode.SUCCESS.getValue()){
            mUiController.get().performSignSuccess();
            checkConnect();
        }else {
            mUiController.get().performSignFail();
        }
    }

    /**
     * 处理断开登录状态后的UI方法调用
     */
    private void handleDeviceDisconnect(){
        if (isPageRecycle()){
            return;
        }
        mUiController.get().disconnect();
    }

    /**
     * 处理退出登录状态后的UI方法调用
     */
    private void handleLogout(){
        if (isPageRecycle()){
            return;
        }
        mUiController.get().performLogout();
    }

    /**
     * 检查用户登录状态
     */
    private void handleCheckUserStatus(int code,String account){
        if (isPageRecycle()){
            return;
        }
        if (code == ResultCode.SUCCESS.getValue()){
            mUiController.get().performUserOnline(account);
            checkConnect();
        }
    }

    /**
     * 处理消息提示
     */
    private void handleToast(String content){
        if (isPageRecycle()){
            return;
        }
        mUiController.get().toastMsg(content);
    }

    /**
     * 检查页面是否存在
     * @return
     */
    private boolean isPageRecycle(){
        return pageActivity.get() == null || mUiController.get() == null;
    }


    /**
     * 回调抽象类的实现
     */
    private class LoginListener extends com.wechat.clibs.LoginListener {

        @Override
        public void onLoginFinish(final int code) {
            mMainHandler.post(new Runnable() {
                @Override
                public void run() {
                    handleLoginResult(code);
                }
            });
        }

        @Override
        public void onSignFinish(final int code) {
            mMainHandler.post(new Runnable() {
                @Override
                public void run() {
                    handleSignResult(code);
                }
            });
        }

        @Override
        public void onLogoutFinish(int code) {
            mMainHandler.post(new Runnable() {
                @Override
                public void run() {
                    handleLogout();
                }
            });
        }

        @Override
        public void onCheckStatusFinish(final int code, final String account) {
            mMainHandler.post(new Runnable() {
                @Override
                public void run() {
                    handleCheckUserStatus(code,account);
                }
            });
        }

        @Override
        public void onDisconnect() {
            mMainHandler.post(new Runnable() {
                @Override
                public void run() {
                    handleDeviceDisconnect();
                }
            });
        }

        @Override
        public void toast(final String content) {
            mMainHandler.post(new Runnable() {
                @Override
                public void run() {
                    handleToast(content);
                }
            });
        }
    }

}
