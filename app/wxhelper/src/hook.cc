#include <WinSock2.h>
#include "hook.h"

#include "client_socket.h"
#include "config.h"
#include "detours.h"
#include "http_client.h"
#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"
#include "thread_pool.h"
#include "utils.h"
#include "wxutils.h"
#include <iostream>

namespace hook
{
  /**
   * @brief 发送 HTTP 消息的回调函数
   * @param instance 回调实例
   * @param context 上下文
   * @param Work 工作项
   */
  void SendHttpMsgCallback(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_WORK Work)
  {
    wxhelper::wxutils::print_utf8_to_console("SendHttpMsgCallback called"); // 调试打印

    SPDLOG_INFO("SendHttpMsgCallback called"); // 调试打印

    InnerMessageStruct *msg = (InnerMessageStruct *)context;
    if (msg == NULL)
    {
      wxhelper::wxutils::print_utf8_to_console("httpMsg is null"); // 调试打印

      SPDLOG_INFO("httpMsg is null");
      return;
    }
    std::unique_ptr<InnerMessageStruct> sms(msg);
    nlohmann::json j_msg = nlohmann::json::parse(
        msg->buffer, msg->buffer + msg->length, nullptr, false);
    if (j_msg.is_discarded())
    {
      wxhelper::wxutils::print_utf8_to_console("jsonMsg discarded"); // 调试打印

      SPDLOG_INFO("jsonMsg discarded"); // 调试打印
      return;
    }
    std::string jstr = j_msg.dump() + "\n";
    std::string url = wxhelper::Config::GetInstance().GetRecvHttpUrl();
    int timeout = wxhelper::Config::GetInstance().GetRecvHttpTimeout();
    http::HttpClient client{url, timeout};
    client.SendRequest(jstr);
  }

  /**
   * @brief 发送 TCP 消息的回调函数
   * @param instance 回调实例
   * @param context 上下文
   * @param Work 工作项
   */
  void SendTcpMsgCallback(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_WORK Work)
  {
    wxhelper::wxutils::print_utf8_to_console("SendTcpMsgCallback called"); // 调试打印

    SPDLOG_INFO("SendTcpMsgCallback called"); // 调试打印
    InnerMessageStruct *msg = (InnerMessageStruct *)context;
    if (msg == NULL)
    {
      wxhelper::wxutils::print_utf8_to_console("add work:msg is null"); // 调试打印

      SPDLOG_INFO("add work:msg is null");
      return;
    }
    std::unique_ptr<InnerMessageStruct> sms(msg);
    nlohmann::json j_msg = nlohmann::json::parse(
        msg->buffer, msg->buffer + msg->length, nullptr, false);
    if (j_msg.is_discarded())
    {
      wxhelper::wxutils::print_utf8_to_console("jsonMsg discarded"); // 调试打印
      SPDLOG_INFO("jsonMsg discarded");                              // 调试打印
      return;
    }
    std::string jstr = j_msg.dump() + "\n";
    // wxhelper::wxutils::print_utf8_to_console("SendTcpMsgCallback jstr:" + jstr);
    wxhelper::wxutils::print_utf8_to_console("SendTcpMsgCallback jstr return JSON");

    std::string ip = wxhelper::Config::GetInstance().GetRecvTcpIp();
    int port = wxhelper::Config::GetInstance().GetRecvTcpPort();
    wxhelper::TcpClient client{ip, port};
    client.SendAndCloseSocket(jstr);
  }

  /**
   * @brief 钩子类的构造函数
   * @param origin 原始函数指针
   * @param detour 替换函数指针
   */
  BaseHook::BaseHook(void *origin, void *detour)
      : origin_(origin), detour_(detour)
  {
    wxhelper::wxutils::print_utf8_to_console("BaseHook created with origin");       // 调试打印
                                                                                    // 调试打印
    SPDLOG_INFO("BaseHook created with origin: {} and detour: {}", origin, detour); // 调试打印
  }

  /**
   * @brief 安装钩子函数
   * @return 返回安装钩子的结果
   */
  int BaseHook::Hook()
  {
    wxhelper::wxutils::print_utf8_to_console("Hook called"); // 调试打印
    SPDLOG_INFO("Hook called");                              // 调试打印
    if (hook_flag_)
    {
      // std::cout << "hook already called" << std::endl; // 调试打印
      wxhelper::wxutils::print_utf8_to_console("hook already called"); // 调试打印

      SPDLOG_INFO("hook already called");

      return 2;
    }
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach((PVOID *)origin_, detour_);
    LONG ret = DetourTransactionCommit();
    if (ret == NO_ERROR)
    {
      hook_flag_ = true;
    }
    return ret;
  }

  /**
   * @brief 卸载钩子函数
   * @return 返回卸载钩子的结果
   */
  int BaseHook::Unhook()
  {
    wxhelper::wxutils::print_utf8_to_console("Unhook called"); // 调试打印

    SPDLOG_INFO("Unhook called"); // 调试打印
    if (!hook_flag_)
    {
      wxhelper::wxutils::print_utf8_to_console("hook already called"); // 调试打印
      SPDLOG_INFO("hook already called");
      return NO_ERROR;
    }
    UINT64 base = wxhelper::wxutils::GetWeChatWinBase();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach((PVOID *)origin_, detour_);
    LONG ret = DetourTransactionCommit();
    if (ret == NO_ERROR)
    {
      hook_flag_ = false;
    }
    return ret;
  }

} // namespace hook