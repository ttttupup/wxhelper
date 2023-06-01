package com.example.wxhk.model.request;

import com.example.wxhk.infe.SendMsg;
import lombok.Data;
import lombok.experimental.Accessors;

/**
 * 发送图片
 *
 * @author wt
 * @date 2023/06/01
 */
@Data
@Accessors(chain = true)
public class SendImg implements SendMsg<SendImg> {
    String wxid;
    /**
     * 发送图片接口
     * "imagePath": "C:/Users/123.png"
     */
    String imagePath;
}
