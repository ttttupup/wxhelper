#include "pch.h"
#include "hook_img.h"

#include "common.h"
using namespace std;

// #define WX_HOOK_IMG_OFFSET 0xd7eaa5
// #define WX_HOOK_IMG_NEXT_OFFSET 0xda56e0
#define WX_HOOK_IMG_OFFSET 0xc672cc
#define WX_HOOK_IMG_NEXT_OFFSET 0xd82370
#define WX_SELF_ID_OFFSET 0x2C42A38
#define BUFSIZE 1024

#define JPEG0 0xFF
#define JPEG1 0xD8
#define JPEG2 0xFF
#define PNG0 0x89
#define PNG1 0x50
#define PNG2 0x4E
#define BMP0 0x42
#define BMP1 0x4D
#define GIF0 0x47
#define GIF1 0x49
#define GIF2 0x46



static wstring kImgStorePath = L"";
static int kImgHooked = FALSE;
static DWORD kWeChatWinBase = GetWeChatWinBase();
static char kOriginImgAsmCode[5] = {0};

static DWORD kHookImgNextAddress = kWeChatWinBase + WX_HOOK_IMG_NEXT_OFFSET;
static DWORD kHookImgJmpBackAddress = kWeChatWinBase + WX_HOOK_IMG_OFFSET + 0x5;

