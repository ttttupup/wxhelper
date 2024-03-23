package com.example.wxhk.util;

import com.example.wxhk.tcp.vertx.InitWeChat;
import io.vertx.core.json.JsonObject;
import org.dromara.hutool.http.client.ClientConfig;
import org.dromara.hutool.http.client.Request;
import org.dromara.hutool.http.client.engine.ClientEngine;
import org.dromara.hutool.http.client.engine.ClientEngineFactory;
import org.dromara.hutool.http.meta.Method;
import org.dromara.hutool.log.Log;

/**
 * http同步请求
 *
 * @author wt
 * @date 2023/05/25
 */
public class HttpSyncUtil {
    protected static final Log log = Log.get();
    static final ClientEngine engine;

    static {
        ClientConfig clientConfig = ClientConfig.of()
                .setTimeout(30 * 1000);
        engine = ClientEngineFactory.createEngine(clientConfig);

    }

    public static JsonObject exec(HttpAsyncUtil.Type type, JsonObject obj) {
        String post = engine.send(Request.of("http://localhost:" + InitWeChat.wxPort + "/api/" + type.getType()).method(Method.POST).body(obj.encode())).bodyStr();
        if (log.isDebugEnabled()) {
            log.debug("type:{},{}", type.getType(), post);
        }
        return new JsonObject(post);
    }
}
