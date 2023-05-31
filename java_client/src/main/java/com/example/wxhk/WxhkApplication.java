package com.example.wxhk;

import io.vertx.core.Vertx;
import io.vertx.core.VertxOptions;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class WxhkApplication {
    public static final Vertx vertx;

    static {
        vertx = Vertx.vertx(new VertxOptions().setWorkerPoolSize(5).setEventLoopPoolSize(5));
    }

    //ConsoleInject.exe -i WeChat.exe -p D:\wxhelper.dll
    //ConsoleApplication.exe -I 4568 -p C:\wxhelper.dll -m 17484 -P 1888
    public static void main(String[] args) {
        SpringApplication.run(WxhkApplication.class, args);
    }

}
