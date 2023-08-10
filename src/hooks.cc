
#include "pch.h"
#include "hooks.h"
#include "thread_pool.h"
#include "wechat_function.h"
#include <WS2tcpip.h>
#include "base64.h"
#include "http_client.h"

namespace offset = wxhelper::V3_9_5_81::offset;
namespace common = wxhelper::common;
namespace wxhelper {
namespace hooks {

static int kServerPort = 19099;
static bool kMsgHookFlag = false;
static char kServerIp[16] = "127.0.0.1";
static bool kEnableHttp = false;
static bool kLogHookFlag = false;

static bool kSnsFinishHookFlag = false;



static UINT64 (*R_DoAddMsg)(UINT64, UINT64, UINT64) = (UINT64(*)(
    UINT64, UINT64, UINT64))(Utils::GetWeChatWinBase() + offset::kDoAddMsg);

static UINT64 (*R_Log)(UINT64, UINT64, UINT64, UINT64, UINT64, UINT64, UINT64,
                       UINT64, UINT64, UINT64, UINT64, UINT64) =
    (UINT64(*)(UINT64, UINT64, UINT64, UINT64, UINT64, UINT64, UINT64, UINT64,
               UINT64, UINT64, UINT64,
               UINT64))(Utils::GetWeChatWinBase() + offset::kHookLog);

static UINT64 (*R_OnSnsTimeLineSceneFinish)(UINT64, UINT64, UINT64) =
    (UINT64(*)(UINT64, UINT64, UINT64))(Utils::GetWeChatWinBase() +
                                        offset::kOnSnsTimeLineSceneFinish);

VOID CALLBACK SendMsgCallback(PTP_CALLBACK_INSTANCE instance, PVOID context,
                             PTP_WORK Work) {
  common::InnerMessageStruct *msg = (common::InnerMessageStruct *)context;
  if (msg == NULL) {
    SPDLOG_INFO("add work:msg is null");
    return;
  }
  std::unique_ptr<common::InnerMessageStruct> sms(msg);
  nlohmann::json j_msg =
      nlohmann::json::parse(msg->buffer, msg->buffer + msg->length, nullptr, false);
  if (j_msg.is_discarded() == true) {
    return;
  }
  std::string jstr = j_msg.dump() + "\n";

  if (kServerPort == 0) {
    SPDLOG_ERROR("http server port error :{}", kServerPort);
    return;
  }
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
    SPDLOG_ERROR("socket connect  fail");
    goto clean;
  }
  char recv_buf[1024] = {0};
  ret = send(client_socket, jstr.c_str(), static_cast<int>(jstr.size()) , 0);
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

VOID CALLBACK SendHttpMsgCallback(PTP_CALLBACK_INSTANCE instance, PVOID context,
                                  PTP_WORK Work) {
  common::InnerMessageStruct *msg = (common::InnerMessageStruct *)context;
  if (msg == NULL) {
    SPDLOG_INFO("http msg is null");
    return;
  }

  std::unique_ptr<common::InnerMessageStruct> sms(msg);
  nlohmann::json j_msg =
      nlohmann::json::parse(msg->buffer, msg->buffer + msg->length, nullptr, false);
  if (j_msg.is_discarded() == true) {
    return;
  }
  std::string jstr = j_msg.dump() + "\n";
  HttpClient::GetInstance().SendRequest(jstr);
}

void HandleSyncMsg(INT64 param1, INT64 param2, INT64 param3) {
  nlohmann::json  msg;

  msg["pid"] = GetCurrentProcessId();
  msg["fromUser"] = Utils::ReadSKBuiltinString(*(INT64 *)(param2 + 0x18));
  msg["toUser"] = Utils::ReadSKBuiltinString(*(INT64 *)(param2 + 0x28));
  msg["content"] = Utils::ReadSKBuiltinString(*(INT64 *)(param2 + 0x30));
  msg["signature"] = Utils::ReadWeChatStr(*(INT64 *)(param2 + 0x48));
  msg["msgId"] = *(INT64 *)(param2 + 0x60);
  msg["msgSequence"] = *(DWORD *)(param2 + 0x5C);
  msg["createTime"] = *(DWORD *)(param2 + 0x58);
  msg["displayFullContent"] = Utils::ReadWeChatStr(*(INT64 *)(param2 + 0x50));
  DWORD type = *(DWORD *)(param2 + 0x24);
  msg["type"] = type;
  if (type == 3) {
    int a = 1;
    std::string img =
        Utils::ReadSKBuiltinBuffer(*(INT64 *)(param2 + 0x40));
    SPDLOG_INFO("encode size:{}",img.size());
    msg["base64Img"] = base64_encode(img);
    a = 2;
  }
  std::string jstr = msg.dump() + '\n';
  common::InnerMessageStruct *inner_msg = new common::InnerMessageStruct;
  inner_msg->buffer = new char[jstr.size() + 1];
  memcpy(inner_msg->buffer, jstr.c_str(), jstr.size() + 1);
  inner_msg->length = jstr.size();
  if(kEnableHttp){
    bool add = ThreadPool::GetInstance().AddWork(SendHttpMsgCallback,inner_msg);
    SPDLOG_INFO("add http msg work:{}",add);
  }else{
    bool add = ThreadPool::GetInstance().AddWork(SendMsgCallback,inner_msg);
    SPDLOG_INFO("add msg work:{}",add);
  }
  R_DoAddMsg(param1,param2,param3);
}

UINT64 HandlePrintLog(UINT64 param1, UINT64 param2, UINT64 param3, UINT64 param4,
                    UINT64 param5, UINT64 param6, UINT64 param7, UINT64 param8,
                    UINT64 param9, UINT64 param10, UINT64 param11,
                    UINT64 param12) {
 UINT64 p = R_Log(param1, param2, param3, param4, param5, param6, param7, param8, param9,
        param10, param11, param12);
if(p== 0 || p == 1){
  return p;
}
 char *msg = (char *)p;
 if (msg != NULL) {
  // INT64 size = *(INT64 *)(p - 0x8);
  std::string str(msg);
  std::wstring ws = Utils::UTF8ToWstring(str);
  std::string out = Utils::WstringToAnsi(ws, CP_ACP);
  spdlog::info("wechat log:{}", out);
 }
 return p;
}

void HandleSNSMsg(INT64 param1, INT64 param2, INT64 param3) {
 nlohmann::json j_sns;
 INT64 begin_addr = *(INT64 *)(param2 + 0x30);
 INT64 end_addr = *(INT64 *)(param2 + 0x38);
 if (begin_addr == 0) {
  j_sns = {{"data", nlohmann::json::array()}};
 } else {
  while (begin_addr < end_addr) {
    nlohmann::json j_item;
    j_item["snsId"] = *(UINT64 *)(begin_addr);
    j_item["createTime"] = *(DWORD *)(begin_addr + 0x38);
    j_item["senderId"] = Utils::ReadWstringThenConvert(begin_addr + 0x18);
    j_item["content"] = Utils::ReadWstringThenConvert(begin_addr + 0x48);
    j_item["xml"] = Utils::ReadWstringThenConvert(begin_addr + 0x580);
    j_sns["data"].push_back(j_item);
    begin_addr += 0x11E0;
  }
 }
 std::string jstr = j_sns.dump() + '\n';
 common::InnerMessageStruct *inner_msg = new common::InnerMessageStruct;
 inner_msg->buffer = new char[jstr.size() + 1];
 memcpy(inner_msg->buffer, jstr.c_str(), jstr.size() + 1);
 inner_msg->length =  jstr.size();
 if (kEnableHttp) {
  bool add = ThreadPool::GetInstance().AddWork(SendHttpMsgCallback, inner_msg);
  SPDLOG_INFO("hook sns add http msg work:{}", add);
 } else {
  bool add = ThreadPool::GetInstance().AddWork(SendMsgCallback, inner_msg);
  SPDLOG_INFO("hook sns add msg work:{}", add);
 }
 R_OnSnsTimeLineSceneFinish(param1, param2, param3);
}

int HookSyncMsg(std::string client_ip, int port, std::string url,
                uint64_t timeout, bool enable) {
  if (kMsgHookFlag) {
    SPDLOG_INFO("recv msg hook already called");
    return 2;
  }
  kEnableHttp = enable;
  if (kEnableHttp) {
    HttpClient::GetInstance().SetConfig(url, timeout);
  }
  if (client_ip.size() < 1) {
    return -2;
  }

  kServerPort = port;
  strcpy_s(kServerIp, client_ip.c_str());
  UINT64 base = Utils::GetWeChatWinBase();
  if (!base) {
    SPDLOG_INFO("base addr is null");
    return -1;
  }

  // DetourRestoreAfterWith();
  DetourTransactionBegin();
  DetourUpdateThread(GetCurrentThread());
  DetourAttach(&(PVOID&)R_DoAddMsg, &HandleSyncMsg);
  LONG ret = DetourTransactionCommit();
  if(ret == NO_ERROR){
    kMsgHookFlag = true;
  }
  SPDLOG_INFO("hook sync {}",ret);
  DetourTransactionBegin();
  DetourUpdateThread(GetCurrentThread());
  DetourAttach(&(PVOID&)R_OnSnsTimeLineSceneFinish, &HandleSNSMsg);
  ret = DetourTransactionCommit();
  if(ret == NO_ERROR){
    kSnsFinishHookFlag = true;
  }
  SPDLOG_INFO("hook sns {}",ret);
  return ret;
}

int UnHookSyncMsg() {
  if (!kMsgHookFlag) {
    kMsgHookFlag = false;
    kEnableHttp = false;
    strcpy_s(kServerIp, "127.0.0.1");
    SPDLOG_INFO("hook sync msg reset");
    return NO_ERROR;
  }
  UINT64 base = Utils::GetWeChatWinBase();
  DetourTransactionBegin();
  DetourUpdateThread(GetCurrentThread());
  DetourDetach(&(PVOID&)R_DoAddMsg, &HandleSyncMsg);
  LONG ret = DetourTransactionCommit();
  if (ret == NO_ERROR) {
    kMsgHookFlag = false;
    kEnableHttp = false;
    strcpy_s(kServerIp, "127.0.0.1");
  }
  return ret;
 }

