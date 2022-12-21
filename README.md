# wxhelper
wechat hook .
#### 免责声明:
本仓库发布的内容，仅用于学习研究，请勿用于非法用途和商业用途！如因此产生任何法律纠纷，均与作者无关！

#### 项目说明：
本项目是个人学习学习逆向的项目，主要参考https://github.com/ttttupup/ComWeChatRobot，在此基础上实现了wechat 3.8.0.41的版本的部分内容。

#### 使用说明：
支持的版本3.8.0.41，目前是最新版本。
src:主要的dll代码
tool：简单的注入工具，一个是控制台，一个是图形界面。
python: 简单的服务器，用以接收消息内容。
release：编译好的dll。

1.通过cmake构建成功后，将wxhelper.dll注入到微信，本地启动tcp server，监听19088端口。
2.通过http协议与dll通信，方便客户端操作。
3.接口的url为http://127.0.0.1:19088，注入成功后，直接进行调用即可。
4.特别注意数据库查询接口需要先调用获取到句柄之后，才能进行查询。
5.相关功能只在win11环境下进行简单测试，其他环境无法保证。

#### 编译环境

Visual Studio 2022(x86)
Visual Studio code 
cmake
vcpkg

### 接口文档：

#### 0.检查微信登录**
###### 接口功能
> 检查微信是否登录

###### 接口地址
> [/api/?type=0](/api/?type=0)

###### HTTP请求方式
> POST  JSON

###### 请求参数
|参数|必选|类型|说明|
|---|---|---|---|


###### 返回字段
|返回字段|字段类型|说明                              |
|---|---|---|
|code|int|返回状态,1 成功, 0失败|
|result|string|成功提示|
|data|string|响应内容|

###### 接口示例
地址：https://www.bincoding.cn/test
入参：
``` javascript
```
响应：
``` javascript
{
    "code": 1,
    "result": "ok"
}
```

#### 1.获取登录用户信息**
###### 接口功能
> 获取登录用户信息

###### 接口地址
> [/api/?type=1](/api/?type=1)

###### HTTP请求方式
> POST  JSON

###### 请求参数
|参数|必选|类型|说明|
|---|---|---|---|


###### 返回字段
|返回字段|字段类型|说明                              |
|---|---|---|
|code|int|返回状态,1 成功, 0失败|
|result|string|成功提示|
|data|object|响应内容|
|account|string|账号|
|bigImage|string|头像大图|
|city|string|城市|
|country|string|国家|
|currentDataPath|string|当前数据目录|
|dataRootPath|string|根目录|
|dataSavePath|string|保存目录|
|mobile|string|手机|
|name|string|昵称|
|province|string|省|
|smallImage|string|小头像|
|wxid|string|wxid|

###### 接口示例
地址：https://www.bincoding.cn/test
入参：
``` javascript
```
响应：
``` javascript
{"code":1,"data":{"account":"xx","bigImage":"https://wx.qlogo.cn/mmhead/ver_1xx","city":"xx","country":"CN","currentDataPath":"xxx","dataRootPath":"C:\\xx","dataSavePath":"C:\\xx","mobie":"13949175447","name":"xx","province":"xx","smallImage":"xx","wxid":"xx"},"result":"OK"}
```




#### 2.发送文本消息**
###### 接口功能
> 发送文本消息

###### 接口地址
> [/api/?type=2](/api/?type=2)

###### HTTP请求方式
> POST  JSON

###### 请求参数
|参数|必选|类型|说明|
|---|---|---|---|
|wxid |ture |string| 接收人wxid |
|msg|true |string|消息文本内容|

###### 返回字段
|返回字段|字段类型|说明                              |
|---|---|---|
|code|int|返回状态,不为0成功, 0失败|
|result|string|成功提示|


###### 接口示例
地址：https://www.bincoding.cn/test
入参：
``` javascript
{
    "wxid": "filehelper",
    "msg": "1112222"
}
```
响应：
``` javascript
{"code":345686720,"result":"OK"}
```



#### 5.发送图片消息**
###### 接口功能
> 发送图片消息

###### 接口地址
> [/api/?type=5](/api/?type=5)

###### HTTP请求方式
> POST  JSON

