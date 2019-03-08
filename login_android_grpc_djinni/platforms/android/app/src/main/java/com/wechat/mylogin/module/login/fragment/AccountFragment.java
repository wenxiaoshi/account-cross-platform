package com.wechat.mylogin.module.login.fragment;

import android.os.Bundle;
import android.support.v4.app.Fragment;

import com.wechat.mylogin.module.login.AccountActivity;

public class AccountFragment extends Fragment {

    /**
     * 切换
     * 账号相关页面
     * @param targetFragment
     * @param bundle
     */
    protected void handleIntentPage(String targetFragment, Bundle bundle){
        if (getActivity() == null){
            return;
        }
        ((AccountActivity)getActivity()).switchTo(targetFragment,bundle);

    }

    public String getAccount(){
        return "";
    }

    public void updateBundle(Bundle bundle){

    }

    public void handleLogout(){
        if (getActivity() == null){
            return;
        }
        ((AccountActivity)getActivity()).handleLogout();
    }

    public void handleLogin(String account,String password){
        if (getActivity() == null){
            return;
        }
        ((AccountActivity)getActivity()).handleLogin(account,password);
    }

    public void handleSign(String account,String password){
        if (getActivity() == null){
            return;
        }
        ((AccountActivity)getActivity()).handleSign(account,password);
    }

}
