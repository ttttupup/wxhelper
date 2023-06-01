package com.example.wxhk.model.request;

import com.example.wxhk.infe.SendMsg;
import lombok.Data;
import lombok.experimental.Accessors;

/**
 * 发送文本
 *
 * @author wt
 * @date 2023/06/01
 */
@Data
@Accessors(chain = true)
public class SendText implements SendMsg<SendText> {
    String wxid;
    String msg;
}
