package com.wechat.mylogin.module;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.wechat.mylogin.R;
import com.wechat.mylogin.utils.CommonUtils;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;
public class LoginActivity extends AppCompatActivity implements LoginUIController {

    static {
        System.loadLibrary("mylogin");
    }

    private ILoginController mController;

    @BindView(R.id.et_login_account)
    public EditText mEtAccount;
    @BindView(R.id.et_login_password)
    public EditText mEtPassword;

    @BindView(R.id.ll_login_operate)
    public View mVOperation;

    @BindView(R.id.ll_login_result)
    public View mVResult;
    @BindView(R.id.tv_result_title)
    public TextView mTvResultTitle;
    @BindView(R.id.tv_result_account)
    public TextView mTvResultAccount;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        ButterKnife.bind(this);

        mController = new LoginController(this,this);

    }

    /**
     * 登录成功，显示处理
     */
    @Override
    public void performLoginSuccess() {
        showResultView(getString(R.string.title_login_success),mEtAccount.getText().toString());
    }

    /**
     * 注册成功，显示处理
     */
    @Override
    public void performSignSuccess() {
        showResultView(getString(R.string.title_sign_success),mEtAccount.getText().toString());
    }

    /**
     * 登录失败，弹窗提示
     */
    @Override
    public void performLoginFail() {
        CommonUtils.showToast(this,R.string.login_action_result_fail,Toast.LENGTH_LONG);
    }

    /**
     * 注册失败，弹窗提示
     */
    @Override
    public void performSignFail() {
        CommonUtils.showToast(this,R.string.sign_action_result_fail,Toast.LENGTH_LONG);
    }

    /**
     * 退出登录，处理界面
     */
    @Override
    public void performLogout() {
        showOperationView();
        CommonUtils.showToast(this,R.string.exit_login_finish,Toast.LENGTH_LONG);

    }

    @Override
    public void performUserOnline(String account) {
        showResultView(getString(R.string.title_login_already),account);
    }

    @Override
    public void toastMsg(String content) {
        Toast.makeText(this,content,Toast.LENGTH_LONG).show();
    }

    /**
     * 被踢下线，弹窗提示
     */
    @Override
    public void disconnect() {
        showOperationView();
        CommonUtils.showToast(this,R.string.login_disconnect,Toast.LENGTH_LONG);
    }

    private void showOperationView(){
        mVOperation.setVisibility(View.VISIBLE);
        mVResult.setVisibility(View.GONE);
        mEtAccount.setText("");
        mEtPassword.setText("");
    }

    private void showResultView(String title,String account){
        mVOperation.setVisibility(View.GONE);
        mVResult.setVisibility(View.VISIBLE);
        mTvResultTitle.setText(title);
        mTvResultAccount.setText(account);
    }

    @OnClick(R.id.btn_exit_login)
    public void exitLogin(){
        mController.actionLogout();
    }

    @OnClick(R.id.bt_login)
    public void handleLoginIn(View view){
        mController.actionLoginIn(mEtAccount.getText().toString(),mEtPassword.getText().toString());
    }

    @OnClick(R.id.bt_sign)
    public void handleSignIn(View view){
        mController.actionSignIn(mEtAccount.getText().toString(),mEtPassword.getText().toString());
    }


}
