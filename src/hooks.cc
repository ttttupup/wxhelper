#include <Ws2tcpip.h>
#include <winsock2.h>

#include <nlohmann/json.hpp>

#include "easylogging++.h"
#include "pch.h"
#include "wechat_function.h"
using namespace nlohmann;
using namespace std;
namespace wxhelper {
namespace hooks {
static int server_port_ = 0;
static bool msg_hook_flag_ = false;
static char server_ip_[16] = "127.0.0.1";

static char msg_asm_code_[5] = {0};
static DWORD msg_back_addr_ = 0;
static DWORD msg_next_addr_ = 0;

static char sns_asm_code_[5] = {0};
static DWORD sns_back_addr_ = 0;
static DWORD sns_next_addr_ = 0;

static bool log_hook_flag_ = false;
static char log_asm_code_[5] = {0};
static DWORD log_back_addr_ = 0;
static DWORD log_next_addr_ = 0;

static bool search_contact_flag_ = false;
static char search_contact_asm_code_[5] = {0};
static DWORD search_contact_back_addr_ = 0;
static DWORD search_contact_next_addr_ = 0;

static bool error_code_flag_ = false;
static char error_code_asm_code_[5] = {0};
static DWORD error_code_back_addr_ = 0;
static DWORD error_code_next_addr_ = 0;

bool user_info_flag_ = false;
static char user_info_asm_code_[5] = {0};
static DWORD user_info_back_addr_ = 0;
static DWORD user_info_next_addr_ = 0;

UserInfo userinfo = {};

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

