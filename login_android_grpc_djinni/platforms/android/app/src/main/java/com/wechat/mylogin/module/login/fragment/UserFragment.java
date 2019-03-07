package com.wechat.mylogin.module.login.fragment;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.TextView;

import com.wechat.mylogin.R;
import com.wechat.mylogin.module.login.AccountActivity;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

import static com.wechat.mylogin.constant.Constants.KEY_ACCOUNT;

public class UserFragment extends AccountFragment {


    @BindView(R.id.tv_user_account)
    TextView mTvAccount;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_user,container,false);
        ButterKnife.bind(this, view);
        return view;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        updateBundle(savedInstanceState);
    }

    public void updateBundle(Bundle bundle) {
        if (bundle == null) {
            return;
        }
        String account = bundle.getString(KEY_ACCOUNT,"");
        if (TextUtils.isEmpty(account)){
            return;
        }
        if (account.length() == 11){
            account = account.substring(0,3) + " " + account.substring(3,7) + " " + account.substring(7,11);
        }
        mTvAccount.setText(account);
    }

    @OnClick
    void handleLogut(){
        mTvAccount.setText("");
        handleLogout();
    }

}
