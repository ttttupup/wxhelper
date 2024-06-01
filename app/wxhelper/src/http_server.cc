#include "http_server.h" // 引入 http_server.h 文件，包含了 HttpServer 类的定义

#include "http_router.h"     // 引入 http_router.h 文件，包含了 HttpRouter 类的定义
#include "nlohmann/json.hpp" // 引入 nlohmann/json.hpp 文件，用于 JSON 处理
#include "spdlog/spdlog.h"   // 引入 spdlog/spdlog.h 文件，用于日志记录
#include "utils.h"           // 引入 utils.h 文件，包含了一些辅助函数
#include "wxutils.h"         // 引入 wxutils.h 文件，包含了一些与 wxWidgets 相关的辅助函数

namespace http // 定义命名空间 http，用于组织代码
{

  HttpServer::~HttpServer() // HttpServer 类的析构函数
  {
    if (thread_ != nullptr) // 如果线程指针 thread_ 不为空
    {
      // wxhelper::wxutils::print_utf8_to_console("Destroying thread"); // 打印到控制台，告知正在销毁线程
      CloseHandle(thread_); // 关闭线程句柄
    }
    mg_mgr_free(&mgr_); // 释放 CivetWeb 管理器
  }

  void HttpServer::init(std::string &&host, int port) // HttpServer 类的初始化函数
  {
    if (port < 1 || port > 65535) // 如果端口号不在有效范围内
    {
      throw std::invalid_argument("Port number is out of range."); // 抛出异常
    }
    mg_mgr_init(&mgr_);      // 初始化 CivetWeb 管理器
    host_ = std::move(host); // 使用移动构造函数将 host 参数赋值给成员变量 host_
    port_ = port;            // 将端口号赋值给成员变量 port_
    // wxhelper::wxutils::print_utf8_to_console("http init :");
    // wxhelper::wxutils::print_utf8_to_console(host_); // 打印初始化信息到控制台
    // wxhelper::wxutils::print_utf8_to_console(std::to_string(port_));
    SPDLOG_INFO("http init :host={},port={}", host_, port_); // 记录日志
  }

