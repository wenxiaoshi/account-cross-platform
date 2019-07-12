package com.wechat.mylogin.constant;

public enum  ResultCode {

    SUCCESS(0),
    FAIL(-1),
    NO_FOUND(404),
    SERVER_ERROR_500(500),

    REQ_PEM_IS_NULL(1010);

    private int value;

    ResultCode(int type) {
        this.value = type;
    }

    public int getValue() {
        return this.value;
    }

}