 int HookLog() {
  if (kLogHookFlag) {
    SPDLOG_INFO("log hook already called");
    return 2;
  }
 
  UINT64 base = Utils::GetWeChatWinBase();
  if (!base) {
    SPDLOG_INFO("base addr is null");
    return -1;
  }

  // DetourRestoreAfterWith();
  DetourTransactionBegin();
  DetourUpdateThread(GetCurrentThread());
  UINT64 do_add_msg_addr = base + offset::kHookLog;
  DetourAttach(&(PVOID &)R_Log, &HandlePrintLog);
  LONG ret = DetourTransactionCommit();
  if (ret == NO_ERROR) {
    kLogHookFlag = true;
  }
  return ret;
 }

 int UnHookLog() {
  if (!kLogHookFlag) {
    kLogHookFlag = false;
    SPDLOG_INFO("hook log reset");
    return NO_ERROR;
  }
  UINT64 base = Utils::GetWeChatWinBase();
  DetourTransactionBegin();
  DetourUpdateThread(GetCurrentThread());
  UINT64 do_add_msg_addr = base + offset::kHookLog;
  DetourDetach(&(PVOID &)R_Log, &HandlePrintLog);
  LONG ret = DetourTransactionCommit();
  if (ret == NO_ERROR) {
    kLogHookFlag = false;
  }
  return ret;
 }

}  // namespace hooks
}  // namespace wxhelper