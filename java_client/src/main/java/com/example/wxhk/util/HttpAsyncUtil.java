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
        return client.post(InitWeChat.wxPort, "localhost", "/api/?type=" + type.getType())
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
        return client.post(InitWeChat.wxPort, "localhost", "/api/?type=" + type.getType())
                .sendJsonObject(object)
                .onComplete(handler)
                ;


    }

    public enum Type {
        检查微信登陆("0"),
        获取登录信息("1"),
        发送文本("2"),
        发送at文本("3"),
        发送图片("5"),
        发送文件("6"),
        开启hook("9"),
        关闭hook("10"),
        添加好友("20"),
        通过好友申请("23"),
        获取群成员("25"),
        获取群成员昵称("26"),
        删除群成员("27"),
        确认收款("45"),
        联系人列表("46"),
        查询微信信息("55"),


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
