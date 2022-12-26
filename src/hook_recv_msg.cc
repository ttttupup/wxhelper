#include "hook_recv_msg.h"

#include <Ws2tcpip.h>
#include <winsock2.h>

#include <nlohmann/json.hpp>

#include "common.h"
#include "pch.h"
using namespace nlohmann;

using namespace std;
#define WX_RECV_MSG_HOOK_OFFSET 0xb97126
#define WX_RECV_MSG_HOOK_NEXT_OFFSET 0x6fc850

// SyncMgr::addMsgListToDB
// #define WX_RECV_MSG_HOOK_OFFSET 0xB9C919
// #define WX_RECV_MSG_HOOK_NEXT_OFFSET 0x722FF0

#define CLIENT_IP "127.0.0.1"
static int kServerPort = 0;
static int kMessageHooked = FALSE;
static DWORD kWeChatWinBase = GetWeChatWinBase();

static char kOriginReceMsgAsmCode[5] = {0};
static DWORD kReceMsgJmpBackAddress =
    kWeChatWinBase + WX_RECV_MSG_HOOK_OFFSET + 0x5;
static DWORD kReceMsgNextAddress =
    kWeChatWinBase + WX_RECV_MSG_HOOK_NEXT_OFFSET;

struct InnerMessageStruct {
  char *buffer;
  int length;
  ~InnerMessageStruct() {
    if (this->buffer != NULL) {
      delete[] this->buffer;
      this->buffer = NULL;
    }
  }
};
/// @brief send message  by socket
/// @param buffer content
/// @param len len
/// @return true/false
BOOL SendBySocket(const char *buffer, size_t len) {
  if (kServerPort == 0) {
    return false;
  }
  SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (client_socket < 0) {
#ifdef _DEBUG
    cout << "create socket error,"
         << " errno:" << errno << endl;
#endif
    return false;
  }
  BOOL status = false;
  sockaddr_in client_addr;
  memset(&client_addr, 0, sizeof(client_addr));
  client_addr.sin_family = AF_INET;
  client_addr.sin_port = htons((u_short)kServerPort);
  InetPtonA(AF_INET, CLIENT_IP, &client_addr.sin_addr.s_addr);
  if (connect(client_socket, reinterpret_cast<sockaddr *>(&client_addr),
              sizeof(sockaddr)) < 0) {
#ifdef _DEBUG
    cout << "connect error,"
         << " errno:" << errno << endl;
#endif
    return false;
  }
  char recv_buf[1024] = {0};
  int ret = send(client_socket, buffer, len, 0);
  if (ret == -1 || ret == 0) {
#ifdef _DEBUG
    cout << "send fail,"
         << " errno:" << errno << endl;
#endif
    closesocket(client_socket);
    return false;
  }
  memset(recv_buf, 0, sizeof(recv_buf));
  ret = recv(client_socket, recv_buf, sizeof(recv_buf), 0);
  closesocket(client_socket);
  if (ret == -1 || ret == 0) {
#ifdef _DEBUG
    cout << "the server close" << endl;
#endif
    return false;
  }
  return true;
}
/// @brief send wrap
/// @param msg msg
void SendSocketMessage(InnerMessageStruct *msg) {
  if (msg == NULL) {
    return;
  }
  unique_ptr<InnerMessageStruct> sms(msg);
  json j_msg =
      json::parse(msg->buffer, msg->buffer + msg->length, nullptr, false);
  if (j_msg.is_discarded() == true) {
    return;
  }
  string jstr = j_msg.dump() + "\n";
#ifdef _DEBUG
  cout << "json:" + jstr << endl;
#endif
  SendBySocket(jstr.c_str(), jstr.size());
}
/// @brief msg handle
/// @param msg_addr msg address in memory
void OnRecvMsg(DWORD msg_addr) {
  json j_msg;
  unsigned long long msgid = *(unsigned long long *)(msg_addr + 0x30);
  j_msg["msgId"] = msgid;
  j_msg["pid"] = GetCurrentProcessId();
  j_msg["type"] = *(DWORD *)(msg_addr + 0x38);
  j_msg["isSendMsg"] = *(BOOL *)(msg_addr + 0x3C);
  if (j_msg["isSendMsg"].get<BOOL>()) {
    j_msg["isSendByPhone"] = (int)(*(BYTE *)(msg_addr + 0xD8));
  }
  j_msg["time"] =
      unicode_to_utf8((wchar_t *)GetTimeW(*(DWORD *)(msg_addr + 0x44)).c_str());
  j_msg["timestamp"] = *(DWORD *)(msg_addr + 0x44);
  j_msg["fromGroup"] =
      unicode_to_utf8((wchar_t *)READ_WSTRING(msg_addr, 0x48).c_str());
  int length = *(DWORD *)(msg_addr + 0x178);
  if (length == 0) {
    j_msg["fromUser"] = j_msg["fromGroup"].get<std::string>();
  } else {
    j_msg["fromUser"] =
        unicode_to_utf8((wchar_t *)READ_WSTRING(msg_addr, 0x174).c_str());
  }
  int content_len = *(DWORD *)(msg_addr + 0x74);
  if (content_len > 0) {
    j_msg["content"] =
        unicode_to_utf8((wchar_t *)READ_WSTRING(msg_addr, 0x70).c_str());
#ifdef _DEBUG
    printf("%s", j_msg["content"].get<std::string>().c_str());
#endif
  }
  int sign_len = *(DWORD *)(msg_addr + 0x18C);
  if (sign_len > 0) {
    j_msg["sign"] =
        unicode_to_utf8((wchar_t *)READ_WSTRING(msg_addr, 0x188).c_str());
  }
  int thumb_len = *(DWORD *)(msg_addr + 0x1A0);
  if (thumb_len > 0) {
    j_msg["thumbPath"] =
        unicode_to_utf8((wchar_t *)READ_WSTRING(msg_addr, 0x19C).c_str());
  }
  int path_len = *(DWORD *)(msg_addr + 0x1B4);
  if (path_len > 0) {
    j_msg["path"] =
        unicode_to_utf8((wchar_t *)READ_WSTRING(msg_addr, 0x1B0).c_str());
  }

  int signature_len = *(DWORD *)(msg_addr + 0x1F4);
  if (signature_len > 0) {
    j_msg["signature"] =
        unicode_to_utf8((wchar_t *)READ_WSTRING(msg_addr, 0x1F0).c_str());
  }

  string jstr = j_msg.dump() + '\n';
  InnerMessageStruct *inner_msg = new InnerMessageStruct;
  inner_msg->buffer = new char[jstr.size() + 1];
  memcpy(inner_msg->buffer, jstr.c_str(), jstr.size() + 1);
  inner_msg->length = jstr.size();
  HANDLE thread = CreateThread(
      NULL, 0, (LPTHREAD_START_ROUTINE)SendSocketMessage, inner_msg, NULL, 0);
  if (thread) {
    CloseHandle(thread);
  }
}
/// @brief  hook implement
_declspec(naked) void handle_sync_msg() {
  __asm {
    PUSHAD
		PUSHFD
		PUSH    ECX
		CALL    OnRecvMsg
		ADD     ESP, 0x4
		POPFD
		POPAD
		CALL    kReceMsgNextAddress
		JMP     kReceMsgJmpBackAddress
  }
}

