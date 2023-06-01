package com.example.wxhk.model.request;

import com.example.wxhk.infe.SendMsg;
import lombok.Data;
import lombok.experimental.Accessors;

/**
 *  增加群成员
 * @author wt
 * @date 2023/06/01
 */
@Data
@Accessors(chain = true)
public class IncreaseGroupMembership implements SendMsg<IncreaseGroupMembership> {
    /**
     * 聊天室id
     */
    String chatRoomId;
    /**
     * 成员id，以,分割
     */
    String memberIds;
}
