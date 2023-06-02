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
 *
 * @author wt
 * @date 2023/05/31
 */
@Component
public class ArrHandle {

    /**
     * 线程处理消息队列,但是必须保证核心数大于2,其中必定要有一个线程可以单独处理交易队列信息
     */
    public static final ThreadPoolExecutor sub = new ThreadPoolExecutor(4, 10, 30, TimeUnit.MINUTES, new LinkedBlockingQueue<>(), new NamedThreadFactory("sub", false));
    public static final ThreadLocal<PrivateChatMsg> chatMsgThreadLocal = new InheritableThreadLocal<>();
    protected static final Log log = Log.get();

    /**
     * 得到当前正在处理的消息
     *
     * @return {@link PrivateChatMsg}
     */
    public static PrivateChatMsg getPriMsg() {
        return chatMsgThreadLocal.get();
    }

    @PostConstruct
    public void exec() {
        for (int i = 0; i < sub.getCorePoolSize()-1; i++) {
            sub.submit(() -> {
                while (!Thread.currentThread().isInterrupted()) {
                    try {
                        JsonObject take = VertxTcp.LINKED_BLOCKING_QUEUE.take();
                        log.info("{}", take.encode());
                        PrivateChatMsg privateChatMsg = take.mapTo(PrivateChatMsg.class);
                        chatMsgThreadLocal.set(privateChatMsg);
                        if ("weixin".equals(privateChatMsg.getFromUser())) {
                            String s = HttpSendUtil.获取当前登陆微信id();
                            InitWeChat.WXID_MAP.add(s);
                            continue;
                        }
                        WxMsgHandle.exec(privateChatMsg);
                    } catch (Exception e) {
                        log.error(e);
                    }finally {
                        chatMsgThreadLocal.remove();
                    }
                }
                log.error("退出线程了");
            });
        }
        sub.submit(() -> {
            while (!Thread.currentThread().isInterrupted()) {
                try {
                    JsonObject take = VertxTcp.LINKED_BLOCKING_QUEUE_MON.take();
                    log.info("{}", take.encode());
                    PrivateChatMsg privateChatMsg = take.mapTo(PrivateChatMsg.class);
                    chatMsgThreadLocal.set(privateChatMsg);
                    if ("weixin".equals(privateChatMsg.getFromUser())) {
                        String s = HttpSendUtil.获取当前登陆微信id();
                        InitWeChat.WXID_MAP.add(s);
                        continue;
                    }
                    WxMsgHandle.exec(privateChatMsg);
                } catch (Exception e) {
                    log.error(e);
                }finally {
                    chatMsgThreadLocal.remove();
                }
            }
            log.error("退出线程了");
        });

    }

}
