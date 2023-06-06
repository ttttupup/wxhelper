package com.example.wxhk.server;

import com.example.wxhk.model.PrivateChatMsg;
import com.example.wxhk.model.dto.PayoutInformation;

/**
 * 微信消息处理提取
 * @author wt
 * @date 2023/06/06
 */
public interface WxSmgServer {
    /**
     * 接到收款
     *
     * @param payoutInformation 支付信息
     */
    void 接到收款(PayoutInformation payoutInformation);

    void 收款之后(PayoutInformation pay);

    void 私聊(PrivateChatMsg chatMsg);

    void 文件助手(PrivateChatMsg chatMsg);

    void 收到名片(PrivateChatMsg chatMsg);

    void 收到好友请求(PrivateChatMsg chatMsg);

    void 扫码收款(PayoutInformation payoutInformation);
}
