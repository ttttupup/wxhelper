package com.example.wxhk.constant;

/**
 * 接受到的微信消息类型
 *
 * @author wt
 * @date 2023/05/26
 */
public enum WxMsgType {

    /**
     *
     */
    私聊信息(1),
    好友请求(37),
    收到名片(42),
    表情(47),
    转账和收款(49),
    收到转账之后或者文件助手等信息(51),

    入群(10000),
    /**
     * 扫码触发,会触发2次, 有一次有编号,一次没有,还有登陆之后也有,很多情况都会调用这个
     */
    扫码触发(10002),

    ;
    Integer type;

    WxMsgType(Integer type) {
        this.type = type;
    }

    public Integer getType() {
        return type;
    }
}