###### 请求参数
|参数|必选|类型|说明|
|---|---|---|---|
|wxid |ture |string| 接收人wxid |
|imagePath|true |string|图片路径|

###### 返回字段
|返回字段|字段类型|说明                              |
|---|---|---|
|code|int|返回状态,不为0成功, 0失败|
|result|string|成功提示|


###### 接口示例
入参：
``` javascript
{
    "wxid": "filehelper",
    "imagePath": "C:/Users/123.png"
}
```
响应：
``` javascript
{"code":345686724,"result":"OK"}
```



#### 6.发送文件消息**
###### 接口功能
> 发送文件

###### 接口地址
> [/api/?type=6](/api/?type=6)

###### HTTP请求方式
> POST  JSON

###### 请求参数
|参数|必选|类型|说明|
|---|---|---|---|
|wxid |ture |string| 接收人wxid |
|filePath|true |string|文件路径|

###### 返回字段
|返回字段|字段类型|说明                              |
|---|---|---|
|code|int|返回状态,1成功, 0失败|
|result|string|成功提示|


###### 接口示例
入参：
``` javascript
{
    "wxid": "filehelper",
    "filePath": "C:/Users/123.txt"
}
```
响应：
``` javascript
{"code":1,"result":"OK"}
```


#### 9.hook消息**
###### 接口功能
> hook接收文本消息，图片消息，群消息.该接口将hook的消息通过tcp回传给本地的端口

###### 接口地址
> [/api/?type=9](/api/?type=9)

###### HTTP请求方式
> POST  JSON

###### 请求参数
|参数|必选|类型|说明|
|---|---|---|---|
|port |ture |string| 本地服务端端口，用来接收消息内容 |


###### 返回字段
|返回字段|字段类型|说明                              |
|---|---|---|
|code|int|返回状态,1成功, 0失败|
|result|string|成功提示|


###### 接口示例
入参：
``` javascript
{
    "port": "19099"
}
```
响应：
``` javascript
{"code":1,"result":"OK"}
```



#### 11.hook图片**
###### 接口功能
> hook图片原始内容，不推荐该接口，可以使用图片查询接口

###### 接口地址
> [/api/?type=11](/api/?type=11)

###### HTTP请求方式
> POST  JSON

###### 请求参数
|参数|必选|类型|说明|
|---|---|---|---|
|imgDir |ture |string| 图片保存的目录 |


###### 返回字段
|返回字段|字段类型|说明                              |
|---|---|---|
|code|int|返回状态,1成功, 0失败|
|result|string|成功提示|


###### 接口示例
入参：
``` javascript
{
    "imgDir":"C:\\other"
}
```
响应：
``` javascript
{"code":1,"result":"OK"}
```



#### 17.删除好友**
###### 接口功能
> 删除好友

###### 接口地址
> [/api/?type=17](/api/?type=17)

###### HTTP请求方式
> POST  JSON

###### 请求参数
|参数|必选|类型|说明|
|---|---|---|---|
|wxid |ture |string| 好友wxid |


###### 返回字段
|返回字段|字段类型|说明                              |
|---|---|---|
|code|int|返回状态,1成功, 0失败|
|result|string|成功提示|


###### 接口示例
入参：
``` javascript
{
    "wxid":"wxid_o"
}
```
响应：
``` javascript
{"code":1,"result":"OK"}
```



#### 25.获取群成员**
###### 接口功能
> 获取群成员

###### 接口地址
> [/api/?type=25](/api/?type=25)

###### HTTP请求方式
> POST  JSON

###### 请求参数
|参数|必选|类型|说明|
|---|---|---|---|
|chatRoomId |ture |string| 群id |


###### 返回字段
|返回字段|字段类型|说明                              |
|---|---|---|
|code|int|返回状态,1成功, 0失败|
|result|string|成功提示|
|data|object|返回内容|
|admin|string|群主id|
|chatRoomId|string|群id|
|members|string|群成员id以^分隔|


###### 接口示例
入参：
``` javascript
{
    "chatRoomId":"123@chatroom"
}
```
响应：
``` javascript
{"code":1,"data":{"admin":"wxid","chatRoomId":"123@chatroom","members":"wxid_123^Gwxid_456^Gwxid_45677"},"result":"OK"}
```



