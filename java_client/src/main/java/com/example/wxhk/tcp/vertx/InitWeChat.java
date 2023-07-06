package com.example.wxhk.tcp.vertx;

import com.example.wxhk.util.HttpAsyncUtil;
import com.example.wxhk.util.HttpSyncUtil;
import io.vertx.core.impl.ConcurrentHashSet;
import io.vertx.core.json.JsonObject;
import org.dromara.hutool.core.io.file.FileUtil;
import org.dromara.hutool.core.net.NetUtil;
import org.dromara.hutool.core.text.StrUtil;
import org.dromara.hutool.core.thread.ThreadUtil;
import org.dromara.hutool.log.Log;
import org.dromara.hutool.setting.Setting;
import org.jetbrains.annotations.NotNull;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.CommandLineRunner;
import org.springframework.core.annotation.Order;
import org.springframework.core.io.ClassPathResource;
import org.springframework.stereotype.Component;

import java.io.File;
import java.io.IOException;

/**
 * 微信注入环境初始化和相关方法
 *
 * @author wt
 * @date 2023/05/16
 */
@Order(-1)
@Component
public class InitWeChat implements CommandLineRunner {

    public final static Log log = Log.get();
    public static final ConcurrentHashSet<String> WXID_MAP = new ConcurrentHashSet<>();
    public static String wxPath;
    public static Integer wxPort;
    public static Integer vertxPort;
    /**
     * wxhelper.dll 所在路径
     */
    public static File DLL_PATH;

    public static void 注入dll(String wxPid) throws IOException {
        String format = StrUtil.format("cmd /C  c.exe -I {} -p {}\\wxhelper.dll -m {}", wxPid, DLL_PATH.getAbsolutePath(), wxPid);
        Process exec = Runtime.getRuntime().exec(format, null, DLL_PATH);
        log.info("注入结果:{}", new String(exec.getInputStream().readAllBytes(), "gbk"));
    }

    @NotNull
    private static File 环境初始化() {
        File target = new File(new File("").getAbsolutePath().split("\\\\")[0] + "\\exec\\");
        try {
            File wxPathFile = new File(wxPath);
            File config = new File(wxPathFile.getParentFile(), "config.ini");
            Setting setting = new Setting(config.getAbsolutePath());
            setting.getGroupedMap().put("config", "port", String.valueOf(wxPort));
            setting.store();
            ClassPathResource classPathResource = new ClassPathResource("exec");
            File file = classPathResource.getFile();
            target.mkdir();
            for (File listFile : file.listFiles()) {
                FileUtil.copy(listFile, target, true);
            }
        } catch (Exception e) {
            log.error(e, "环境初始化失败,请检查");
        }
        return target;
    }

    /**
     * 返回最后一个微信的pid
     *
     * @return {@link String}
     * @throws IOException ioexception
     */
    public static String createWx() throws IOException {
        Runtime.getRuntime().exec("cmd /C  \"" + wxPath + "\"");
        return getWxPid();
    }

    @NotNull
    private static String getWxPid() throws IOException {
        String line = null;
        try {
            Process exec = Runtime.getRuntime().exec("cmd /C tasklist /FI \"IMAGENAME eq WeChat.exe\" ");
            byte[] bytes = exec.getInputStream().readAllBytes();
            line = new String(bytes, "gbk");
            String[] split = line.split("\n");
            if (!line.contains("WeChat.exe")) {
                return createWx();
            }
            String[] split1 = split[split.length - 1].replaceAll("\\s{2,}", " ").split(" ");
            return split1[1];
        } catch (IOException e) {
            log.error("获取端口错误:{}", line);
            throw e;
        }
    }

    public static Integer getWxPort() {
        return wxPort;
    }

    @Value("${wx.port}")
    public void setWxPort(Integer wxPort) {
        InitWeChat.wxPort = wxPort;
    }

    public static String getWxPath() {
        return wxPath;
    }

    @Value("${wx.path}")
    public void setWxPath(String wxPath) {
        InitWeChat.wxPath = wxPath;
    }

    public static Integer getVertxPort() {
        return vertxPort;
    }

    @Value("${vertx.port}")
    public void setVertxPort(Integer vertxPort) {
        InitWeChat.vertxPort = vertxPort;
    }

    @Override
    public void run(String... args) throws Exception {
        //tasklist /FI "IMAGENAME eq WeChat.exe" /m
        boolean usableLocalPort = NetUtil.isUsableLocalPort(wxPort);
        if (usableLocalPort) {
            DLL_PATH = 环境初始化();
            String wxPid = getWxPid();
            注入dll(wxPid);
        }
        ThreadUtil.execute(() -> {
            while (!Thread.currentThread().isInterrupted()) {
                JsonObject exec = HttpSyncUtil.exec(HttpAsyncUtil.Type.检查微信登陆, new JsonObject());
                if (exec.getInteger("code").equals(1)) {
                    JsonObject dl = HttpSyncUtil.exec(HttpAsyncUtil.Type.获取登录信息, new JsonObject());
                    JsonObject jsonObject = dl.getJsonObject("data");
                    String wx = jsonObject.getString("wxid");
                    WXID_MAP.add(wx);
                    if (log.isDebugEnabled()) {
                        log.debug("检测到微信登陆:{}", wx);
                    }
                    break;
                }
                ThreadUtil.safeSleep(500);
            }

        });
        // FIXME: 2023/6/2 程序结束后关闭hook会偶尔出现微信闪退情况,暂时禁用
//        Runtime.getRuntime().addShutdownHook(new Thread(HttpSendUtil::关闭hook));
        //netstat -aon|findstr "端口号"
        // c.exe -I 4568 -p D:\exec\wxhelper.dll -m 4568
    }
}
