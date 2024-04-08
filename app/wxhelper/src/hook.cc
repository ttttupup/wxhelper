#include <WinSock2.h>
#include "hook.h"


#include "client_socket.h"
#include "config.h"
#include "detours.h"
#include "http_client.h"
#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"
// #include "thread_pool.h"
#include "utils.h"
#include "wxutils.h"
namespace hook {
void SendHttpMsgCallback(PTP_CALLBACK_INSTANCE instance, PVOID context,
                         PTP_WORK Work) {
  InnerMessageStruct *msg = (InnerMessageStruct *)context;
  if (msg == NULL) {
    SPDLOG_INFO("http msg is null");
    return;
  }
  std::unique_ptr<InnerMessageStruct> sms(msg);
  nlohmann::json j_msg = nlohmann::json::parse(
      msg->buffer, msg->buffer + msg->length, nullptr, false);
  if (j_msg.is_discarded()) {
    return;
  }
  std::string jstr = j_msg.dump() + "\n";
  std::string url = wxhelper::Config::GetInstance().GetRecvHttpUrl();
  int timeout = wxhelper::Config::GetInstance().GetRecvHttpTimeout();
  http::HttpClient client{url, timeout};
  client.SendRequest(jstr);
}


void SendTcpMsgCallback(PTP_CALLBACK_INSTANCE instance, PVOID context,
                        PTP_WORK Work) {
  InnerMessageStruct *msg = (InnerMessageStruct *)context;
  if (msg == NULL) {
    SPDLOG_INFO("add work:msg is null");
    return;
  }
  std::unique_ptr<InnerMessageStruct> sms(msg);
  nlohmann::json j_msg = nlohmann::json::parse(
      msg->buffer, msg->buffer + msg->length, nullptr, false);
  if (j_msg.is_discarded()) {
    return;
  }
  std::string jstr = j_msg.dump() + "\n";
  std::string ip = wxhelper::Config::GetInstance().GetRecvTcpIp();
  int port = wxhelper::Config::GetInstance().GetRecvTcpPort();
  wxhelper::TcpClient client{ip, port};
  client.SendAndCloseSocket(jstr);
}



BaseHook::BaseHook(void *origin, void *detour)
    : origin_(origin), detour_(detour) {}

int BaseHook::Hook() {
  if (hook_flag_) {
    SPDLOG_INFO("hook already called");
    return 2;
  }
  DetourTransactionBegin();
  DetourUpdateThread(GetCurrentThread());
  DetourAttach((PVOID *)origin_, detour_);
  LONG ret = DetourTransactionCommit();
  if (ret == NO_ERROR) {
    hook_flag_ = true;
  }
  return ret;
}

int BaseHook::Unhook() {
  if (!hook_flag_) {
    SPDLOG_INFO("hook already called");
    return NO_ERROR;
  }
  UINT64 base = wxhelper::wxutils::GetWeChatWinBase();
  DetourTransactionBegin();
  DetourUpdateThread(GetCurrentThread());
  DetourDetach((PVOID *)origin_, detour_);
  LONG ret = DetourTransactionCommit();
  if (ret == NO_ERROR) {
    hook_flag_ = false;
  }
  return ret;
}

}  // namespace hook