#include "pch.h"
#include "search_contact.h"

#include "common.h"

#include "wechat_data.h"

#define WX_SEARCH_CONTACT_ERROR_CODE_HOOK_OFFSeT 0xd94d1e
#define WX_SEARCH_CONTACT_ERROR_CODE_HOOK_NEXT_OFFSeT 0xed13a0
#define WX_SEARCH_CONTACT_DETAIL_HOOK_OFFSeT 0xa2a260
#define WX_SEARCH_CONTACT_DETAIL_HOOK_NEXT_OFFSeT 0xa2a4c0


#define WX_SEARCH_CONTACT_OFFSeT 0xc5bb00
#define WX_SEARCH_CONTACT_MGR_OFFSeT 0xa0d550



static BOOL kSearchContactHooked = false;
static char kHookSearchContactErrcodeOldAsm[5] = {0};
static char kHookUserInfoOldAsm[5] = {0};
static DWORD kWeChatWinBase = GetWeChatWinBase();

static DWORD kHookSearchContactErrcodeNextAddr =
    kWeChatWinBase + WX_SEARCH_CONTACT_ERROR_CODE_HOOK_NEXT_OFFSeT;
static DWORD kHookSearchContactErrcodeAddr =
    kWeChatWinBase + WX_SEARCH_CONTACT_ERROR_CODE_HOOK_OFFSeT;
static DWORD kHookSearchContactErrcodeJmpBackAddr =
    kHookSearchContactErrcodeAddr + 0x5;

static DWORD kHookUserInfoNextAddr =
    kWeChatWinBase + WX_SEARCH_CONTACT_DETAIL_HOOK_NEXT_OFFSeT;
static DWORD kHookUserInfoAddr =
    kWeChatWinBase + WX_SEARCH_CONTACT_DETAIL_HOOK_OFFSeT;
static DWORD kHookUserInfoJmpBackAddr = kHookUserInfoAddr + 0x5;

static UserInfo userinfo;

void SetErrorCode(int code) { userinfo.error_code = code; }

void SetUserInfoDetail(DWORD address) {
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
  }else {
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
  }else {
    ZeroMemory(userinfo.nickname, (length + 1) * sizeof(wchar_t));
  }

  length = *(DWORD *)(address + 0x108);
  userinfo.v2 = new wchar_t[length + 1];
  userinfo.v2_len = length;
  if (length) {
    memcpy(userinfo.v2, (wchar_t *)(*(DWORD *)(address + 0x104)),
           (length + 1) * sizeof(wchar_t));
  }else {
    ZeroMemory(userinfo.v2, (length + 1) * sizeof(wchar_t));
  }

  length = *(DWORD *)(address + 0x16C);
  userinfo.small_image = new wchar_t[length + 1];
  userinfo.small_image_len = length;
  if (length) {
    memcpy(userinfo.small_image, (wchar_t *)(*(DWORD *)(address + 0x168)),
           (length + 1) * sizeof(wchar_t));
  }else {
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

static void DeleteUserInfoCache() {
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
		pushad;
		pushfd;
		push edi;
		call SetErrorCode;
		add esp, 0x4;
		popfd;
		popad;
		call kHookSearchContactErrcodeNextAddr;
		jmp kHookSearchContactErrcodeJmpBackAddr;
  }
}

__declspec(naked) void HandleUserInfoDetail() {
  __asm {
		pushad;
		pushfd;
		push dword ptr [ebp + 0x14];
		call SetUserInfoDetail;
		add esp, 0x4;
		popfd;
		popad;
		call kHookUserInfoNextAddr;
		jmp kHookUserInfoJmpBackAddr;
  }
}

int SearchContactNetScene(wchar_t *keyword,UserInfo ** user) {
  int success = -1;
  HookSearchContact();
  DeleteUserInfoCache();
  DWORD base = GetWeChatWinBase();
  DWORD search_contact_mgr_addr = base + WX_SEARCH_CONTACT_MGR_OFFSeT;
  DWORD search_contact_addr = base + WX_SEARCH_CONTACT_OFFSeT;
  WeChatString key(keyword);

  __asm {
		pushad;
		pushfd;
		call search_contact_mgr_addr;
		lea ebx, key;
		push ebx;
		mov ecx, eax;
		call search_contact_addr;
		popfd;
		popad;
  }
  success = 1;
  while (userinfo.error_code == 1 && kSearchContactHooked) {
    Sleep(20);
  }
  if (userinfo.error_code == 0) {
    while (userinfo.over == false && kSearchContactHooked) {
      Sleep(20);
    }
  }
  *user= &userinfo;
  return success;
}

int HookSearchContact() {
  kWeChatWinBase = GetWeChatWinBase();
  if (!kWeChatWinBase) {
    return -1;
  }
  if (kSearchContactHooked) {
    return 2;
  }
  kHookSearchContactErrcodeNextAddr =
      kWeChatWinBase + WX_SEARCH_CONTACT_ERROR_CODE_HOOK_NEXT_OFFSeT;
  kHookSearchContactErrcodeAddr =
      kWeChatWinBase + WX_SEARCH_CONTACT_ERROR_CODE_HOOK_OFFSeT;
  kHookSearchContactErrcodeJmpBackAddr = kHookSearchContactErrcodeAddr + 0x5;

  kHookUserInfoNextAddr =
      kWeChatWinBase + WX_SEARCH_CONTACT_DETAIL_HOOK_NEXT_OFFSeT;
  kHookUserInfoAddr = kWeChatWinBase + WX_SEARCH_CONTACT_DETAIL_HOOK_OFFSeT;
  kHookUserInfoJmpBackAddr = kHookUserInfoAddr + 0x5;
  HookAnyAddress(kHookSearchContactErrcodeAddr,
                 (LPVOID)HandleErrorCode,
                 kHookSearchContactErrcodeOldAsm);
  HookAnyAddress(kHookUserInfoAddr, (LPVOID)HandleUserInfoDetail, kHookUserInfoOldAsm);
  kSearchContactHooked = true;
  return 1;
}

int UnHookSearchContact() {
  if (!kSearchContactHooked) return 2;
  UnHookAnyAddress(kHookSearchContactErrcodeAddr,
                   kHookSearchContactErrcodeOldAsm);
  UnHookAnyAddress(kHookUserInfoAddr, kHookUserInfoOldAsm);
  kSearchContactHooked = false;
  return 1;
}