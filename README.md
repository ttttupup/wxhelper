# wxhelper
wechat hook 。PC端微信逆向学习。支持3.8.0.41，3.8.1.26, 3.9.0.28, 3.9.2.23版本。
#### 免责声明:
本仓库发布的内容，仅用于学习研究，请勿用于非法用途和商业用途！如因此产生任何法律纠纷，均与作者无关！

#### 项目说明：
本项目是个人学习学习逆向的项目，参考  https://github.com/ljc545w/ComWeChatRobot ，在此基础上实现了微信的的其它版本的部分内容。

#### 实现原理：  
逆向分析PC端微信客户端，定位相关功能关键Call，编写dll调用关键Call。然后将该dll文件注入到微信进程。   
dll在注入成功时，创建了一个默认端口为19088的http服务端，然后所有的功能直接可以通过http协议调用。       
```

                                      |----------------
--------------------------    注入    |  WeChat.exe    |
| ConsoleApplication.exe  |————————>  |----------------           --------------    访问      ---------
|                         |           | wxhelper.dll   |————————>| 启动http服务  | <----------| clent |
|--------------------------           |-----------------          --------------              --------    

```
#### 使用说明：
支持的版本3.8.0.41、3.8.1.26、3.9.0.28、3.9.2.23 。  
源码和主要实现在相应的分支内。  
src:主要的dll代码  
tool：简单的注入工具，一个是控制台，一个是图形界面。  
python: tcpserver.py: 简单的服务器，用以接收消息内容。decrpty.py: 微信数据库解密工具。      
source: 简单的命令行远程注入源码。   


#### 0.首先安装对应的版本的微信，分支名称即代表的是微信对应的版本。dll的前缀都会带有微信版本。
#### 1.使用注入工具注入wxhelper.dll,注入成功后，即可通过postman直接调用对应的接口。
#### 2.可以使用python/clent.py进行简单测试。    


##### 特别注意：  
 
##### 1.hook相关的接口都需要先调用对应的hook接口，server端才会收到相应消息。    
##### 2.注意个别接口在一些版本没有实现，功能预览里没有的功能就是没有实现。      
##### 3.如果注入不成功，请先检查注入工具，或者使用其他注入工具。     
##### 4.相关功能只在win11环境下进行简单测试，其他环境无法保证。  


#### 参与项目
个人精力和水平有限，项目还有许多不足，欢迎提出 issues 或 pr。期待你的贡献。



#### 问题讨论   
个人常用的方法，请参考https://github.com/ttttupup/wxhelper/wiki    
使用上的问题，可查询https://github.com/ttttupup/wxhelper/discussions    
数据库解密，请参考https://github.com/ttttupup/wxhelper/wiki      
个人精力有限，只维护最新版本，旧版本的bug会在新版本中修复，不维护旧版本。   


#### 编译环境

Visual Studio 2022(x86)  

Visual Studio code   

cmake  

vcpkg
#### 编译构建
先准备好编译环境。   
```
cd wxhelper  
mkdir build  
cd build  
cmake -DCMAKE_C_COMPILER=cl.exe  \
-DCMAKE_CXX_COMPILER=cl.exe \
-DCMAKE_BUILD_TYPE=Debug \
-DCMAKE_INSTALL_PREFIX=C:/other/codeSource/windows/wxhelper/out/install/x86-debug \
-DCMAKE_TOOLCHAIN_FILE:FILEPATH=C:/vcpkg/scripts/buildsystems/vcpkg.cmake \
-SC:/wxhelper \
-BC:/wxhelper/build/x86-debug\
-G Ninja

cmake --build ..  
```

以下是在vscode中操作，vs中的操作类似。  
1.安装vcpkg，cmake，vscode  

