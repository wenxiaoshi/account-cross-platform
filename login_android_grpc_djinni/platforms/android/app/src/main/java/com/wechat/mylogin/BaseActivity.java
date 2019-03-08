package com.wechat.mylogin;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

import com.wechat.mylogin.widgets.ProgressDialog;

public class BaseActivity extends AppCompatActivity {


    private ProgressDialog progressDialog;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    protected void showProgress(){
        if (progressDialog == null){
            progressDialog = new ProgressDialog(this);
        }
        progressDialog.show();
    }

    protected void hideProgress(){
        if (progressDialog == null){
            return;
        }
        progressDialog.cancel();
    }

}
