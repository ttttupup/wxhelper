#maven打包
```aidl
进入weChatHook-java项目根目录执行如下，命令进行打包

mvn package

打包完成后在target目录下找到
weChatHook-java-1.0-jar-with-dependencies.jar
文件就可以直接启动了
```
#启动命令
####命令参数说明
###port:监听的端口 默认端口19077
###hookApi：消息转发的接口 为空不转发
```aidl
java -jar .\weChatHook-java-1.0-jar-with-dependencies.jar --port=9999 --hookApi=http://localhost:29099/api/demo/msg
```
#java接收hook消息示例
```aidl
@RequestMapping("/api/demo")
public class DemoController {
    @PostMapping("/msg")
      public void getMsg(String msg){
        JSONObject jsonObject = JSON.parseObject(msg);
        jsonObject.forEach((k,v)->{
          System.out.println(k+":"+v);
     });
}
```