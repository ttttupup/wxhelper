package com.example.wxhk.util;

import com.example.wxhk.WxhkApplication;
import com.example.wxhk.tcp.vertx.InitWeChat;
import io.vertx.core.AsyncResult;
import io.vertx.core.Future;
import io.vertx.core.Handler;
import io.vertx.core.buffer.Buffer;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.client.HttpResponse;
import io.vertx.ext.web.client.WebClient;
import io.vertx.ext.web.client.WebClientOptions;
import org.dromara.hutool.log.Log;

/**
 * http异步请求
 *
 * @author wt
 * @date 2023/05/25
 */
public class HttpAsyncUtil {
    public static final WebClient client = WebClient.create(WxhkApplication.vertx, new WebClientOptions().setDefaultHost("localhost").setDefaultPort(InitWeChat.wxPort)
            .setConnectTimeout(10000).setMaxPoolSize(10).setPoolEventLoopSize(10));
    protected static final Log log = Log.get();

    public static Future<HttpResponse<Buffer>> exec(Type type, JsonObject object) {
        return client.post(InitWeChat.wxPort, "localhost", "/api/" + type.getType())
                .sendJsonObject(object)
                .onSuccess(event ->
                        {
                            if (log.isDebugEnabled()) {
                                log.debug("type:{},{}", type.getType(), event.bodyAsJsonObject());
                            }
                        }
                );
    }

    public static Future<HttpResponse<Buffer>> exec(Type type, JsonObject object, Handler<AsyncResult<HttpResponse<Buffer>>> handler) {
        return client.post(InitWeChat.wxPort, "localhost", "/api/" + type.getType())
                .sendJsonObject(object)
                .onComplete(handler)
                ;


    }

    public enum Type {
        检查微信登陆("checkLogin"),
        获取登录信息("userInfo"),
        发送文本("sendTextMsg"),
        转发消息("forwardMsg"),
        发送at文本("sendAtText"),
        发送图片("5"),
        发送文件("sendFileMsg"),
        开启hook("hookSyncMsg"),
        关闭hook("unhookSyncMsg"),
        添加好友("20"),
        通过好友申请("23"),
        获取群成员("getMemberFromChatRoom"),
        获取群成员基础信息("getContactProfile"),
        获取群详情("getChatRoomDetailInfo"),
        添加群成员("addMemberToChatRoom"),
        修改群昵称("modifyNickname"),
        删除群成员("delMemberFromChatRoom"),
        置顶群消息("topMsg"),
        取消置顶群消息("removeTopMsg"),
        邀请入群("InviteMemberToChatRoom"),
        确认收款("45"),
        联系人列表("getContactList"),
        查询微信信息("55"),
        下载附件("downloadAttach"),
        解码("decodeImage"),


        ;
        String type;

        Type(String type) {
            this.type = type;
        }

        public String getType() {
            return type;
        }
    }
}
