package com.example.wxhk.model.request;

import com.example.wxhk.infe.SendMsg;
import lombok.Data;
import lombok.experimental.Accessors;

/**
 * 通过手机或者qq查找微信
 * @author wt
 * @date 2023/06/01
 */
@Data
@Accessors(chain = true)
public class FindWeChat implements SendMsg<FindWeChat> {
    /**
     * 通过	手机或qq查询信息
     */
    String keyword;
}
