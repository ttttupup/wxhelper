package com.example.wxhk.model.request;

import lombok.Data;
import lombok.experimental.Accessors;

/**
 * http请求参数
 *
 * @author wt
 * @date 2023/06/01
 */
@Data
@Accessors(chain = true)
public class SendMsg {
    /**
     * wxid
     */
    String wxid;
    /**
     * 消息内容
     */
    String msg;

    /**
     * 聊天室id,群聊用
     */
    String chatRoomId;
    /**
     * 成员id
     */
    String memberId;

    /**
     * 群聊的时候用at多个用逗号隔开,@所有人则是<b>notify@all</b>
     */
    String wxids;
    /**
     * 发送图片接口
     * "imagePath": "C:/Users/123.png"
     */
    String imagePath;
    /**
     * 发送文件路径
     * "filePath": "C:/Users/123.txt"
     */
    String filePath;
    /**
     * 通过	手机或qq查询信息
     */
    String keyword;

}
