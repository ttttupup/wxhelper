#include "pch.h"
#include "hook_log.h"

#include "common.h"

using namespace std;

#define WX_HOOK_LOG_OFFSET 0xed1675
#define WX_HOOK_LOG_NEXT_OFFSET 0x2344832

static int kLogHooked = FALSE;
static DWORD kWeChatWinBase = GetWeChatWinBase();
static char kOriginLogAsmCode[5] = {0};

static DWORD kHookLogAddress = kWeChatWinBase + WX_HOOK_LOG_OFFSET;
static DWORD kHookLogNextAddress = kWeChatWinBase + WX_HOOK_LOG_NEXT_OFFSET;
static DWORD kHookLogJmpBackAddress = kWeChatWinBase + WX_HOOK_LOG_OFFSET + 0x5;

void log_print(DWORD addr) {
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
  cout << ansi_message;
  delete[] ansi_message;
  ansi_message = NULL;
}

_declspec(naked) void handle_log() {
  __asm {
        PUSHAD
		PUSHFD
		PUSH        EAX
		CALL        log_print
		ADD         ESP, 0x4
		POPFD
		POPAD
		CALL        kHookLogNextAddress
		JMP         kHookLogJmpBackAddress
  }
}

int HookLog() {
  kWeChatWinBase = GetWeChatWinBase();
  if (!kWeChatWinBase) {
    return -1;
  }
  if (kLogHooked) {
    return 2;
  }
  kHookLogAddress = kWeChatWinBase + WX_HOOK_LOG_OFFSET;
  kHookLogNextAddress = kWeChatWinBase + WX_HOOK_LOG_NEXT_OFFSET;
  kHookLogJmpBackAddress = kHookLogAddress + 0x5;
  HookAnyAddress(kHookLogAddress, (LPVOID)handle_log, kOriginLogAsmCode);
  kLogHooked = TRUE;
  return 1;
}

int UnHookLog() {
  if (!kLogHooked) {
    return 1;
  }
  DWORD hook_img_addr = kWeChatWinBase + WX_HOOK_LOG_OFFSET;
  UnHookAnyAddress(hook_img_addr, kOriginLogAsmCode);
  kLogHooked = FALSE;
  return 1;
}