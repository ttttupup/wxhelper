package com.example.wxhk.model.request;

import com.example.wxhk.infe.SendMsg;
import lombok.Data;
import lombok.experimental.Accessors;

/**
 *  转发消息
 * @author wt
 * @date 2023/06/01
 */
@Data
@Accessors(chain = true)
public class ForwardMessages implements SendMsg<ForwardMessages> {
    /**
     * 消息接收人wxid
     */
    String wxid;
    /**
     * 消息id
     */
    String msgid;
}
