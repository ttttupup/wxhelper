#include "include/utils.h"

#include <winternl.h>

#include <fstream>

#include "utils.h"
namespace base {
namespace utils {
const std::string hex_table = "0123456789abcdef";
std::wstring Utf8ToWstring(const std::string &str) {
  return AnsiToWstring(str, CP_UTF8);
}

std::string WstringToUtf8(const std::wstring &str) {
  return WstringToAnsi(str, CP_UTF8);
}

std::wstring AnsiToWstring(const std::string &input, INT64 locale) {
  int wchar_len = MultiByteToWideChar(locale, 0, input.c_str(), -1, NULL, 0);
  if (wchar_len > 0) {
    std::vector<wchar_t> temp(wchar_len);
    MultiByteToWideChar(CP_UTF8, 0, input.c_str(), -1, &temp[0], wchar_len);
    return std::wstring(&temp[0]);
  }
  return std::wstring();
}

std::string WstringToAnsi(const std::wstring &input, INT64 locale) {
  int char_len = WideCharToMultiByte(locale, 0, input.c_str(), -1, 0, 0, 0, 0);
  if (char_len > 0) {
    std::vector<char> temp(char_len);
    WideCharToMultiByte(locale, 0, input.c_str(), -1, &temp[0], char_len, 0, 0);
    return std::string(&temp[0]);
  }
  return std::string();
}

std::string WcharToUtf8(wchar_t *wstr) {
  int c_size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, FALSE);
  if (c_size > 0) {
    char *buffer = new char[c_size];
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, buffer, c_size, NULL, FALSE);
    std::string str(buffer);
    delete[] buffer;
    buffer = NULL;
    return str;
  }
  return std::string();
}

std::string StringToHex(const std::string &str) {
  std::string sb;
  for (int i = 0; i < str.length(); i++) {
    sb += hex_table.at((str[i] & 0xf0) >> 4);
    sb += hex_table.at((str[i] & 0x0f) >> 0);
  }
  return sb;
}

std::string HexToString(const std::string &hex_str) {
  std::string ret;
  for (int i = 0; i < hex_str.length(); i += 2) {
    ret += BYTE(hex_table.find(hex_str.at(i)) << 4 |
                hex_table.find(hex_str.at(i + 1)));
  }
  return ret;
}

std::string BytesToHex(const BYTE *bytes, const int length) {
  if (bytes == NULL) {
    return "";
  }
  std::string buff;
  const int len = length;
  for (int j = 0; j < len; j++) {
    int high = bytes[j] / 16, low = bytes[j] % 16;
    buff += (high < 10) ? ('0' + high) : ('a' + high - 10);
    buff += (low < 10) ? ('0' + low) : ('a' + low - 10);
  }
  return buff;
}
void HexToBytes(const std::string &hex, BYTE *bytes) {
  int byte_len = hex.length() / 2;
  std::string str;
  unsigned int n;
  for (int i = 0; i < byte_len; i++) {
    str = hex.substr(i * 2, 2);
    sscanf_s(str.c_str(), "%x", &n);
    bytes[i] = n;
  }
}

bool CreateConsole() {
  if (AllocConsole()) {
    AttachConsole(GetCurrentProcessId());
    FILE *retStream;
    freopen_s(&retStream, "CONOUT$", "w", stdout);
    if (!retStream) throw std::runtime_error("Stdout redirection failed.");
    freopen_s(&retStream, "CONOUT$", "w", stderr);
    if (!retStream) throw std::runtime_error("Stderr redirection failed.");
    return false;
  }
  return true;
}

void CloseConsole() {
  fclose(stdin);
  fclose(stdout);
  fclose(stderr);
  FreeConsole();
}

void HideModule(HMODULE module) {
#ifdef _WIN64
  PPEB peb = (PPEB)__readgsqword(0x60);
  PPEB_LDR_DATA ldr = peb->Ldr;

  void *cur_ptr = *((void **)((unsigned char *)ldr + 0x18));
  void *next_ptr = cur_ptr;
  do {
    void *next = *((void **)((unsigned char *)next_ptr));
    void *last = *((void **)((unsigned char *)next_ptr + 0x8));
    void *base_addr = *((void **)((unsigned char *)next_ptr + 0x30));
    if (base_addr == module) {
      *((void **)((unsigned char *)last)) = next;
      *((void **)((unsigned char *)next + 0x8)) = last;
      cur_ptr = next;
    }
    next_ptr = *((void **)next_ptr);
  } while (cur_ptr != next_ptr);
#else
  void *peb_ptr = nullptr;
  _asm {
		PUSH EAX
		MOV  EAX, FS:[0x30]
		MOV  peb_ptr, EAX
		POP  EAX
  }
  void *ldr_ptr = *((void **)((unsigned char *)peb_ptr + 0xc));
  void *cur_ptr = *((void **)((unsigned char *)ldr_ptr + 0x0c));
  void *next_ptr = cur_ptr;
  do {
    void *next = *((void **)((unsigned char *)next_ptr));
    void *last = *((void **)((unsigned char *)next_ptr + 0x4));
    void *base_addr = *((void **)((unsigned char *)next_ptr + 0x18));
    if (base_addr == module) {
      *((void **)((unsigned char *)last)) = next;
      *((void **)((unsigned char *)next + 0x4)) = last;
      cur_ptr = next;
    }
    next_ptr = *((void **)next_ptr);
  } while (cur_ptr != next_ptr);
}
#endif
}

bool IsDigit(const std::string &str) {
  if (str.length() == 0) {
    return false;
  }
  for (auto it : str) {
    if (it < '0' || it > '9') {
      return false;
    }
  }
  return true;
}

}  // namespace utils
}  // namespace base
