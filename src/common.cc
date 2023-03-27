#include "pch.h"
#include "common.h"


using namespace std;

/// @brief utf8 转换成unicode
/// @param buffer utf8
/// @return  unicode
wstring utf8_to_unicode(const char *buffer) {
  int c_size = MultiByteToWideChar(CP_UTF8, 0, buffer, -1, NULL, 0);
  if (c_size > 0) {
    wchar_t *temp = new wchar_t[c_size + 1];
    MultiByteToWideChar(CP_UTF8, 0, buffer, -1, temp, c_size);
    temp[c_size] = L'\0';
    wstring ret(temp);
    delete[] temp;
    temp = NULL;
    return ret;
  }
  return wstring();
}

/// @brief unicode转换utf8
/// @param wstr unicode
/// @return string utf8
string unicode_to_utf8(wchar_t *wstr) {
  int c_size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, FALSE);
  if (c_size > 0) {
    char *buffer = new char[c_size + 1];
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, buffer, c_size, NULL, FALSE);
    buffer[c_size] = '\0';
    string str(buffer);
    delete[] buffer;
    buffer = NULL;
    return str;
  }
  return string();
}

/// @brief 获取WeChatWin.dll基址
/// @return 基址
DWORD GetWeChatWinBase() { return (DWORD)GetModuleHandleA("WeChatWin.dll"); }

/// @brief 创建窗口
/// @param  void
/// @return 创建结果
BOOL CreateConsole(void) {
  if (AllocConsole()) {
    AttachConsole(GetCurrentProcessId());
    FILE *retStream;
    freopen_s(&retStream, "CONOUT$", "w", stdout);
    if (!retStream) throw std::runtime_error("Stdout redirection failed.");
    freopen_s(&retStream, "CONOUT$", "w", stderr);
    if (!retStream) throw std::runtime_error("Stderr redirection failed.");
    return 0;
  }
  return 1;
}

/// @brief hook any addr
/// @param hook_addr need hook of addr
/// @param jmp_addr hook function addr
/// @param origin origin code
void HookAnyAddress(DWORD hook_addr, LPVOID jmp_addr, char *origin) {
  BYTE jmp_code[5] = {0};
  jmp_code[0] = 0xE9;
  *(DWORD *)&jmp_code[1] = (DWORD)jmp_addr - hook_addr - 5;
  DWORD old_protext = 0;
  VirtualProtect((LPVOID)hook_addr, 5, PAGE_EXECUTE_READWRITE, &old_protext);
  ReadProcessMemory(GetCurrentProcess(), (LPVOID)hook_addr, origin, 5, 0);
  memcpy((void *)hook_addr, jmp_code, 5);
  VirtualProtect((LPVOID)hook_addr, 5, old_protext, &old_protext);
}

/// @brief unhook
/// @param hook_addr hook addr
/// @param origin origin addr code
void UnHookAnyAddress(DWORD hook_addr, char *origin) {
  DWORD old_protext = 0;
  VirtualProtect((LPVOID)hook_addr, 5, PAGE_EXECUTE_READWRITE, &old_protext);
  WriteProcessMemory(GetCurrentProcess(), (LPVOID)hook_addr, origin, 5, 0);
  VirtualProtect((LPVOID)hook_addr, 5, old_protext, &old_protext);
}

/// @brief get timeW
/// @param timestamp timestamp
/// @return str
wstring GetTimeW(long long timestamp) {
  wchar_t *wstr = new wchar_t[20];
  memset(wstr, 0, 20 * 2);
  tm tm_out;
  localtime_s(&tm_out, &timestamp);
  swprintf_s(wstr, 20, L"%04d-%02d-%02d %02d:%02d:%02d", 1900 + tm_out.tm_year,
             tm_out.tm_mon + 1, tm_out.tm_mday, tm_out.tm_hour, tm_out.tm_min,
             tm_out.tm_sec);
  wstring strTimeW(wstr);
  delete[] wstr;
  return strTimeW;
}

wstring String2Wstring(string str) {
  wstring result;
  int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
  wchar_t *buffer = new wchar_t[len + 1];
  MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
  buffer[len] = '\0';  
  result.append(buffer);
  delete[] buffer;
  return result;
}

string Wstring2String(wstring wstr) {
  string result;
  int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0,
                                NULL, NULL);
  char *buffer = new char[len + 1];
  WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL,
                      NULL);
  buffer[len] = '\0';
  result.append(buffer);
  delete[] buffer;
  return result;
}

BOOL FindOrCreateDirectoryW(const wchar_t *path) {
  WIN32_FIND_DATAW fd;
  HANDLE hFind = ::FindFirstFileW(path, &fd);
  if (hFind != INVALID_HANDLE_VALUE  && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
    FindClose(hFind);
    return true;
  }

  if (!::CreateDirectoryW(path, NULL)) {
    return false;
  }
  return true;
}

void CloseConsole(){
  fclose(stdin);
  fclose(stdout);
  fclose(stderr);
  FreeConsole();
}

std::string EncodeHexString(const std::string &str) {
  const std::string hex_table = "0123456789abcdef";
  string sb;
  for (int i = 0; i < str.length(); i++) {
    sb += hex_table.at((str[i] & 0xf0) >> 4);
    sb += hex_table.at((str[i] & 0x0f) >> 0);
  }
  return sb;
}

std::string Hex2String(const std::string &hex_str) {
  std::string ret;
  const std::string hex_table = "0123456789abcdef";
  for (int i = 0; i < hex_str.length(); i += 2) {
    ret += BYTE(hex_table.find(hex_str.at(i)) << 4 |
                hex_table.find(hex_str.at(i + 1)));
  }
  return ret;
}

std::string Bytes2Hex(const BYTE *bytes, const int length) {
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

void Hex2Bytes(const std::string &hex, BYTE *bytes) {
  int byte_len = hex.length() / 2;
  std::string str;
  unsigned int n;
  for (int i = 0; i < byte_len; i++) {
    str = hex.substr(i * 2, 2);
    sscanf_s(str.c_str(), "%x", &n);
    bytes[i] = n;
  }
}
