## 可以使用对应分支下的注入工具，或者自己编译一下 source目录下的注入程序。

1.ConsoleApplication.exe   
编译好的x64版本的注入器  
命令行注入工具，注入命令 
``` javascript
    //-i  注入程序名   -p 注入dll路径   
    // -u 卸载程序名   -d 卸载dll名称
    //注入  
    ConsoleInject.exe  -i demo.exe -p E:\wxhelper.dll
    //卸载 
    ConsoleInject.exe  -u demo.exe -d  wxhelper.dll
  
```