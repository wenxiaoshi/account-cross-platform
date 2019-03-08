package com.wechat.mylogin.widgets;

import android.app.Dialog;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.LinearLayout;

import com.wechat.mylogin.R;

public class ProgressDialog extends Dialog {

    private View contentView;

    public ProgressDialog(Context context){
        super(context);
        initView();
    }

    private void initView(){
        Window window = getWindow();
        if (window != null){
            window.getDecorView().setBackgroundColor(getContext().getResources().getColor(R.color.transparent));
        }

        setCanceledOnTouchOutside(false);

        contentView = LayoutInflater.from(getContext()).inflate(R.layout.layout_progressbar, null);
        setContentView(contentView);



    }

}
