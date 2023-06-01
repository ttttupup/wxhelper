package com.example.service;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import com.example.client.WeChatHookClient;
import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelOption;
import io.netty.channel.SimpleChannelInboundHandler;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioServerSocketChannel;
import io.netty.handler.codec.DelimiterBasedFrameDecoder;
import io.netty.handler.codec.Delimiters;
import io.netty.handler.codec.string.StringDecoder;
import io.netty.handler.codec.string.StringEncoder;
import io.netty.util.CharsetUtil;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetSocketAddress;

/**
 * @PACKAGE_NAME: com.example.service
 * @NAME: WeChatHookNettyServer
 * @AUTHOR: wxs
 * @DATE: 2023/5/31 15:07
 * @PROJECT_NAME: WeChatHook-java
 **/
public class WeChatHookNettyServer {

    /**
     * 直接启动main方法
     * @param args
     */
    public static void main(String[] args) {
        Integer hookPort = 19099;
        //1、注入
        inject();

        //2、开启hook
        try {
            JSONObject result = WeChatHookClient.hook_msg("127.0.0.1", hookPort.toString());
        } catch (Exception e) {
            System.out.println("hook 失败，请检查微信是否登录");
            return;
        }
        //3、启动服务
        start(hookPort);
    }

    /**
     * 执行注入命令
     */
    public static void inject() {
        ClassLoader classLoader = WeChatHookNettyServer.class.getClassLoader();
        //获取ConsoleInject 文件路径
        String ConsoleInject = classLoader.getResource("ConsoleInject.exe").getPath().replaceFirst("/", "");
        //获取 wxhelper.dll 文件路径
        String wxhelper = classLoader.getResource("wxhelper.dll").getPath().replaceFirst("/", "");

        //ConsoleInject.exe  -i WeChat.exe -p C:\Users\DELL\Desktop\injector\wxhelper.dll
        String command = ConsoleInject + " -i WeChat.exe -p " + wxhelper;

        //重试3次
        int retryCount = 3;
        do {
            retryCount--;
            try {
                //检查登录状态
                JSONObject jsonObject = WeChatHookClient.check_login();
                //如果已登录不需要注入
                if (jsonObject.getInteger("code").equals(1)) {
                    return;
                }
            } catch (Exception e) {
                System.out.println(e.getMessage() + "请确认微信已登录");

            }
            //执行注入命令
            excuteShell(command);

        } while (retryCount > 0);
    }

    /**
     * 启动服务
     *
     * @param port
     */
    public static void start(Integer port) {
        NioEventLoopGroup bossGroup = new NioEventLoopGroup();
        NioEventLoopGroup workerGroup = new NioEventLoopGroup();
        try {
            ServerBootstrap serverBootstrap = new ServerBootstrap();
            serverBootstrap.group(bossGroup, workerGroup)
                    .channel(NioServerSocketChannel.class)
                    .localAddress(new InetSocketAddress(port))
                    .childHandler(new ChannelInitializer<SocketChannel>() {
                        @Override
                        protected void initChannel(SocketChannel ch) {
                            ch.pipeline().addLast(new DelimiterBasedFrameDecoder(8192, Delimiters.lineDelimiter()));
                            ch.pipeline().addLast(new StringDecoder(CharsetUtil.UTF_8));
                            ch.pipeline().addLast(new StringEncoder(CharsetUtil.UTF_8));
                            ch.pipeline().addLast(new ReceiveMsgHandler());
                        }
                    })
                    .option(ChannelOption.SO_BACKLOG, 128)
                    .childOption(ChannelOption.SO_KEEPALIVE, true);

            Channel channel = serverBootstrap.bind().sync().channel();
            System.out.println("服务启动成功 端口号 " + port);
            channel.closeFuture().sync();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            bossGroup.shutdownGracefully();
            workerGroup.shutdownGracefully();
        }
    }

    private static class ReceiveMsgHandler extends SimpleChannelInboundHandler<String> {

        @Override
        protected void channelRead0(ChannelHandlerContext ctx, String msg) {
            JSON.parseObject(msg).forEach((k, v) -> {
                System.out.println(k + " = " + v);
            });
            System.out.println("----------end----------");
        }

        @Override
        public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) {
            cause.printStackTrace();
            ctx.close();
        }

    }

    /**
     * 执行shell 命令
     *
     * @param command
     */
    public static void excuteShell(String command) {
        try {
            Process process = Runtime.getRuntime().exec(command);

            BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String line;
            while ((line = reader.readLine()) != null) {
                System.out.println(line);
            }
            int exitCode = process.waitFor();
            System.out.println("Exit Code: " + exitCode);
        } catch (IOException | InterruptedException e) {
            e.printStackTrace();
        }
    }

}
