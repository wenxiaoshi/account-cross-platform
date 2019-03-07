package com.wechat.mylogin.module.login;


import android.os.Handler;
import android.os.Looper;
import android.text.TextUtils;

import com.wechat.clibs.ActionResult;
import com.wechat.clibs.LoginCore;
import com.wechat.mylogin.constant.ResultCode;

import java.lang.ref.WeakReference;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;


public class LoginController implements ILoginController {

    private WeakReference<AccountActivity> pageActivity;
    private WeakReference<LoginUIController> mUiController;
    private LoginCore mLoginCore;
    private LoginListener mLoginListener;

    private ExecutorService executorCache = Executors.newCachedThreadPool();
    private ExecutorService executorSingle = Executors.newSingleThreadExecutor();

    private Handler mMainHandler = new Handler(Looper.getMainLooper());

    public LoginController(AccountActivity activity, LoginUIController controller){
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
        executorSingle.execute(new Runnable() {
            @Override
            public void run() {
                mLoginCore.checkConnection();
            }
        });
    }

    /**
     * 处理登录完成后的UI方法调用
     * @param result
     */
    private void handleLoginResult(ActionResult result){
        if (isPageRecycle()){
            return;
        }
        if (result.getCode() == ResultCode.SUCCESS.getValue()){
            mUiController.get().performLoginSuccess();
            checkConnect();
        }else {
            mUiController.get().performLoginFail();
        }
        if (!TextUtils.isEmpty(result.getMsg())){
            handleToast(result.getMsg());
        }
    }

    /**
     * 处理注册完成后的UI方法调用
     * @param result
     */
    private void handleSignResult(ActionResult result){
        if (isPageRecycle()){
            return;
        }
        if (result.getCode() == ResultCode.SUCCESS.getValue()){
            mUiController.get().performSignSuccess();
            checkConnect();
        }else {
            mUiController.get().performSignFail();
        }
        if (!TextUtils.isEmpty(result.getMsg())){
            handleToast(result.getMsg());
        }
    }

    /**
     * 处理断开登录状态后的UI方法调用
     */
    private void handleDeviceDisconnect(ActionResult result){
        if (isPageRecycle()){
            return;
        }
        mUiController.get().disconnect();
        if (!TextUtils.isEmpty(result.getMsg())){
            handleToast(result.getMsg());
        }
    }

    /**
     * 处理退出登录状态后的UI方法调用
     */
    private void handleLogout(ActionResult result){
        if (isPageRecycle()){
            return;
        }
        mUiController.get().performLogout();
        if (!TextUtils.isEmpty(result.getMsg())){
            handleToast(result.getMsg());
        }
    }

    /**
     * 检查用户登录状态
     */
    private void handleCheckUserStatus(ActionResult result){
        if (isPageRecycle()){
            return;
        }
        if (result.getCode() == ResultCode.SUCCESS.getValue()){
            mUiController.get().performUserOnline(result.getData());
            checkConnect();
        }
        if (!TextUtils.isEmpty(result.getMsg())){
            handleToast(result.getMsg());
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
        public void onLoginFinish(final ActionResult result) {
            mMainHandler.post(new Runnable() {
                @Override
                public void run() {
                    handleLoginResult(result);
                }
            });
        }

        @Override
        public void onSignFinish(final ActionResult result) {
            mMainHandler.post(new Runnable() {
                @Override
                public void run() {
                    handleSignResult(result);
                }
            });
        }

        @Override
        public void onLogoutFinish(final ActionResult result) {
            mMainHandler.post(new Runnable() {
                @Override
                public void run() {
                    handleLogout(result);
                }
            });
        }

        @Override
        public void onCheckStatusFinish(final ActionResult result) {
            mMainHandler.post(new Runnable() {
                @Override
                public void run() {
                    handleCheckUserStatus(result);
                }
            });
        }

        @Override
        public void onDisconnect(final ActionResult result) {
            mMainHandler.post(new Runnable() {
                @Override
                public void run() {
                    handleDeviceDisconnect(result);
                }
            });
        }
        
    }

}
