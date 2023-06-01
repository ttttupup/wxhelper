package com.example.wxhk.tcp;

import com.example.wxhk.util.HttpAsyncUtil;
import io.vertx.core.Future;
import io.vertx.core.buffer.Buffer;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.client.HttpResponse;
import org.dromara.hutool.core.lang.Console;
import org.dromara.hutool.core.thread.ThreadUtil;
import org.dromara.hutool.log.Log;
import org.junit.jupiter.api.Test;
import org.springframework.boot.test.context.SpringBootTest;

@SpringBootTest
class HttpAsyncUtilTest {

    protected static final Log log = Log.get();


    @Test
    void exec() {
        Future<HttpResponse<Buffer>> exec = HttpAsyncUtil.exec(HttpAsyncUtil.Type.联系人列表, new JsonObject());
        exec.onSuccess(event -> {
           Console.log(event.bodyAsJsonObject());
        });
    }
    @Test
    void exec1() {

        for(int i=0;i<10000;i++){
            int finalI = i;
             HttpAsyncUtil.exec(HttpAsyncUtil.Type.获取登录信息, new JsonObject(), event -> {
                if (event.succeeded()) {
                    log.info("i:{},{}", finalI,event.result().bodyAsJsonObject());
                }else{
                    event.cause().printStackTrace();
                }
            });
            log.info("发出请求:{}",i);
        }

        ThreadUtil.sync(this);
    }
    @Test
    void exec2() {

    }
}