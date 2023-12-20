# wxhelper
wechat hook 。PC端微信逆向学习。
#### 免责声明:
本仓库发布的内容，仅用于学习研究，请勿用于非法用途和商业用途！如因此产生任何法律纠纷，均与作者无关！

#### 项目说明：
<font color= "#dd0000">本项目是逆向练习项目，可能会造成封号等后果。请自行承担风险。仅用于学习研究，请勿于非法用途。</font>  

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
#### 快速开始：

1.使用注入工具将wxhelper.dll注入到WeChat中。  
2.执行 

```
curl --location --request POST '127.0.0.1:19088/api/checkLogin'
```
即可检查登录状态。
如果已经登录则执行获取登录用户信息
```
curl --location --request POST '127.0.0.1:19088/api/userInfo'
```

3.其他更多接口参考相关文档。




#### 参与项目
个人精力和水平有限，项目还有许多不足，欢迎提出 issues 或 pr。期待你的贡献。



#### 问题讨论   
个人常用的方法，请参考https://github.com/ttttupup/wxhelper/wiki    
使用上的问题，可查询https://github.com/ttttupup/wxhelper/discussions    
数据库解密，请参考https://github.com/ttttupup/wxhelper/wiki      
个人精力有限，只维护最新版本，旧版本的bug会在新版本中修复，不维护旧版本。   


#### 编译环境

Visual Studio 2022

Visual Studio code   

cmake  

vcpkg
#### 编译构建

```
cd wxhelper  
mkdir build  
cd build  
cmake -DCMAKE_C_COMPILER=cl.exe  \
-DCMAKE_CXX_COMPILER=cl.exe \
-DCMAKE_BUILD_TYPE=Debug \
-DCMAKE_TOOLCHAIN_FILE:FILEPATH=${vcpkg install dir}/scripts/buildsystems/vcpkg.cmake \
-SC:/wxhelper \
-BC:/wxhelper/build/x64-debug\
-G Ninja


#### 讨论组
https://t.me/+LmvAauweyUpjYzJl  
