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
import io.netty.util.internal.StringUtil;

import java.io.*;
import java.net.InetSocketAddress;
import java.util.*;

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
     *
     * @param args
     */
    public static void main(String[] args) {

        Integer serverPort = 19077;

        String hookApi = null;

        for (String arg : args) {
            System.out.println(arg);
            if (arg.startsWith("--port")) {
                serverPort = Integer.valueOf(arg.split("=")[1]);
            }
            if (arg.startsWith("--hookApi")) {
                hookApi = arg.split("=")[1];
            }
        }

        //1、注入
        inject();

        //2、开启hook
        try {
            JSONObject result = WeChatHookClient.hook_msg("127.0.0.1", serverPort.toString());
        } catch (Exception e) {
            System.out.println("hook 失败，请检查微信是否登录");
            return;
        }
        //3、启动服务
        start(serverPort, hookApi);
    }

    /**
     * 执行注入命令
     */
    public static void inject() {
        //
        File consoleInjectTemp = null;
        File wxhelperTemp = null;
        try {
            consoleInjectTemp = createTempFile("ConsoleInject", ".exe");
            wxhelperTemp = createTempFile("wxhelper", ".dll");

            String ConsoleInject = consoleInjectTemp.getAbsolutePath();

            String wxhelper = wxhelperTemp.getAbsolutePath();

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

            } while (retryCount >= 0);

        } catch (Exception e) {
            e.printStackTrace();
            return;
        } finally {
            ////如果不为空删除临时文件
            if (Objects.nonNull(consoleInjectTemp)) {
                consoleInjectTemp.delete();
            }
            //如果不为空删除临时文件
            if (Objects.nonNull(wxhelperTemp)) {
                wxhelperTemp.delete();
            }
        }

    }

    /**
     * 创建临时文件
     *
     * @param fileName
     * @param suffix
     * @return
     */
    private static File createTempFile(String fileName, String suffix) {
        InputStream inputStream = WeChatHookNettyServer.class.getResourceAsStream("/" + fileName + suffix);

        FileOutputStream outputStream = null;
        try {
            File tempFile = File.createTempFile(fileName, suffix);
            outputStream = new FileOutputStream(tempFile);
            byte[] buffer = new byte[4096];
            int bytesRead;
            while ((bytesRead = inputStream.read(buffer)) != -1) {
                outputStream.write(buffer, 0, bytesRead);
            }
            return tempFile;
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (Objects.nonNull(outputStream)) {
                try {
                    outputStream.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

        }
        return null;
    }

    /**
     * 启动服务
     *
     * @param port
     */
    public static void start(Integer port, String hookApi) {
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
                            ch.pipeline().addLast(new DelimiterBasedFrameDecoder(1024 * 100, Delimiters.lineDelimiter()));
                            ch.pipeline().addLast(new StringDecoder(CharsetUtil.UTF_8));
                            ch.pipeline().addLast(new StringEncoder(CharsetUtil.UTF_8));
                            ch.pipeline().addLast(new ReceiveMsgHandler(hookApi));
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

        private String hookApi;

        public ReceiveMsgHandler(String hookApi) {
            this.hookApi = hookApi;
        }

        @Override
        protected void channelRead0(ChannelHandlerContext ctx, String msg) {
            JSONObject jsonObject = JSON.parseObject(msg);

            jsonObject.forEach((k, v) -> {
                System.out.println(k + " = " + v);
            });

            String fromGroup = jsonObject.getString("fromGroup");
            String fromUser = jsonObject.getString("fromUser");
            String from;
            if (fromGroup.equals(fromUser)) {
                JSONObject fromGroupJson = WeChatHookClient.query_nickname(jsonObject.getString("fromGroup"));
                String groupNname = fromGroupJson.getString("name");
                from = "消息来自：" + groupNname;
                jsonObject.put("fromUserName", groupNname);
            } else {
                JSONObject fromGroupJson = WeChatHookClient.query_nickname(jsonObject.getString("fromGroup"));
                String groupNname = fromGroupJson.getString("name");

                JSONObject fromUserJson = WeChatHookClient.query_nickname(jsonObject.getString("fromUser"));
                String fromUserName = fromUserJson.getString("name");
                jsonObject.put("fromUserName", fromUserName);

                from = "消息来自：" + groupNname + "->" + fromUserName;
            }
            System.out.println("----------" + from + "----------");
            //消息转发
            if (StringUtil.isNullOrEmpty(hookApi)) {
                return;
            }
            //检查api接口是否是通的
            //转发消息
            try {
                WeChatHookClient.hook(hookApi, msg);
            } catch (Exception e) {
                //请检查hookApi服务是否正常
                System.err.println("--》消息转发失败，请检查hookApi服务是否正常");
            }

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
