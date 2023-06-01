package com.example.wxhk.model.request;

import com.example.wxhk.infe.SendMsg;
import lombok.Data;
import lombok.experimental.Accessors;

/**
 * 获取群成员
 * @author wt
 * @date 2023/06/01
 */
@Data
@Accessors(chain = true)
public class GetGroupMembers implements SendMsg<GetGroupMembers> {
    String chatRoomId;
}