/// @brief hook any address   address+0x5
/// @param port 端口
/// @return 成功返回1,已经hook返回2,失败返回-1
int HookRecvMsg(int port) {
  kServerPort = port;
  kWeChatWinBase = GetWeChatWinBase();
  if (!kWeChatWinBase) {
    return -1;
  }

  if (kMessageHooked) {
    return 2;
  }
  kWeChatWinBase = GetWeChatWinBase();
  DWORD hook_recv_msg_addr = kWeChatWinBase + WX_RECV_MSG_HOOK_OFFSET;
  kReceMsgNextAddress = kWeChatWinBase + WX_RECV_MSG_HOOK_NEXT_OFFSET;
  kReceMsgJmpBackAddress = hook_recv_msg_addr + 0x5;
  HookAnyAddress(hook_recv_msg_addr, (LPVOID)handle_sync_msg,
                 kOriginReceMsgAsmCode);
  kMessageHooked = TRUE;
  return 1;
}

int UnHookRecvMsg() {
  kServerPort = 0;
  if (!kMessageHooked) return 2;
  DWORD hook_recv_msg_addr = kWeChatWinBase + WX_RECV_MSG_HOOK_OFFSET;
  UnHookAnyAddress(hook_recv_msg_addr, kOriginReceMsgAsmCode);
  kMessageHooked = FALSE;
  return 1;
}