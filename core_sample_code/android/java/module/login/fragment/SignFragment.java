package com.wechat.mylogin.module.login.fragment;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;
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

public class SignFragment extends AccountFragment {

    @BindView(R.id.et_login_account)
    public EditText mEtAccount;
    @BindView(R.id.et_login_password)
    public EditText mEtPassword;

    @BindView(R.id.tv_sign)
    public TextView mTvSign;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_sign,container,false);
        ButterKnife.bind(this, view);
        return view;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        Bundle bundle = getArguments();
        updateBundle(bundle);
        initListener();
    }

    private void initListener(){
        judgeBtnCanClick();
        mEtPassword.addTextChangedListener(mTextWatcher);
        mEtAccount.addTextChangedListener(mTextWatcher);
    }

    private TextWatcher mTextWatcher = new TextWatcher() {
        @Override
        public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

        }

        @Override
        public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {

        }

        @Override
        public void afterTextChanged(Editable editable) {
            judgeBtnCanClick();
        }
    };

    private void judgeBtnCanClick(){
        if (TextUtils.isEmpty(mEtPassword.getText().toString()) || TextUtils.isEmpty(mEtAccount.getText().toString())){
            mTvSign.setEnabled(false);
            mTvSign.setSelected(false);
        }else {
            mTvSign.setEnabled(true);
            mTvSign.setSelected(true);
        }
    }

    @OnClick(R.id.tv_sign)
    public void handleLoginIn(View view){
        handleSign(mEtAccount.getText().toString(),mEtPassword.getText().toString());
    }

    @OnClick(R.id.tv_turn_to_login)
    public void handleTurnToLogin(View view){
        Bundle bundle = new Bundle();
        bundle.putString(KEY_ACCOUNT,mEtAccount.getText().toString());

        mEtPassword.setText("");

        handleIntentPage(AccountActivity.TAB_LOGIN,bundle);
    }

    @OnClick(R.id.iv_password_clean)
    public void cleanPwd(){
        mEtPassword.setText("");
    }

    @Override
    public String getAccount(){
        return mEtAccount.getText().toString();
    }

    @Override
    public void updateBundle(Bundle bundle){
        mEtPassword.setText("");
        if (bundle == null){
            mEtAccount.setText("");
            return;
        }
        String account = bundle.getString(KEY_ACCOUNT,"");
        if (TextUtils.isEmpty(account)){
            mEtAccount.setText("");
            return;
        }
        mEtAccount.setText(account);
        mEtAccount.setSelection(account.length());
    }

}