  if (server_port_ == 0) {
    LOG(INFO) << "http server port error :" << server_port_;
    return;
  }
  SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (client_socket < 0) {
    LOG(INFO) << "socket init  fail";
    return;
  }
  BOOL status = false;
  sockaddr_in client_addr;
  memset(&client_addr, 0, sizeof(client_addr));
  client_addr.sin_family = AF_INET;
  client_addr.sin_port = htons((u_short)server_port_);
  InetPtonA(AF_INET, server_ip_, &client_addr.sin_addr.s_addr);
  if (connect(client_socket, reinterpret_cast<sockaddr *>(&client_addr),
              sizeof(sockaddr)) < 0) {
    LOG(INFO) << "socket connect  fail";
    return;
  }
  char recv_buf[1024] = {0};
  int ret = send(client_socket, jstr.c_str(), jstr.size(), 0);
  if (ret == -1 || ret == 0) {
    LOG(INFO) << "socket send  fail ,ret:" << ret;
    closesocket(client_socket);
    return;
  }
  memset(recv_buf, 0, sizeof(recv_buf));
  ret = recv(client_socket, recv_buf, sizeof(recv_buf), 0);
  closesocket(client_socket);
  if (ret == -1 || ret == 0) {
    LOG(INFO) << "socket recv  fail ,ret:" << ret;
  }
}

void __cdecl OnRecvMsg(DWORD msg_addr) {
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
      Utils::WstringToUTF8(Utils::GetTimeW(*(DWORD *)(msg_addr + 0x44)));
  j_msg["timestamp"] = *(DWORD *)(msg_addr + 0x44);
  j_msg["fromGroup"] = Utils::WstringToUTF8(READ_WSTRING(msg_addr, 0x48));
  int length = *(DWORD *)(msg_addr + 0x178);
  if (length == 0) {
    j_msg["fromUser"] = j_msg["fromGroup"].get<std::string>();
  } else {
    j_msg["fromUser"] = Utils::WstringToUTF8(READ_WSTRING(msg_addr, 0x174));
  }
  int content_len = *(DWORD *)(msg_addr + 0x74);
  if (content_len > 0) {
    j_msg["content"] = Utils::WstringToUTF8(READ_WSTRING(msg_addr, 0x70));
  }
  int sign_len = *(DWORD *)(msg_addr + 0x18C);
  if (sign_len > 0) {
    j_msg["sign"] = Utils::WstringToUTF8(READ_WSTRING(msg_addr, 0x188));
  }
  int thumb_len = *(DWORD *)(msg_addr + 0x1A0);
  if (thumb_len > 0) {
    j_msg["thumbPath"] = Utils::WstringToUTF8(READ_WSTRING(msg_addr, 0x19C));
  }
  int path_len = *(DWORD *)(msg_addr + 0x1B4);
  if (path_len > 0) {
    j_msg["path"] = Utils::WstringToUTF8(READ_WSTRING(msg_addr, 0x1B0));
  }

  int signature_len = *(DWORD *)(msg_addr + 0x1F4);
  if (signature_len > 0) {
    j_msg["signature"] = Utils::WstringToUTF8(READ_WSTRING(msg_addr, 0x1F0));
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

/// @brief  hook msg implement
_declspec(naked) void HandleSyncMsg() {
  __asm {
    PUSHAD
	  PUSHFD
    PUSH    ECX
	  CALL    OnRecvMsg
	  ADD     ESP, 0x4
	  POPFD
	  POPAD
	  CALL    msg_next_addr_
	  JMP     msg_back_addr_
  }
}

void __cdecl OnSnsTimeLineMsg(DWORD msg_addr) {
  json j_sns;
  DWORD begin_addr = *(DWORD *)(msg_addr + 0x20);
  DWORD end_addr = *(DWORD *)(msg_addr + 0x24);
  if (begin_addr == 0) {
    j_sns = {{"data", json::array()}};
  } else {
    while (begin_addr < end_addr) {
      json j_item;
      j_item["snsId"] = *(unsigned long long *)(begin_addr);
      j_item["createTime"] = *(DWORD *)(begin_addr + 0x2C);

      j_item["senderId"] = Utils::WstringToUTF8(READ_WSTRING(begin_addr, 0x18));

      j_item["content"] = Utils::WstringToUTF8(READ_WSTRING(begin_addr, 0x3c));

      j_item["xml"] = Utils::WstringToUTF8(READ_WSTRING(begin_addr, 0x384));

      j_sns["data"].push_back(j_item);
      begin_addr += 0xB48;
    }
  }
  string jstr = j_sns.dump() + '\n';
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

/// @brief  hook sns msg implement
_declspec(naked) void HandleSNSMsg() {
  __asm {
    PUSHAD
		PUSHFD
    PUSH    [ESP + 0x24]
		CALL    OnSnsTimeLineMsg
		ADD     ESP, 0x4
    POPFD
		POPAD
		CALL    sns_next_addr_
		JMP     sns_back_addr_
  }
}

int HookRecvMsg(char *client_ip, int port) {
  server_port_ = port;
  strcpy_s(server_ip_, client_ip);
  DWORD base = Utils::GetWeChatWinBase();
  if (!base) {
    return -1;
  }

  if (msg_hook_flag_) {
    return 2;
  }

  DWORD hook_recv_msg_addr = base + WX_RECV_MSG_HOOK_OFFSET;
  msg_next_addr_ = base + WX_RECV_MSG_HOOK_NEXT_OFFSET;
  msg_back_addr_ = hook_recv_msg_addr + 0x5;
  LOG(INFO) << "base" << base;
  LOG(INFO) << "msg_next_addr_" << msg_next_addr_;
  LOG(INFO) << "msg_back_addr_" << msg_back_addr_;
  Utils::HookAnyAddress(hook_recv_msg_addr, (LPVOID)HandleSyncMsg,
                        msg_asm_code_);

  DWORD hook_sns_msg_addr = base + WX_SNS_HOOK_OFFSET;
  sns_next_addr_ = base + WX_SNS_HOOK_NEXT_OFFSET;
  sns_back_addr_ = hook_sns_msg_addr + 0x5;
  LOG(INFO) << "base" << base;
  LOG(INFO) << "sns_next_addr_" << sns_next_addr_;
  LOG(INFO) << "sns_back_addr_" << sns_back_addr_;
  Utils::HookAnyAddress(hook_sns_msg_addr, (LPVOID)HandleSNSMsg, sns_asm_code_);

  msg_hook_flag_ = true;
  return 1;
}

int UnHookRecvMsg() {
  server_port_ = 0;
  if (!msg_hook_flag_) {
    LOG(INFO) << "this port already hooked";
    return 2;
  }
  DWORD base = Utils::GetWeChatWinBase();
  DWORD hook_recv_msg_addr = base + WX_RECV_MSG_HOOK_OFFSET;
  DWORD hook_sns_addr = base + WX_SNS_HOOK_OFFSET;
  Utils::UnHookAnyAddress(hook_recv_msg_addr, msg_asm_code_);
  Utils::UnHookAnyAddress(hook_sns_addr, sns_asm_code_);
  msg_hook_flag_ = false;
  return 1;
}

void PrintLog(DWORD addr) {
  if (!addr) {
    return;
  }
  DWORD dwId = 0;
  char *msg = (char *)addr;
  int size = MultiByteToWideChar(CP_UTF8, 0, msg, -1, 0, 0);
  wchar_t *w_msg = new wchar_t[size + 1];
  memset(w_msg, 0, (size + 1) * 2);
  MultiByteToWideChar(CP_UTF8, 0, msg, -1, w_msg, size);
  size = WideCharToMultiByte(CP_ACP, 0, w_msg, -1, 0, 0, 0, 0);
  char *ansi_message = new char[size + 1];
  memset(ansi_message, 0, size + 1);
  WideCharToMultiByte(CP_ACP, 0, w_msg, -1, ansi_message, size, 0, 0);
  delete[] w_msg;
  w_msg = NULL;
  LOG(INFO) << ansi_message;
  delete[] ansi_message;
  ansi_message = NULL;
}

_declspec(naked) void HandleLog() {
  __asm {
    PUSHAD
		PUSHFD
		PUSH        EAX
		CALL        PrintLog
		ADD         ESP, 0x4
		POPFD
		POPAD
		CALL        log_next_addr_
		JMP         log_back_addr_
  }
}

int HookLog() {
  DWORD base = Utils::GetWeChatWinBase();
  if (!base) {
    return -1;
  }
  if (log_hook_flag_) {
    return 2;
  }
  DWORD hook_log_addr = base + WX_HOOK_LOG_OFFSET;
  log_next_addr_ = base + WX_HOOK_LOG_NEXT_OFFSET;
  log_back_addr_ = hook_log_addr + 0x5;
  Utils::HookAnyAddress(hook_log_addr, (LPVOID)HandleLog, log_asm_code_);
  log_hook_flag_ = true;
  return 1;
}
int UnHookLog() {
  if (!log_hook_flag_) {
    return 1;
  }
  DWORD base = Utils::GetWeChatWinBase();
  DWORD hook_log_addr = base + WX_HOOK_LOG_OFFSET;
  Utils::UnHookAnyAddress(hook_log_addr, log_asm_code_);
  log_hook_flag_ = FALSE;
  return 1;
}

void SetErrorCode(int code) { userinfo.error_code = code; }

void SetUserInfoDetail(DWORD address) {
  LOG(INFO) << "hook userinfo addr" <<&userinfo;
  DWORD length = *(DWORD *)(address + 0x8);
  userinfo.keyword = new wchar_t[length + 1];
  userinfo.keyword_len = length;
  if (length) {
    memcpy(userinfo.keyword, (wchar_t *)(*(DWORD *)(address + 0x4)),
           (length + 1) * sizeof(wchar_t));
  } else {
    ZeroMemory(userinfo.keyword, (length + 1) * sizeof(wchar_t));
  }

  length = *(DWORD *)(address + 0x1C);
  userinfo.v3 = new wchar_t[length + 1];
  userinfo.v3_len = length;
  if (length) {
    memcpy(userinfo.v3, (wchar_t *)(*(DWORD *)(address + 0x18)),
           (length + 1) * sizeof(wchar_t));
  } else {
    ZeroMemory(userinfo.v3, (length + 1) * sizeof(wchar_t));
  }

  length = *(DWORD *)(address + 0x30);
  userinfo.big_image = new wchar_t[length + 1];
  userinfo.big_image_len = length;
  if (length) {
    memcpy(userinfo.big_image, (wchar_t *)(*(DWORD *)(address + 0x2C)),
           (length + 1) * sizeof(wchar_t));
  } else {
    ZeroMemory(userinfo.big_image, (length + 1) * sizeof(wchar_t));
  }

  length = *(DWORD *)(address + 0xC8);
  userinfo.nickname = new wchar_t[length + 1];
  userinfo.nickname_len = length;
  if (length) {
    memcpy(userinfo.nickname, (wchar_t *)(*(DWORD *)(address + 0xC4)),
           (length + 1) * sizeof(wchar_t));
  } else {
    ZeroMemory(userinfo.nickname, (length + 1) * sizeof(wchar_t));
  }

  length = *(DWORD *)(address + 0x108);
  userinfo.v2 = new wchar_t[length + 1];
  userinfo.v2_len = length;
  if (length) {
    memcpy(userinfo.v2, (wchar_t *)(*(DWORD *)(address + 0x104)),
           (length + 1) * sizeof(wchar_t));
  } else {
    ZeroMemory(userinfo.v2, (length + 1) * sizeof(wchar_t));
  }

  length = *(DWORD *)(address + 0x16C);
  userinfo.small_image = new wchar_t[length + 1];
  userinfo.small_image_len = length;
  if (length) {
    memcpy(userinfo.small_image, (wchar_t *)(*(DWORD *)(address + 0x168)),
           (length + 1) * sizeof(wchar_t));
  } else {
    ZeroMemory(userinfo.small_image, (length + 1) * sizeof(wchar_t));
  }

  length = *(DWORD *)(address + 0x1F8);
  userinfo.signature = new wchar_t[length + 1];
  userinfo.signature_len = length;
  if (length) {
    memcpy(userinfo.signature, (wchar_t *)(*(DWORD *)(address + 0x1F4)),
           (length + 1) * sizeof(wchar_t));
  } else {
    ZeroMemory(userinfo.signature, (length + 1) * sizeof(wchar_t));
  }

  length = *(DWORD *)(address + 0x20C);
  userinfo.nation = new wchar_t[length + 1];
  userinfo.nation_len = length;
  if (length) {
    memcpy(userinfo.nation, (wchar_t *)(*(DWORD *)(address + 0x208)),
           (length + 1) * sizeof(wchar_t));
  } else {
    ZeroMemory(userinfo.nation, (length + 1) * sizeof(wchar_t));
  }

  length = *(DWORD *)(address + 0x220);
  userinfo.province = new wchar_t[length + 1];
  userinfo.province_len = length;
  if (length) {
    memcpy(userinfo.province, (wchar_t *)(*(DWORD *)(address + 0x21C)),
           (length + 1) * sizeof(wchar_t));
  } else {
    ZeroMemory(userinfo.province, (length + 1) * sizeof(wchar_t));
  }

  length = *(DWORD *)(address + 0x234);
  userinfo.city = new wchar_t[length + 1];
  userinfo.city_len = length;
  if (length) {
    memcpy(userinfo.city, (wchar_t *)(*(DWORD *)(address + 0x230)),
           (length + 1) * sizeof(wchar_t));
  } else {
    ZeroMemory(userinfo.city, (length + 1) * sizeof(wchar_t));
  }

  userinfo.sex = *(DWORD *)(address + 0x1BC);
  userinfo.over = true;
}

void DeleteUserInfoCache() {
  if (userinfo.keyword) {
    delete userinfo.keyword;
  }
  if (userinfo.v2) {
    delete userinfo.v2;
  }
  if (userinfo.v3) {
    delete userinfo.v3;
  }
  if (userinfo.nickname) {
    delete userinfo.nickname;
  }
  if (userinfo.nation) {
    delete userinfo.nation;
  }
  if (userinfo.province) {
    delete userinfo.province;
  }
  if (userinfo.city) {
    delete userinfo.city;
  }
  if (userinfo.signature) {
    delete userinfo.signature;
  }
  if (userinfo.small_image) {
    delete userinfo.small_image;
  }
  if (userinfo.big_image) {
    delete userinfo.big_image;
  }
  ZeroMemory(&userinfo, sizeof(UserInfo));
  userinfo.error_code = 1;
}

__declspec(naked) void HandleErrorCode() {
  __asm {
		PUSHAD;
		PUSHFD;
		PUSH ESI;
		CALL SetErrorCode;
		ADD ESP, 0x4;
		POPFD;
		POPAD;
		CALL error_code_next_addr_;
		JMP error_code_back_addr_;
  }
}

__declspec(naked) void HandleUserInfoDetail() {
  __asm {
		PUSHAD;
		PUSHFD;
		PUSH dword ptr [EBP + 0x14];
		CALL SetUserInfoDetail;
		ADD ESP, 0x4;
		POPFD;
		POPAD;
		CALL user_info_next_addr_;
		JMP user_info_back_addr_;
  }
}

int HookSearchContact() {
  DWORD base = Utils::GetWeChatWinBase();
  if (!base) {
    return -1;
  }
  if (search_contact_flag_) {
    return 2;
  }
  DWORD hook_error_code_addr = base + WX_SEARCH_CONTACT_ERROR_CODE_HOOK_OFFSET;
  error_code_next_addr_ = base + WX_SEARCH_CONTACT_ERROR_CODE_HOOK_NEXT_OFFSET;
  error_code_back_addr_ = hook_error_code_addr + 0x5;
  Utils::HookAnyAddress(hook_error_code_addr, (LPVOID)HandleErrorCode,
                        error_code_asm_code_);

  DWORD hook_user_info_addr = base + WX_SEARCH_CONTACT_DETAIL_HOOK_OFFSET;
  user_info_next_addr_ = base + WX_SEARCH_CONTACT_DETAIL_HOOK_NEXT_OFFSET;
  user_info_back_addr_ = hook_user_info_addr + 0x5;

  Utils::HookAnyAddress(hook_user_info_addr, (LPVOID)HandleUserInfoDetail,
                        user_info_asm_code_);
  error_code_flag_ = true;
  user_info_flag_ = true;
  return 1;
}

int UnHookSearchContact() {
  DWORD base = Utils::GetWeChatWinBase();
  DWORD hook_user_info_addr = base + WX_SEARCH_CONTACT_DETAIL_HOOK_OFFSET;
  DWORD hook_error_code_addr = base + WX_SEARCH_CONTACT_ERROR_CODE_HOOK_OFFSET;

  if (!user_info_flag_) return 2;
  Utils::UnHookAnyAddress(hook_user_info_addr, user_info_asm_code_);
  user_info_flag_ = false;

  if (!error_code_flag_) return 2;
  Utils::UnHookAnyAddress(hook_error_code_addr, error_code_asm_code_);
  error_code_flag_ = false;

  return 1;
}
}  // namespace hooks
}  // namespace wxhelper