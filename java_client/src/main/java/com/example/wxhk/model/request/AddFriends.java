package com.example.wxhk.model.request;

import com.example.wxhk.infe.SendMsg;
import lombok.Data;
import lombok.experimental.Accessors;

/**
 * 添加wxid 好友
 * @author wt
 * @date 2023/06/01
 */
@Data
@Accessors(chain = true)
public class AddFriends implements SendMsg<AddFriends> {
    String wxid;
    /**
     * 验证信息
     */
    String msg;
}
