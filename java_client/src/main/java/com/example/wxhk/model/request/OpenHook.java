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
    /**
     * 0/1 ：1.启用http 0.不启用http
     */
    boolean enableHttp;
    /**
     * 超时时间,单位ms
     */
    String timeout;

    /**
     * http的请求地址，enableHttp=1时，不能为空
     */
    String url;
}