#### 27.删除群成员**
###### 接口功能
> 删除群成员

###### 接口地址
> [/api/?type=27](/api/?type=27)

###### HTTP请求方式
> POST  JSON

###### 请求参数
|参数|必选|类型|说明|
|---|---|---|---|
|chatRoomId |ture |string| 群id |
|memberIds |ture |string| 成员id，以,分割 |


###### 返回字段
|返回字段|字段类型|说明                              |
|---|---|---|
|code|int|返回状态,1成功, 0失败|
|result|string|成功提示|


###### 接口示例
入参：
``` javascript
{
    "chatRoomId":"34932563384@chatroom",
    "memberIds":"wxid_oyb662qhop4422"
}
```
响应：
``` javascript
{"code":1,"result":"OK"}
```



#### 28.增加群成员**
###### 接口功能
> 增加群成员

###### 接口地址
> [/api/?type=28](/api/?type=28)

###### HTTP请求方式
> POST  JSON

###### 请求参数
|参数|必选|类型|说明|
|---|---|---|---|
|chatRoomId |ture |string| 群id |
|memberIds |ture |string| 成员id，以,分割 |


###### 返回字段
|返回字段|字段类型|说明                              |
|---|---|---|
|code|int|返回状态,1成功, 0失败|
|result|string|成功提示|


###### 接口示例
入参：
``` javascript
{
    "chatRoomId":"34932563384@chatroom",
    "memberIds":"wxid_oyb662qhop4422"
}
```
响应：
``` javascript
{"code":1,"result":"OK"}
```





#### 32.获取数据库句柄**
###### 接口功能
> 获取sqlite3数据库句柄

###### 接口地址
> [/api/?type=32](/api/?type=32)

###### HTTP请求方式
> POST  JSON

###### 请求参数
|参数|必选|类型|说明|
|---|---|---|---|



###### 返回字段
|返回字段|字段类型|说明                              |
|---|---|---|
|code|int|返回状态,1成功, 0失败|
|result|string|成功提示|
|data|array|返回数据|
|databaseName|string|数据库名称|
|handle|int|数据库句柄|
|tables|array|表信息|
|name|string|表名|
|rootpage|string|rootpage|
|sql|string|sql|
|tableName|string|tableName|


###### 接口示例
入参：
``` javascript

```
响应：
``` javascript
{
  "data": [
    {
      "databaseName": "MicroMsg.db",
      "handle": 119561688,
      "tables": [
        {
          "name": "Contact",
          "rootpage": "2",
          "sql": "CREATE TABLE Contact(UserName TEXT PRIMARY KEY ,Alias TEXT,EncryptUserName TEXT,DelFlag INTEGER DEFAULT 0,Type INTEGER DEFAULT 0,VerifyFlag INTEGER DEFAULT 0,Reserved1 INTEGER DEFAULT 0,Reserved2 INTEGER DEFAULT 0,Reserved3 TEXT,Reserved4 TEXT,Remark TEXT,NickName TEXT,LabelIDList TEXT,DomainList TEXT,ChatRoomType int,PYInitial TEXT,QuanPin TEXT,RemarkPYInitial TEXT,RemarkQuanPin TEXT,BigHeadImgUrl TEXT,SmallHeadImgUrl TEXT,HeadImgMd5 TEXT,ChatRoomNotify INTEGER DEFAULT 0,Reserved5 INTEGER DEFAULT 0,Reserved6 TEXT,Reserved7 TEXT,ExtraBuf BLOB,Reserved8 INTEGER DEFAULT 0,Reserved9 INTEGER DEFAULT 0,Reserved10 TEXT,Reserved11 TEXT)",
          "tableName": "Contact"
        }
      ]
    }
  ],
  "result": "OK"
}
```



#### 34.查询数据库**
###### 接口功能
> 查询数据库

###### 接口地址
> [/api/?type=34](/api/?type=34)

###### HTTP请求方式
> POST  JSON

###### 请求参数
|参数|必选|类型|说明|
|---|---|---|---|
|dbHandle |ture |int| 句柄 |
|sql |ture |string| sql语句 |


