package com.example.wxhk.model.request;

import com.example.wxhk.infe.SendMsg;
import lombok.Data;
import lombok.experimental.Accessors;

/**
 * 发送文件
 *
 * @author wt
 * @date 2023/06/01
 */
@Data
@Accessors(chain = true)
public class SendFile implements SendMsg<SendFile> {
    String wxid;
    /**
     * 发送文件路径
     * "filePath": "C:/Users/123.txt"
     */
    String filePath;
}
