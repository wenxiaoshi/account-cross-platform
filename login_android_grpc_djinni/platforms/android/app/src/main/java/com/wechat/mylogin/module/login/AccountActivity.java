package com.wechat.mylogin.module.login;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.text.TextUtils;
import android.widget.Toast;

import com.wechat.mylogin.R;
import com.wechat.mylogin.module.login.fragment.AccountFragment;
import com.wechat.mylogin.module.login.fragment.LoginFragment;
import com.wechat.mylogin.module.login.fragment.SignFragment;
import com.wechat.mylogin.module.login.fragment.UserFragment;

import static com.wechat.mylogin.constant.Constants.KEY_ACCOUNT;

public class AccountActivity extends AppCompatActivity implements LoginUIController {

    //登录界面
    public static final String TAB_LOGIN = LoginFragment.class.getName();
    //注册界面
    public static final String TAB_SIGN = SignFragment.class.getName();
    //用户界面
    public static final String TAB_USER = UserFragment.class.getName();

    static {
        System.loadLibrary("mylogin");
    }

    private ILoginController mController;

    private AccountFragment mLastShowFragment;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_account);

        mController = new LoginController(this,this);

        switchTo(TAB_LOGIN, null);

    }

    /**
     * 页面切换
     */
    public void switchTo(String name, Bundle args) {
        if (TextUtils.isEmpty(name)) {
            return;
        }

        FragmentManager fm = getSupportFragmentManager();
        FragmentTransaction ft = fm.beginTransaction();

        AccountFragment fragment = (AccountFragment) fm.findFragmentByTag(name);//mFragments.get(name);
        if (fragment == null) {
            fragment = (AccountFragment) Fragment.instantiate(this, name);
            fragment.setArguments(args);
            ft.add(R.id.fl_container, fragment, name);
        } else {
            fragment.updateBundle(args);
            ft.show(fragment);
        }

        if (mLastShowFragment != null) {
            ft.hide(mLastShowFragment);
        }

        mLastShowFragment = fragment;
        ft.commitAllowingStateLoss();

    }

    /**
     * 登录成功，显示处理
     */
    @Override
    public void performLoginSuccess() {
        String account = mLastShowFragment.getAccount();
        Bundle bundle = new Bundle();
        bundle.putString(KEY_ACCOUNT,account);
        switchTo(TAB_USER,bundle);
    }

    /**
     * 注册成功，显示处理
     */
    @Override
    public void performSignSuccess() {
        String account = mLastShowFragment.getAccount();
        Bundle bundle = new Bundle();
        bundle.putString(KEY_ACCOUNT,account);
        switchTo(TAB_USER,bundle);
    }

    /**
     * 登录失败
     */
    @Override
    public void performLoginFail() {
    }

    /**
     * 注册失败
     */
    @Override
    public void performSignFail() {
    }

    /**
     * 退出登录，处理界面
     */
    @Override
    public void performLogout() {
        switchTo(TAB_LOGIN,null);
    }

    /**
     * 处理用户已在线状态
     */
    @Override
    public void performUserOnline(String account) {
        Bundle bundle = new Bundle();
        bundle.putString(KEY_ACCOUNT,account);
        switchTo(TAB_USER,bundle);
    }

    /**
     * 弹窗提醒
     */
    public void toastMsg(String content) {
        Toast.makeText(this,content,Toast.LENGTH_LONG).show();
    }

    /**
     * 被踢下线
     */
    @Override
    public void disconnect() {
        switchTo(TAB_LOGIN,null);
    }

    /**
     * 点击事件
     * 用户登录
     */
    public void handleLogin(String account, String password){
        mController.actionLoginIn(account,password);
    }

    /**
     * 点击事件
     * 用户注册
     */
    public void handleSign(String account, String password){
        mController.actionSignIn(account,password);
    }

    /**
     * 点击事件
     * 退出登录
     */
    public void handleLogout(){
        mController.actionLogout();
    }

}