###### 返回字段
|返回字段|字段类型|说明                              |
|---|---|---|
|code|int|返回状态,1成功, 0失败|
|result|string|成功提示|
|data|array|返回数据|



###### 接口示例
入参：
``` javascript
{
    "dbHandle": 219277920,
    "sql":"select * from MSG where MsgSvrID=8985035417589024392"
}
```
响应：
``` javascript
{"code":1,"data":[["localId","TalkerId","MsgSvrID","Type","SubType","IsSender","CreateTime","Sequence","StatusEx","FlagEx","Status","MsgServerSeq","MsgSequence","StrTalker","StrContent","DisplayContent","Reserved0","Reserved1","Reserved2","Reserved3","Reserved4","Reserved5","Reserved6","CompressContent","BytesExtra","BytesTrans"],["6346","24","8985035417589024392","1","0","0","1670897832","1670897832000","0","0","2","1","778715089","wxid_1222","112","","0","2","","","","","","","CgQIEBAAGkEIBxI9PG1zZ3NvdXJjZT4KCTxzaWduYXR1cmU+djFfSFFyeVAwZTE8L3NpZ25hdHVyZT4KPC9tc2dzb3VyY2U+ChokCAISIDU5NjI1NjUxNWE0YzU2ZDQxZDJlOWMyYmIxMjFhNmZl",""]],"result":"OK"}
```



#### 46.联系人列表**
###### 接口功能
> 联系人列表

###### 接口地址
> [/api/?type=46](/api/?type=46)

###### HTTP请求方式
> POST  JSON

###### 请求参数
|参数|必选|类型|说明|
|---|---|---|---|


###### 返回字段
|返回字段|字段类型|说明                              |
|---|---|---|
|code|int|返回状态,1成功, 0失败|
|result|string|成功提示|
|data|array|返回内容|
|customAccount|string|自定义账号|
|delFlag|int|删除标志|
|type|int|好友类型|
|userName|string|用户名称|
|verifyFlag|int|验证|
|wxid|string|wxid|



###### 接口示例
入参：
``` javascript
{
    "wxid": "filehelper",
    "msgid":7215505498606506901
}
```
响应：
``` javascript
{"code":1,"data":[{"customAccount":"custom","delFlag":0,"type":8388611,"userName":"昵称","verifyFlag":0,"wxid":"wxid_123pcqm22"}]}
```


#### 47.群详情**
###### 接口功能
> 获取群详情

###### 接口地址
> [/api/?type=47](/api/?type=47)

###### HTTP请求方式
> POST  JSON

###### 请求参数
|参数|必选|类型|说明|
|---|---|---|---|
|chatRoomId |ture |string| 群id |

###### 返回字段
|返回字段|字段类型|说明                              |
|---|---|---|
|code|int|返回状态,1成功, 0失败|
|result|string|成功提示|
|data|object|返回内容|
|admin|string|群主id|
|chatRoomId|int|群id|
|notice|int|通知|
|xml|string|xml|



###### 接口示例
入参：
``` javascript
{
    "wxid": "filehelper",
    "msgid":7215505498606506901
}
```
响应：
``` javascript
{"code":1,"data":{"admin":"123","chatRoomId":"123@chatroom","notice":"1222","xml":""},"result":"OK"}
```



#### 48.获取解密图片**
###### 接口功能
> 获取解密图片

###### 接口地址
> [/api/?type=48](/api/?type=48)

###### HTTP请求方式
> POST  JSON

###### 请求参数
|参数|必选|类型|说明|
|---|---|---|---|
|imagePath |ture |string| 图片路径 |
|savePath |ture |string| 保存路径 |

###### 返回字段
|返回字段|字段类型|说明                              |
|---|---|---|
|code|int|返回状态,1成功, 0失败|
|result|string|成功提示|



###### 接口示例
入参：
``` javascript
{
    "imagePath":"C:\\3a610d7bc1cf5a15d12225a64b8962.dat",
    "savePath":"C:\\other"
}

```
响应：
``` javascript
{"code":1,"result":"OK"}
```


#### 感谢
https://github.com/ljc545w/ComWeChatRobot
https://github.com/NationalSecurityAgency/ghidra
https://github.com/x64dbg/x64dbg