package com.example.wxhk.model.dto;

import java.math.BigDecimal;

/**
 * 支付信息
 *
 * @author wt
 * @param receiverUsername 付款人
 * @param decimal 收款金额
 * @param remark 备注
 * @param transcationid
 * @param transferid
 * @date 2023/06/06
 */
public record PayoutInformation(String receiverUsername, BigDecimal decimal, String remark,String transcationid,String transferid) implements java.io.Serializable {

    public PayoutInformation(String receiverUsername, BigDecimal decimal, String remark) {
        this(receiverUsername, decimal, remark, null, null);
    }
}