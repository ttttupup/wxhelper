package com.example.wxhk.tcp.vertx;

import com.example.wxhk.model.PrivateChatMsg;
import com.example.wxhk.msg.WxMsgHandle;
import com.example.wxhk.util.HttpSendUtil;
import io.vertx.core.json.JsonObject;
import jakarta.annotation.PostConstruct;
import org.dromara.hutool.core.thread.NamedThreadFactory;
import org.dromara.hutool.log.Log;
import org.springframework.stereotype.Component;

import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

/**
 * 消息处理
 * @author wt
 * @date 2023/05/31
 */
@Component
public class ArrHandle {

    protected static final  Log log = Log.get();
        public static final ThreadPoolExecutor sub = new ThreadPoolExecutor(1, 10, 30, TimeUnit.MINUTES, new LinkedBlockingQueue<>(), new NamedThreadFactory("sub", false));


        @PostConstruct
        public void exec(){
            for (int i = 0; i < sub.getCorePoolSize(); i++) {
                sub.submit(() -> {
                    while (!Thread.currentThread().isInterrupted()){
                        try {
                            JsonObject take = VertxTcp.LINKED_BLOCKING_QUEUE.take();
                            log.info("{}",take.encode());

                            PrivateChatMsg privateChatMsg = take.mapTo(PrivateChatMsg.class);
                            if("weixin".equals(privateChatMsg.getFromUser())){
                                String s = HttpSendUtil.获取当前登陆微信id();
                                InitWeChat.WXID_MAP.add(s);
                                continue;
                            }
                            WxMsgHandle.exec(privateChatMsg);
                        } catch (Exception e) {
                           log.error(e);
                        }
                    }
                    log.error("退出线程了");
                });
            }

        }

}
