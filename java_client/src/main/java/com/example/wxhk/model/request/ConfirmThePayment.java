package com.example.wxhk.model.request;

import com.example.wxhk.infe.SendMsg;
import lombok.Data;
import lombok.experimental.Accessors;

/**
 * 确认收款
 * @author wt
 * @date 2023/06/01
 */
@Data
@Accessors(chain = true)
public class ConfirmThePayment implements SendMsg<ConfirmThePayment> {
    /**
     * 转账人微信id，从hook的消息中获取
     */
    String wxid;
    /**
     * 从hook的消息中获取对应的字段内容
     */
    String transcationId;
    /**
     * 从hook的消息中获取对应的字段内容。
     */
    String transferId;
}