2.安装相应的库，如果安装的版本不同，则根据vcpkg安装成功后提示的find_package修改CMakeLists.txt内容即可。或者自己编译。
```
    vcpkg  install mongoose  
    vcpkg  install nlohmann-json
```
3.vscode 配置CMakePresets.json,主要设置CMAKE_C_COMPILER 和CMAKE_CXX_COMPILER 为cl.exe.参考如下
```
 {
            "name": "x86-release",
            "displayName": "x86-release",
            "description": "Sets Ninja generator, build and install directory",
            "generator": "Ninja",
            "binaryDir": "${sourceDir}/out/build/${presetName}",
            "architecture":{
                "value": "x86",
                "strategy": "external"
            },
            "cacheVariables": {
                "CMAKE_C_COMPILER": "cl.exe",
                "CMAKE_CXX_COMPILER": "cl.exe",
                "CMAKE_BUILD_TYPE": "Release",
                "CMAKE_INSTALL_PREFIX": "${sourceDir}/out/install/${presetName}",
                "CMAKE_TOOLCHAIN_FILE": {
                    "value": "C:/soft/vcpkg/scripts/buildsystems/vcpkg.cmake",
                     "type": "FILEPATH"
                  }
            },
            "environment": {

            }
          
        }
```
4.执行cmake build vscode中右键configure all  projects,在Terminal中点击Run Task，如没有先配置build任务，然后运行即可   

5.命令行注入工具，注入命令 
``` javascript
    //-i  注入程序名   -p 注入dll路径   
    // -u 卸载程序名   -d 卸载dll名称
    // -m pid  关闭微信互斥体，多开微信
    // -P port 指定http端口，需要使用 specify-port 分支的生成的dll
    //注入  
    ConsoleInject.exe  -i demo.exe -p E:\testInject.dll
    //卸载 
    ConsoleInject.exe  -u demo.exe -d  testInject.dll
    //多开
    ConsoleInject.exe  -m 1222
    // 注入并指定http端口
    ConsoleInject.exe  -i demo.exe -p E:\testInject.dll  -P  18888
```

#### 更新说明
2022-12-26 ： 增加3.8.1.26版本支持。  

2022-12-29 ： 新增提取文字功能。  

2023-01-02 ： 退出微信登录。  

2023-01-31 ： 新增修改群昵称（仅支持3.8.1.26）。  

2023-02-01 ： 新增拍一拍（仅支持3.8.1.26）。  

2023-02-04 ： 新增群消息置顶和取消置顶。  

2023-02-06 ： 新增确认收款。  

2023-02-08 ： 新增朋友圈消息。  

2023-02-09 ： 新增3.9.0.28版本基础功能。  

2023-02-13 ： 新增群昵称和微信名称。    

2023-02-17 :  新增通过wxid添加好友,搜索查找微信。  

2023-03-02 ： 新增发送@消息     

2023-03-04 ： 新增消息附件下载      

2023-03-21 ： 新增hook语音     

2023-03-30 ： 新增获取语音文件(推荐使用这个非hook接口)      

2023-04-08 : 3.9.2.23版本功能更新   

#### 功能预览：
0.检查是否登录    
1.获取登录微信信息    
2.发送文本    
3.发送@文本   
5.发送图片     
6.发送文件  
9.hook消息  
10.取消hook消息  
11.hook图片    
12.取消hook图片    
13.hook语音   
14.取消hook语音    
17.删除好友    
19.通过手机或qq查找微信  
20.通过wxid添加好友    
23.通过好友申请   
25.获取群成员  
26.获取群成员昵称  
27.删除群成员  
28.增加群成员   
31.修改群昵称   
32.获取数据库句柄      
34.查询数据库    
35.hook日志   
36.取消hook日志    
40.转发消息        
44.退出登录    
45.确认收款     
46.联系人列表      
47.获取群详情   
48.获取解密图片    
49.图片提取文字ocr    
50.拍一拍  
51.群消息置顶消息    
52.群消息取消置顶    
53.朋友圈首页    
54.朋友圈下一页    
55.获取联系人或者群名称    
56.获取消息附件（图片，视频，文件）   
57.获取语音文件(silk3格式)    
#### 感谢
https://github.com/ljc545w/ComWeChatRobot  

https://github.com/NationalSecurityAgency/ghidra  

https://github.com/x64dbg/x64dbg  