  bool HttpServer::Start() // HttpServer 类的启动函数
  {
    if (running_) // 如果服务器已经在运行
    {
      wxhelper::wxutils::print_utf8_to_console("http server is already running");
      SPDLOG_INFO("http server is already running"); // 记录日志
      return true;                                   // 返回 true
    }
#ifdef _DEBUG                     // 如果定义了 _DEBUG 宏
    base::utils::CreateConsole(); // 创建控制台窗口
#endif
    running_.store(true); // 将 running_ 原子变量设置为 true
    thread_ = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LaunchServer, this,
                           NULL, 0); // 创建线程运行 LaunchServer 函数
    wxhelper::wxutils::print_utf8_to_console("CreateThread for http server ,the result is...");
    SPDLOG_INFO("CreateThread for http server ,the result is {}", thread_ != NULL);
    return thread_ != NULL; // 返回线程创建是否成功
  }

  bool HttpServer::Stop() // HttpServer 类的停止函数
  {
    if (!running_) // 如果服务器不在运行状态
    {
      wxhelper::wxutils::print_utf8_to_console("http server is not running");
      return true; // 直接返回 true
    }
#ifdef _DEBUG
    base::utils::CloseConsole(); // 关闭控制台窗口
#endif
    running_ = false; // 将 running_ 设置为 false
    if (thread_)      // 如果线程句柄不为空
    {
      WaitForSingleObject(thread_, -1); // 等待线程结束
      CloseHandle(thread_);             // 关闭线程句柄
      thread_ = NULL;                   // 将线程句柄设置为 NULL
    }
    return true; // 返回 true
  }

  void HttpServer::LaunchServer(HttpServer *server) // HttpServer 类的服务器启动函数
  {
    int port = server->GetPort();                                // 获取端口号
    std::string host = server->GetHost();                        // 获取主机名
    const mg_mgr *mgr = server->GetMgr();                        // 获取 CivetWeb 管理器
    std::string listen_addr = host + ":" + std::to_string(port); // 构建监听地址
    if (mg_http_listen(const_cast<mg_mgr *>(mgr), listen_addr.c_str(),
                       EventHandler, NULL) == NULL) // 如果监听失败
    {
      // wxhelper::wxutils::print_utf8_to_console("http server  listen fail.port:{}");
      // wxhelper::wxutils::print_utf8_to_console(std::to_string(port)); // 打印错误信息到控制台
      SPDLOG_ERROR("http server  listen fail.port:{}", port); // 记录错误日志
      return;                                                 // 返回
    }
    // wxhelper::wxutils::print_utf8_to_console("Listening on port: {}");
    // wxhelper::wxutils::print_utf8_to_console(std::to_string(port)); // 打印监听信息到控制台
    for (;;)
    {
      mg_mgr_poll(const_cast<mg_mgr *>(mgr), 100); // 轮询 CivetWeb 管理器
    }
  }

  void HttpServer::EventHandler(struct mg_connection *c, int ev, void *ev_data,
                                void *fn_data) // HttpServer 类的事件处理函数
  {
    if (ev == MG_EV_OPEN) // 如果事件类型是连接打开
    {
      // wxhelper::wxutils::print_utf8_to_console("Connection opened"); // 打印信息到控制台
    }
    else if (ev == MG_EV_HTTP_MSG) // 如果事件类型是 HTTP 消息
    {
      struct mg_http_message *hm = (struct mg_http_message *)ev_data; // 转换事件数据为 mg_http_message 类型
      if (mg_http_match_uri(hm, "/websocket/*"))                      // 如果匹配到 WebSocket 路由
      {
        mg_ws_upgrade(c, hm, NULL); // 升级为 WebSocket 连接
      }
      else if (mg_http_match_uri(hm, "/api/*")) // 如果匹配到 API 路由
      {
        HandleHttpRequest(c, hm, fn_data); // 处理 HTTP 请求
      }
      else // 如果不匹配任何路由
      {
        nlohmann::json res = {{"code", 400},
                              {"msg", "invalid url, please check url"},
                              {"data", NULL}};
        std::string ret = res.dump(); // 将 JSON 对象转换为字符串
        mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s\n",
                      ret.c_str()); // 发送 HTTP 响应
      }
    }
    else if (ev == MG_EV_WS_MSG) // 如果事件类型是 WebSocket 消息
    {
      HandleWebsocketRequest(c, ev_data, fn_data); // 处理 WebSocket 请求
    }
  }

  void HttpServer::HandleHttpRequest(struct mg_connection *c, void *ev_data,
                                     void *fn_data) // HttpServer 类的 HTTP 请求处理函数
  {
    struct mg_http_message *hm = (struct mg_http_message *)ev_data;       // 转换事件数据为 mg_http_message 类型
    wxhelper::wxutils::print_utf8_to_console("Handling HTTP request..."); // 打印信息到控制台
    std::string ret = R"({"code":200,"msg":"success"})";                  // 初始化 HTTP 响应
    try
    {
      if (mg_vcasecmp(&hm->method, "GET") == 0) // 如果请求方法是 GET
      {
        nlohmann::json ret_data = {
            {"code", 200},
            {"data", {}},
            {"msg", "the get method is not supported.please use post method."}};
        ret = ret_data.dump(); // 将 JSON 对象转换为字符串
      }
      else if (mg_vcasecmp(&hm->method, "POST") == 0) // 如果请求方法是 POST
      {
        std::string url(hm->uri.ptr, hm->uri.len); // 获取请求的 URL
        wxhelper::wxutils::print_utf8_to_console("Parsed HTTP url: " + url);
        std::string p = "{}"; // 初始化 POST 参数字符串
        if (hm->body.len > 0) // 如果请求体长度大于 0
        {
          nlohmann::json params = nlohmann::json::parse(hm->body.ptr, hm->body.ptr + hm->body.len); // 解析 POST 参数
          p = params.dump();                                                                        // 将 JSON 对象转换为字符串
        }
        wxhelper::wxutils::print_utf8_to_console("Parsed HTTP parameters: " + p);
        ret = HttpRouter::GetInstance().HandleHttpRequest(url, p); // 调用 HttpRouter 处理请求
      }
    }
    catch (nlohmann::json::exception &e) // 如果解析 JSON 时发生异常
    {
      nlohmann::json res = {{"code", "500"}, {"msg", e.what()}, {"data", NULL}};
      ret = res.dump(); // 设置错误响应
    }

    try
    {
      wxhelper::wxutils::print_utf8_to_console("Sending HTTP response... ");

      // wxhelper::wxutils::print_utf8_to_console("Sending HTTP response: ");
      // wxhelper::wxutils::print_utf8_to_console(std::string(ret));
    }
    catch (const std::exception &e)
    {
      wxhelper::wxutils::print_utf8_to_console("Sending HTTP response error: " + std::string(e.what()));
    }

    mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s\n",
                  ret.c_str()); // 发送 HTTP 响应
  }

  void HttpServer::HandleWebsocketRequest(struct mg_connection *c, void *ev_data,
                                          void *fn_data) // HttpServer 类的 WebSocket 请求处理函数
  {
    struct mg_ws_message *wm = (struct mg_ws_message *)ev_data;             // 转换事件数据为 mg_ws_message 类型
    wxhelper::wxutils::print_utf8_to_console("Handling WebSocket request"); // 打印信息到控制台
    std::string ret = R"({"code":200,"msg":"success"})";                    // 初始化 WebSocket 响应
    try
    {
      nlohmann::json params = nlohmann::json::parse(wm->data.ptr, wm->data.ptr + wm->data.len); // 解析 WebSocket 参数
      std::string cmd = params["cmd"];                                                          // 获取命令
      std::string p = params.dump();                                                            // 将 JSON 对象转换为字符串
      wxhelper::wxutils::print_utf8_to_console("Parsed WebSocket parameters: " + p);
      ret = HttpRouter::GetInstance().HandleHttpRequest(cmd, p); // 调用 HttpRouter 处理请求
    }
    catch (nlohmann::json::exception &e) // 如果解析 JSON 时发生异常
    {
      nlohmann::json res = {{"code", "500"}, {"msg", e.what()}, {"data", NULL}};
      ret = res.dump(); // 设置错误响应
    }
    wxhelper::wxutils::print_utf8_to_console("Sending WebSocket response: {}" + ret);
    mg_ws_send(c, ret.c_str(), ret.size(), WEBSOCKET_OP_TEXT); // 发送 WebSocket 响应
  }

} // namespace http