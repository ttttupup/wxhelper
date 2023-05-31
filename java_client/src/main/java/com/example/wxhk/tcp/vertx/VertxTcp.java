package com.example.wxhk.tcp.vertx;

import com.example.wxhk.WxhkApplication;
import com.example.wxhk.util.HttpAsyncUtil;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.DeploymentOptions;
import io.vertx.core.Future;
import io.vertx.core.Promise;
import io.vertx.core.json.JsonObject;
import io.vertx.core.net.NetServer;
import io.vertx.core.net.NetServerOptions;
import io.vertx.core.parsetools.JsonParser;
import org.dromara.hutool.log.Log;
import org.springframework.boot.CommandLineRunner;
import org.springframework.core.annotation.Order;
import org.springframework.stereotype.Component;

import java.util.concurrent.LinkedBlockingQueue;

/**
 * 接受微信hook信息
 * @author wt
 * @date 2023/05/26
 */
@Component
@Order()
public class VertxTcp extends AbstractVerticle implements CommandLineRunner {
    protected static final  Log log = Log.get();
     NetServer netServer;
    public final static LinkedBlockingQueue<JsonObject> LINKED_BLOCKING_QUEUE = new LinkedBlockingQueue<>();



    @Override
    public void start(Promise<Void> startPromise) throws Exception {
         netServer = vertx.createNetServer(new NetServerOptions()
                .setPort(InitWeChat.getVertxPort())
                .setIdleTimeout(0)
                .setLogActivity(false)
        );
        netServer.connectHandler(socket -> {
            JsonParser parser = JsonParser.newParser();
            parser.objectValueMode();
            parser.handler(event -> {
                switch (event.type()) {
                    case START_OBJECT -> {
                    }
                    case END_OBJECT -> {
                    }
                    case START_ARRAY -> {
                    }
                    case END_ARRAY -> {
                    }
                    case VALUE -> {
                        LINKED_BLOCKING_QUEUE.add(event.objectValue());
                    }
                }
            });
            socket.handler(parser);
        });

        Future<NetServer> listen = netServer.listen();
        listen.onComplete(event -> {
            boolean succeeded = event.succeeded();
            if (succeeded) {
                HttpAsyncUtil.exec(HttpAsyncUtil.Type.开启hook, new JsonObject().put("port", "8080").put("ip", "127.0.0.1"));
                startPromise.complete();
            }else{
                startPromise.fail(event.cause());
            }

        });
    }

    @Override
    public void run(String... args) throws Exception {
        WxhkApplication.vertx.deployVerticle(this,new DeploymentOptions().setWorkerPoolSize(6));
    }
}
