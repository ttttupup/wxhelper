package com.example.wxhk.model.request;

import com.example.wxhk.infe.SendMsg;
import lombok.Data;
import lombok.experimental.Accessors;

/**
 * 获取群成员昵称
 * @author wt
 * @date 2023/06/01
 */
@Data
@Accessors(chain = true)
public class GetsTheNicknameOfAGroupMember implements SendMsg<GetsTheNicknameOfAGroupMember> {
    /**
     * 聊天室id
     */
    String chatRoomId;
    /**
     * 成员id
     */
    String memberId;
}
