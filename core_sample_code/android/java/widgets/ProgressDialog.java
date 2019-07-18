package com.wechat.mylogin.widgets;

import android.app.Dialog;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;

import com.wechat.mylogin.R;

public class ProgressDialog extends Dialog {

    public ProgressDialog(Context context){
        super(context);
        init();
    }

    private void init(){
        View view = LayoutInflater.from(getContext()).inflate(R.layout.layout_progressbar,null);
        setContentView(view);

        Window window = getWindow();
        if (window != null){
            window.setBackgroundDrawableResource(R.drawable.bg_corner_tran);
        }

        setCancelable(false);
        setCanceledOnTouchOutside(false);
    }

}
