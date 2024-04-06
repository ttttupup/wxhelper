#include <WinSock2.h>
#include "hook.h"


#include "base64.h"
#include "client_socket.h"
#include "config.h"
#include "detours.h"
#include "http_client.h"
#include "nlohmann/json.hpp"
#include "offset.h"
#include "spdlog/spdlog.h"
#include "thread_pool.h"
#include "utils.h"
#include "wechat_interface.h"
#include "wxutils.h"
namespace hook {

VOID SendTcpMsgCallback(PTP_CALLBACK_INSTANCE instance, PVOID context,
                        PTP_WORK Work) {
  wechat::InnerMessageStruct *msg = (wechat::InnerMessageStruct *)context;
  if (msg == NULL) {
    SPDLOG_INFO("add work:msg is null");
    return;
  }
  std::unique_ptr<wechat::InnerMessageStruct> sms(msg);
  nlohmann::json j_msg = nlohmann::json::parse(
      msg->buffer, msg->buffer + msg->length, nullptr, false);
  if (j_msg.is_discarded() == true) {
    return;
  }
  std::string jstr = j_msg.dump() + "\n";
  std::string ip =wxhelper::Config::GetInstance().GetRecvTcpIp();
  int port = wxhelper::Config::GetInstance().GetRecvTcpPort();
  wxhelper::TcpClient client{ip,port};
  client.SendAndCloseSocket(jstr);
}

VOID SendHttpMsgCallback(PTP_CALLBACK_INSTANCE instance, PVOID context,
                         PTP_WORK Work) {
  wechat::InnerMessageStruct *msg = (wechat::InnerMessageStruct *)context;
  if (msg == NULL) {
    SPDLOG_INFO("http msg is null");
    return;
  }

  std::unique_ptr<wechat::InnerMessageStruct> sms(msg);
  nlohmann::json j_msg = nlohmann::json::parse(
      msg->buffer, msg->buffer + msg->length, nullptr, false);
  if (j_msg.is_discarded() == true) {
    return;
  }
  std::string jstr = j_msg.dump() + "\n";
  std::string url =wxhelper::Config::GetInstance().GetRecvHttpUrl();
  int timeout = wxhelper::Config::GetInstance().GetRecvHttpTimeout();
  http::HttpClient client{url,timeout};
  client.SendRequest(jstr);
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
  DetourAttach((PVOID *)origin_, (PVOID *)detour_);
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
  DetourDetach((PVOID *)origin_, (PVOID *)detour_);
  LONG ret = DetourTransactionCommit();
  if (ret == NO_ERROR) {
    hook_flag_ = false;
  }
  return ret;
}

SyncMsgHook::SyncMsgHook(void *origin, void *detour)
    : BaseHook(origin, detour) {}

void SyncMsgHook::HandleSyncMsg(INT64 param1, INT64 param2, INT64 param3) {
  nlohmann::json msg;

  msg["pid"] = GetCurrentProcessId();
  msg["fromUser"] =
      wxhelper::wxutils::ReadSKBuiltinString(*(INT64 *)(param2 + 0x18));
  msg["toUser"] =
      wxhelper::wxutils::ReadSKBuiltinString(*(INT64 *)(param2 + 0x28));
  msg["content"] =
      wxhelper::wxutils::ReadSKBuiltinString(*(INT64 *)(param2 + 0x30));
  msg["signature"] =
      wxhelper::wxutils::ReadWeChatStr(*(INT64 *)(param2 + 0x48));
  msg["msgId"] = *(INT64 *)(param2 + 0x60);
  msg["msgSequence"] = *(DWORD *)(param2 + 0x5C);
  msg["createTime"] = *(DWORD *)(param2 + 0x58);
  msg["displayFullContent"] =
      wxhelper::wxutils::ReadWeChatStr(*(INT64 *)(param2 + 0x50));
  DWORD type = *(DWORD *)(param2 + 0x24);
  msg["type"] = type;
  if (type == 3) {
    int a = 1;
    std::string img =
        wxhelper::wxutils::ReadSKBuiltinBuffer(*(INT64 *)(param2 + 0x40));
    SPDLOG_INFO("encode size:{}", img.size());
    msg["base64Img"] = base64_encode(img);
    a = 2;
  }
  std::string jstr = msg.dump() + '\n';
  wechat::InnerMessageStruct *inner_msg = new wechat::InnerMessageStruct;
  inner_msg->buffer = new char[jstr.size() + 1];
  memcpy(inner_msg->buffer, jstr.c_str(), jstr.size() + 1);
  inner_msg->length = jstr.size();
  std::string mode = wxhelper::Config::GetInstance().GetRecvMessageMode();
  if (mode == "http") {
    bool add =
        base::ThreadPool::GetInstance().AddWork(SendHttpMsgCallback, inner_msg);
    SPDLOG_INFO("add http msg work:{}", add);
  } else if (mode == "tcp") {
    bool add =
        base::ThreadPool::GetInstance().AddWork(SendTcpMsgCallback, inner_msg);
    SPDLOG_INFO("add tcp msg work:{}", add);
  }
  int64_t addr =
      wxhelper::wxutils::GetWeChatWinBase() + wechat::offset::kDoAddMsg;
  wechat::function::__DoAddMsg addMsg = (wechat::function::__DoAddMsg)addr;
  addMsg(param1, param2, param3);
}

}  // namespace hook