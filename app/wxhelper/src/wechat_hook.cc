#include <WS2tcpip.h>
#include "wechat_hook.h"


#include <detours/detours.h>
#include <windows.h>

#include <nlohmann/json.hpp>

#include "base64.h"
#include "http_client.h"
#include "spdlog/spdlog.h"
#include "thread_pool.h"
#include "wxutils.h"
namespace offset = wxhelper::V3_9_7_29::offset;
namespace common = wxhelper::common;

namespace hook {
static bool kEnableHttp = false;
static bool kLogHookFlag = false;
static char kServerIp[20] = "127.0.0.1";
static int kServerPort = 19099;

UINT64(*RealDoAddMsg)
(UINT64, UINT64, UINT64) = (UINT64(*)(UINT64, UINT64, UINT64))(
    wxhelper::wxutils::GetWeChatWinBase() + offset::kDoAddMsg);

VOID SendMsgCallback(PTP_CALLBACK_INSTANCE instance, PVOID context,
                     PTP_WORK Work) {
  common::InnerMessageStruct *msg = (common::InnerMessageStruct *)context;
  if (msg == NULL) {
    SPDLOG_INFO("add work:msg is null");
    return;
  }
  std::unique_ptr<common::InnerMessageStruct> sms(msg);
  nlohmann::json j_msg = nlohmann::json::parse(
      msg->buffer, msg->buffer + msg->length, nullptr, false);
  if (j_msg.is_discarded() == true) {
    return;
  }
  std::string jstr = j_msg.dump() + "\n";

  WSADATA was_data = {0};
  int ret = WSAStartup(MAKEWORD(2, 2), &was_data);
  if (ret != 0) {
    SPDLOG_ERROR("WSAStartup failed:{}", ret);
    return;
  }

  SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (client_socket < 0) {
    SPDLOG_ERROR("socket init  fail");
    return;
  }
  BOOL status = false;
  sockaddr_in client_addr;
  memset(&client_addr, 0, sizeof(client_addr));
  client_addr.sin_family = AF_INET;
  client_addr.sin_port = htons((u_short)kServerPort);
  InetPtonA(AF_INET, kServerIp, &client_addr.sin_addr.s_addr);
  if (connect(client_socket, reinterpret_cast<sockaddr *>(&client_addr),
              sizeof(sockaddr)) < 0) {
    SPDLOG_ERROR("socket connect  fail. host:{} , port:{},",kServerIp,kServerPort);
    goto clean;
  }
  char recv_buf[1024] = {0};
  ret = send(client_socket, jstr.c_str(), static_cast<int>(jstr.size()), 0);
  if (ret < 0) {
    SPDLOG_ERROR("socket send  fail ,ret:{}", ret);
    goto clean;
  }
  ret = shutdown(client_socket, SD_SEND);
  if (ret == SOCKET_ERROR) {
    SPDLOG_ERROR("shutdown failed with erro:{}", ret);
    goto clean;
  }
  ret = recv(client_socket, recv_buf, sizeof(recv_buf), 0);
  if (ret < 0) {
    SPDLOG_ERROR("socket recv  fail ,ret:{}", ret);
    goto clean;
  }
clean:
  closesocket(client_socket);
  WSACleanup();
  return;
}

VOID SendHttpMsgCallback(PTP_CALLBACK_INSTANCE instance, PVOID context,
                         PTP_WORK Work) {
  common::InnerMessageStruct *msg = (common::InnerMessageStruct *)context;
  if (msg == NULL) {
    SPDLOG_INFO("http msg is null");
    return;
  }

  std::unique_ptr<common::InnerMessageStruct> sms(msg);
  nlohmann::json j_msg = nlohmann::json::parse(
      msg->buffer, msg->buffer + msg->length, nullptr, false);
  if (j_msg.is_discarded() == true) {
    return;
  }
  std::string jstr = j_msg.dump() + "\n";
  http::HttpClient::SendRequest(jstr);
}

void HandleSyncMsg(INT64 param1, INT64 param2, INT64 param3) {
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
  common::InnerMessageStruct *inner_msg = new common::InnerMessageStruct;
  inner_msg->buffer = new char[jstr.size() + 1];
  memcpy(inner_msg->buffer, jstr.c_str(), jstr.size() + 1);
  inner_msg->length = jstr.size();
  if (kEnableHttp) {
    bool add =
        base::ThreadPool::GetInstance().AddWork(SendHttpMsgCallback, inner_msg);
    SPDLOG_INFO("add http msg work:{}", add);
  } else {
    bool add =
        base::ThreadPool::GetInstance().AddWork(SendMsgCallback, inner_msg);
    SPDLOG_INFO("add msg work:{}", add);
  }
  RealDoAddMsg(param1, param2, param3);
}

void WechatHook::Init(WechatHookParam param)  {
    if(!init_){
        param_ = param;
        kEnableHttp = param.enable_http;
    }
}

int WechatHook::HookSyncMsg() {
  if (sync_msg_flag_) {
    SPDLOG_INFO("recv msg hook already called");
    return 2;
  }

  if (param_.server_ip.size() < 1) {
    return -2;
  }
  UINT64 base = wxhelper::wxutils::GetWeChatWinBase();
  if (!base) {
    SPDLOG_INFO("base addr is null");
    return -1;
  }
  if (param_.enable_http) {
    http::HttpClient::SetConfig(param_.http_url,param_.http_time_out);
  }else{
    const char* charPtr = param_.server_ip.c_str();
    std::strcpy(kServerIp, charPtr);
    kServerPort = param_.server_port;
  }
  DetourTransactionBegin();
  DetourUpdateThread(GetCurrentThread());
  DetourAttach(&(PVOID &)RealDoAddMsg, &HandleSyncMsg);
  LONG ret = DetourTransactionCommit();
  if (ret == NO_ERROR) {
    sync_msg_flag_ = true;
  }
  return ret;
}
int WechatHook::UnHookSyncMsg() {
  if (!sync_msg_flag_) {
    SPDLOG_INFO("call UnHookSyncMsg but no hooked ");
    return NO_ERROR;
  }
  UINT64 base = wxhelper::wxutils::GetWeChatWinBase();
  DetourTransactionBegin();
  DetourUpdateThread(GetCurrentThread());
  DetourDetach(&(PVOID &)RealDoAddMsg, &HandleSyncMsg);
  LONG ret = DetourTransactionCommit();
  if (ret == NO_ERROR) {
    sync_msg_flag_ = false;
    strcpy_s(kServerIp, "127.0.0.1");
  }
  return ret;
 }
 int WechatHook::HookLog() { return 0; }
 int WechatHook::UnHookLog() { return 0; }
 }  // namespace hook