void OnHookImg(DWORD obj_addr) {
  DWORD wxid_addr = GetWeChatWinBase() + WX_SELF_ID_OFFSET;
  string wxid = string(*(char **)wxid_addr, *(DWORD *)(wxid_addr + 0x10));
  wstring self_id = String2Wstring(wxid);
  wstring save_path = kImgStorePath + self_id;
  if (!FindOrCreateDirectoryW(save_path.c_str())) {
    return;
  }
  wchar_t *origin_file_path = *(wchar_t **)obj_addr;
  wstring img_path(origin_file_path);
  if (img_path.find(L"_t.dat") != wstring::npos) {
    return;
  }

  int pos_begin = img_path.find_last_of(L"\\") + 1;
  int pos_end = img_path.find_last_of(L".");
  wstring file_name = img_path.substr(pos_begin, pos_end - pos_begin);
  char buffer[BUFSIZE] = {0};
  DWORD bytes_read = 0;
  DWORD bytes_write = 0;
  unsigned char magic_head[4] = {0};
  wchar_t suffix[5] = {0};

  HANDLE h_origin_file =
      CreateFileW(origin_file_path, GENERIC_READ, 0, NULL, OPEN_EXISTING,
                  FILE_ATTRIBUTE_NORMAL, NULL);
  if (h_origin_file == INVALID_HANDLE_VALUE) {
    return;
  }

  if (ReadFile(h_origin_file, buffer, BUFSIZE, &bytes_read, NULL)) {
    memcpy(magic_head, buffer, 3);
  }

  if (magic_head[0] == PNG0 && magic_head[1] == PNG1 && magic_head[2] == PNG2) {
    lstrcpyW(suffix, L".png");
  } else if (magic_head[0] == GIF0 && magic_head[1] == GIF1 &&
             magic_head[2] == GIF2) {
    lstrcpyW(suffix, L".gif");
  } else if (magic_head[0] == JPEG0 && magic_head[1] == JPEG1 &&
             magic_head[2] == JPEG2) {
    lstrcpyW(suffix, L".jpg");
  } else {
    lstrcpyW(suffix, L"");
  }

  wstring save_img_path = save_path + L"\\" + file_name + suffix;
  HANDLE save_file = CreateFileW(save_img_path.c_str(), GENERIC_ALL, 0, NULL,
                                 CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (save_file == INVALID_HANDLE_VALUE) {
    return;
  }
  if (!WriteFile(save_file, (LPCVOID)buffer, bytes_read, &bytes_write, 0)) {
    CloseHandle(h_origin_file);
    CloseHandle(save_file);
    return;
  }
  do {
    if (ReadFile(h_origin_file, buffer, BUFSIZE, &bytes_read, NULL)) {
      if (!WriteFile(save_file, (LPCVOID)buffer, bytes_read, &bytes_write, 0)) {
        CloseHandle(h_origin_file);
        CloseHandle(save_file);
        return;
      }
    }
  } while (bytes_read == BUFSIZE);
  CloseHandle(h_origin_file);
  CloseHandle(save_file);
}

/// @brief  hook img implement
_declspec(naked) void handle_img() {
  __asm {
        PUSHAD
		PUSHFD
		PUSH        ECX
		CALL        OnHookImg
		ADD         ESP, 0x4
		POPFD
		POPAD
		CALL        kHookImgNextAddress
		JMP         kHookImgJmpBackAddress
  }
}

/// @brief  hook image
/// @param save_path  image save dir
/// @return
int HookImg(wstring save_path) {
  kWeChatWinBase = GetWeChatWinBase();
  if (!kWeChatWinBase) {
    return -1;
  }
  if (kImgHooked) {
    return 2;
  }
  kImgStorePath = save_path;
  if (kImgStorePath.back() != '\\') {
    kImgStorePath += L"\\";
  }
  wstring createpath = kImgStorePath.substr(0, kImgStorePath.length() - 1);
  if (!FindOrCreateDirectoryW(createpath.c_str())) {
    return -2;
  }
  DWORD hook_img_addr = kWeChatWinBase + WX_HOOK_IMG_OFFSET;
  kHookImgNextAddress = kWeChatWinBase + WX_HOOK_IMG_NEXT_OFFSET;
  static DWORD kHookImgJmpBackAddress = hook_img_addr + 0x5;
  HookAnyAddress(hook_img_addr, (LPVOID)handle_img, kOriginImgAsmCode);
  kImgHooked = TRUE;
  return 1;
}

int UnHookImg() {
  if (!kImgHooked) return 1;
  DWORD hook_img_addr = kWeChatWinBase + WX_HOOK_IMG_OFFSET;
  UnHookAnyAddress(hook_img_addr, kOriginImgAsmCode);
  kImgHooked = FALSE;
  return 1;
}

int GetImgByName(wchar_t* file_path,wchar_t* save_dir) {
  wstring save_path(save_dir);
  wstring orign_file_path(file_path);
  if (!FindOrCreateDirectoryW(save_path.c_str())) {
    return 0;
  }

  int pos_begin = orign_file_path.find_last_of(L"\\") + 1;
  int pos_end = orign_file_path.find_last_of(L".");
  wstring file_name = orign_file_path.substr(pos_begin, pos_end - pos_begin);
  HANDLE h_origin_file =
      CreateFileW(file_path, GENERIC_READ, 0, NULL, OPEN_EXISTING,
                  FILE_ATTRIBUTE_NORMAL, NULL);
  char buffer[BUFSIZE] = {0};
  DWORD bytes_read = 0;
  DWORD bytes_write = 0;
  unsigned char magic_head[4] = {0};
  wstring suffix;
  short key = 0;
  if (ReadFile(h_origin_file, buffer, BUFSIZE, &bytes_read, NULL)) {
    memcpy(magic_head, buffer, 3);
  } else {
    CloseHandle(h_origin_file);
    return 0;
  }
  if ((magic_head[0] ^ JPEG0) == (magic_head[1] ^ JPEG1)) {
    key = magic_head[0] ^ JPEG0;
    suffix = L".jpg";
  } else if ((magic_head[0] ^ PNG1) == (magic_head[1] ^ PNG2)) {
    key = magic_head[0] ^ PNG1;
    suffix = L".png";
  } else if ((magic_head[0] ^ GIF0) == (magic_head[1] ^ GIF1)) {
    key = magic_head[0] ^ GIF0;
    suffix = L".gif";
  } else if ((magic_head[0] ^ BMP0) == (magic_head[1] ^ BMP1)) {
    key = magic_head[0] ^ BMP0;
    suffix = L".bmp";
  } else {
    key = -1;
    suffix = L".dat";
  }
  wstring save_img_path = save_path + L"\\" + file_name + suffix;
  HANDLE save_img = CreateFileW(save_img_path.c_str(), GENERIC_ALL, 0, NULL,
                                 CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (save_img == INVALID_HANDLE_VALUE) {
    return 0;
  }
  if (key > 0) {
    for (unsigned int i = 0; i < bytes_read; i++) {
      buffer[i]^=key;
    }
  }
  if (!WriteFile(save_img, (LPCVOID)buffer, bytes_read, &bytes_write, 0)) {
    CloseHandle(h_origin_file);
    CloseHandle(save_img);
    return 0;
  }

  do {
    if (ReadFile(h_origin_file, buffer, BUFSIZE, &bytes_read, NULL)) {
      if (key > 0) {
        for (unsigned int i = 0; i < bytes_read; i++) {
          buffer[i] ^= key;
        }
      }
      if (!WriteFile(save_img, (LPCVOID)buffer, bytes_read, &bytes_write, 0)) {
        CloseHandle(h_origin_file);
        CloseHandle(save_img);
        return 0;
      }
    }
  } while (bytes_read == BUFSIZE);
  CloseHandle(h_origin_file);
  CloseHandle(save_img);
  return 1;
}