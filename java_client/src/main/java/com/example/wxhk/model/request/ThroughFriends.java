package com.example.wxhk.model.request;

import com.example.wxhk.infe.SendMsg;
import lombok.Data;
import lombok.experimental.Accessors;

/**
 * 通过好友请求
 * @author wt
 * @date 2023/06/01
 */
@Data
@Accessors(chain = true)
public class ThroughFriends implements SendMsg<ThroughFriends> {
    /**
     * 	添加好友消息内容里的encryptusername
     */
    String v3;
    /**
     * 添加好友消息内容里的ticket
     */
    String v4;
    /**
     * 好友权限,0是无限制,1是不让他看我,2是不看他,3是1+2
     */
    String permission;
}
