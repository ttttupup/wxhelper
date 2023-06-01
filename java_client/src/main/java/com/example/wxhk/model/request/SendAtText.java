package com.example.wxhk.model.request;

import com.example.wxhk.infe.SendMsg;
import lombok.Data;
import lombok.experimental.Accessors;

/**
 * 发送at文本
 * @author wt
 * @date 2023/06/01
 */
@Data
@Accessors(chain = true)
public class SendAtText implements SendMsg<SendAtText> {
    /**
     * 聊天室id,群聊用
     */
    String chatRoomId;
    /**
     * 群聊的时候用at多个用逗号隔开,@所有人则是<b>notify@all</b>
     */
    String wxids;

    String msg;
}
