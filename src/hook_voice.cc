#include "pch.h"
#include "hook_voice.h"

#include "common.h"

using namespace std;

#define WX_HOOK_VOICE_OFFSET 0xd4d8d8
#define WX_HOOK_VOICE_NEXT_OFFSET 0x203d130
#define WX_SELF_ID_OFFSET 0x2FFD484

static wstring kVoiceStorePath = L"";
static int kVoiceHooked = FALSE;
static DWORD kWeChatWinBase = GetWeChatWinBase();
static char kOriginVoiceAsmCode[5] = {0};

static DWORD kHookVoiceNextAddress = kWeChatWinBase + WX_HOOK_VOICE_NEXT_OFFSET;
static DWORD kHookVoiceJmpBackAddress =
    kWeChatWinBase + WX_HOOK_VOICE_OFFSET + 0x5;

void OnHookVoice(DWORD buff,int len , DWORD msg_addr) {
  DWORD wxid_addr = GetWeChatWinBase() + WX_SELF_ID_OFFSET;
  string wxid = string(*(char **)wxid_addr, *(DWORD *)(wxid_addr + 0x10));
  wstring self_id = utf8_to_unicode(wxid.c_str());
  wstring save_path = kVoiceStorePath + self_id;
  if (!FindOrCreateDirectoryW(save_path.c_str())) {
    return;
  }
  unsigned long long msgid = *(unsigned long long *)(msg_addr + 0x30);
  save_path = save_path + L"\\" + to_wstring(msgid) + L".amr";
  HANDLE file_handle = CreateFileW(save_path.c_str(), GENERIC_ALL, 0, NULL,
                                   CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (file_handle == INVALID_HANDLE_VALUE) {
    return;
  }
  DWORD bytes_write = 0;
  WriteFile(file_handle, (LPCVOID)buff, len, &bytes_write, 0);
  CloseHandle(file_handle);
}

/// @brief  hook voice implement
_declspec(naked) void handle_voice() {
  __asm {
    PUSHAD
		PUSHFD
		PUSH        EDI
    PUSH        EDX
    PUSH        EAX
		CALL        OnHookVoice
		ADD         ESP, 0xC
		POPFD
		POPAD
		CALL        kHookVoiceNextAddress
		JMP         kHookVoiceJmpBackAddress
  }
}

int HookVoice(std::wstring save_path) {
  kWeChatWinBase = GetWeChatWinBase();
  if (!kWeChatWinBase) {
    return -1;
  }
  if (kVoiceHooked) {
    return 2;
  }
  kVoiceStorePath = save_path;
  if (kVoiceStorePath.back() != '\\') {
    kVoiceStorePath += L"\\";
  }
  wstring createpath = kVoiceStorePath.substr(0, kVoiceStorePath.length() - 1);
  if (!FindOrCreateDirectoryW(createpath.c_str())) {
    return -2;
  }
  DWORD hook_voice_addr = kWeChatWinBase + WX_HOOK_VOICE_OFFSET;
  kHookVoiceNextAddress = kWeChatWinBase + WX_HOOK_VOICE_NEXT_OFFSET;
  static DWORD kHookVoiceJmpBackAddress = hook_voice_addr + 0x5;
  HookAnyAddress(hook_voice_addr, (LPVOID)handle_voice, kOriginVoiceAsmCode);
  kVoiceHooked = TRUE;
  return 1;
}

int UnHookVoice() {
  if (!kVoiceHooked) return 1;
  DWORD hook_voice_addr = kWeChatWinBase + WX_HOOK_VOICE_OFFSET;
  UnHookAnyAddress(hook_voice_addr, kOriginVoiceAsmCode);
  kVoiceHooked = FALSE;
  return 1;
}
