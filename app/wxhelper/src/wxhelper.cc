#include <winsock2.h>
#include "wxhelper.h"

#include "config.h"
#include "http_server.h"
#include "thread_pool.h"
#include "utils.h"
#include "wxutils.h"
#include "wechat_db.h"
#include "sync_msg_hook.h"
#include "wechat_service.h"

// 引入其他头文件，这里包含了配置、HTTP服务器、线程池、工具函数等模块

namespace wxhelper { // 定义wxhelper命名空间，包含wxhelper相关的函数和类

void WxHelper::init(HMODULE module) {
  // 打印初始化配置信息
  // wxhelper::wxutils::print_utf8_to_console("Config initialization start"); // 调用wxutils命名空间中的print_utf8_to_console函数，打印配置初始化开始信息

  Config::GetInstance().init(); // 获取Config类的单例对象并调用其init函数，进行配置初始化

  // 打印模块隐藏状态
  // wxhelper::wxutils::print_utf8_to_console("Module hiding check"); // 打印模块隐藏检查信息

  if (Config::GetInstance().GetHideDll()) { // 判断配置中是否需要隐藏模块
    // wxhelper::wxutils::print_utf8_to_console("Hiding module"); // 如果需要隐藏，则打印隐藏模块信息
    base::utils::HideModule(module); // 调用base命名空间中utils的HideModule函数，隐藏模块
  }

  // 打印微信数据库初始化信息
  // wxhelper::wxutils::print_utf8_to_console("WeChatDb initialization start"); // 打印微信数据库初始化开始信息
  wechat::WeChatDb::GetInstance().Init(); // 获取WeChatDb类的单例对象并调用其Init函数，进行微信数据库初始化

  // 打印WeChat服务初始化信息
  // wxhelper::wxutils::print_utf8_to_console("WeChatService initialization start"); // 打印WeChat服务初始化开始信息
  wechat::WeChatService::GetInstance().Init(); // 获取WeChatService类的单例对象并调用其Init函数，进行WeChat服务初始化

  // 打印消息同步钩子初始化信息
  // wxhelper::wxutils::print_utf8_to_console("SyncMsgHook initialization start"); // 打印消息同步钩子初始化开始信息
  SyncMsgHook::GetInstance().Init(); // 获取SyncMsgHook类的单例对象并调用其Init函数，进行消息同步钩子初始化

  // 获取HTTP服务器配置并打印
  // wxhelper::wxutils::print_utf8_to_console("HttpServer initialization start"); // 打印HTTP服务器初始化开始信息
  std::string host = Config::GetInstance().GetHttpServerHost(); // 获取配置中的HTTP服务器主机地址
  int port = Config::GetInstance().GetHttpServerPort(); // 获取配置中的HTTP服务器端口
  // wxhelper::wxutils::print_utf8_to_console("HttpServer host: " + host + ", port: " + std::to_string(port)); // 打印HTTP服务器主机和端口信息
  http::HttpServer::GetInstance().init(Config::GetInstance().GetHttpServerHost(), port); // 获取HttpServer类的单例对象并调用其init函数，初始化HTTP服务器

  // 打印HTTP服务器启动信息
  // wxhelper::wxutils::print_utf8_to_console("HttpServer start"); // 打印HTTP服务器启动信息
  http::HttpServer::GetInstance().Start(); // 调用HttpServer类的Start函数，启动HTTP服务器

  // 打印线程池创建信息
  wxhelper::wxutils::print_utf8_to_console("Thread pool creation start"); // 打印线程池创建开始信息
  base::ThreadPool::GetInstance().Create(2, 8); // 获取ThreadPool类的单例对象并调用其Create函数，创建线程池，参数为最小线程数和最大线程数
}

void WxHelper::finally() {
  // 打印HTTP服务器停止信息
  wxhelper::wxutils::print_utf8_to_console("HttpServer stop"); // 打印HTTP服务器停止信息
  http::HttpServer::GetInstance().Stop(); // 获取HttpServer类的单例对象并调用其Stop函数，停止HTTP服务器
}

}  // namespace wxhelper