package com.example.wxhk.model;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import lombok.Data;
import lombok.experimental.Accessors;

import java.io.Serializable;

/**
 * 私聊
 *
 * @author wt
 * @date 2023/05/26
 */
@Data
@Accessors(chain = true)
@JsonIgnoreProperties(ignoreUnknown = true)
public class PrivateChatMsg implements Serializable {

    String path;
    /**
     * 内容
     */

    private String content;
    /**
     * 当是群聊的时候 为群id,否则为微信id
     */
    private String fromGroup;
    /**
     * 微信id
     */
    private String fromUser;
    private Integer isSendMsg;
    /**
     * 1通过手机发送
     */
    private Integer isSendByPhone;
    private Long msgId;
    private Integer pid;
    private String sign;
    private String signature;
    private String time;
    private Integer timestamp;

    /**
     * 对用户,如果是文件助手是filehelper
     */
    private String toUser;
    /**
     * 类型
     */
    private Integer type;
}
