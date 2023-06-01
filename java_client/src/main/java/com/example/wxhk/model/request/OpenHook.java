package com.example.wxhk.model.request;

import com.example.wxhk.infe.SendMsg;
import lombok.Data;
import lombok.experimental.Accessors;

/**
 * 开启hook
 *
 * @author wt
 * @date 2023/06/01
 */
@Data
@Accessors(chain = true)
public class OpenHook implements SendMsg<OpenHook> {
    String port;
    String ip;
}
