#include "pch.h"
#include "utils.h"



namespace wxhelper {
std::wstring Utils::UTF8ToWstring(const std::string &str) {
  return Utils::AnsiToWstring(str, CP_UTF8);
}

std::string Utils::WstringToUTF8(const std::wstring &str) {
  return Utils::WstringToAnsi(str, CP_UTF8);
}

std::wstring Utils::AnsiToWstring(const std::string &input, DWORD locale) {
  int wchar_len = MultiByteToWideChar(locale, 0, input.c_str(), -1, NULL, 0);
  if (wchar_len > 0) {
    std::vector<wchar_t> temp(wchar_len);
    MultiByteToWideChar(CP_UTF8, 0, input.c_str(), -1, &temp[0], wchar_len);
    return std::wstring(&temp[0]);
  }
  
  return std::wstring();
}

std::string Utils::WstringToAnsi(const std::wstring &input, DWORD locale) {
  int char_len = WideCharToMultiByte(locale, 0, input.c_str(), -1, 0, 0, 0, 0);
  if (char_len > 0) {
    std::vector<char> temp(char_len);
    WideCharToMultiByte(locale, 0, input.c_str(), -1, &temp[0], char_len, 0, 0);
    return std::string(&temp[0]);
  }
  return std::string();
}

DWORD Utils::GetWeChatWinBase() {
  return (DWORD)GetModuleHandleA("WeChatWin.dll");
}

bool Utils::CreateConsole() {
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

void Utils::CloseConsole() {
  fclose(stdin);
  fclose(stdout);
  fclose(stderr);
  FreeConsole();
}

std::string Utils::EncodeHexString(const std::string &str) {
  const std::string hex_table = "0123456789abcdef";
  std::string sb;
  for (int i = 0; i < str.length(); i++) {
    sb += hex_table.at((str[i] & 0xf0) >> 4);
    sb += hex_table.at((str[i] & 0x0f) >> 0);
  }
  return sb;
}

std::string Utils::Hex2String(const std::string &hex_str) {
  std::string ret;
  const std::string hex_table = "0123456789abcdef";
  for (int i = 0; i < hex_str.length(); i += 2) {
    ret += BYTE(hex_table.find(hex_str.at(i)) << 4 |
                hex_table.find(hex_str.at(i + 1)));
  }
  return ret;
}

std::string Utils::Bytes2Hex(const BYTE *bytes, const int length) {
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

void Utils::Hex2Bytes(const std::string &hex, BYTE *bytes) {
  int byte_len = hex.length() / 2;
  std::string str;
  unsigned int n;
  for (int i = 0; i < byte_len; i++) {
    str = hex.substr(i * 2, 2);
    sscanf_s(str.c_str(), "%x", &n);
    bytes[i] = n;
  }
}



bool Utils::IsDigit(std::string str) {
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

bool Utils::FindOrCreateDirectoryW(const wchar_t *path) {
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

void Utils::HookAnyAddress(DWORD hook_addr, LPVOID jmp_addr, char *origin) {
  BYTE jmp_code[5] = {0};
  jmp_code[0] = 0xE9;
  *(DWORD *)&jmp_code[1] = (DWORD)jmp_addr - hook_addr - 5;
  DWORD old_protext = 0;
  VirtualProtect((LPVOID)hook_addr, 5, PAGE_EXECUTE_READWRITE, &old_protext);
  ReadProcessMemory(GetCurrentProcess(), (LPVOID)hook_addr, origin, 5, 0);
  memcpy((void *)hook_addr, jmp_code, 5);
  VirtualProtect((LPVOID)hook_addr, 5, old_protext, &old_protext);
}

void Utils::UnHookAnyAddress(DWORD hook_addr, char *origin) {
  DWORD old_protext = 0;
  VirtualProtect((LPVOID)hook_addr, 5, PAGE_EXECUTE_READWRITE, &old_protext);
  WriteProcessMemory(GetCurrentProcess(), (LPVOID)hook_addr, origin, 5, 0);
  VirtualProtect((LPVOID)hook_addr, 5, old_protext, &old_protext);
}

std::wstring Utils::GetTimeW(long long timestamp) {
  wchar_t *wstr = new wchar_t[20];
  memset(wstr, 0, 20 * 2);
  tm tm_out;
  localtime_s(&tm_out, &timestamp);
  swprintf_s(wstr, 20, L"%04d-%02d-%02d %02d:%02d:%02d", 1900 + tm_out.tm_year,
             tm_out.tm_mon + 1, tm_out.tm_mday, tm_out.tm_hour, tm_out.tm_min,
             tm_out.tm_sec);
  std::wstring str_time(wstr);
  delete[] wstr;
  return str_time;
}

std::string Utils::WCharToUTF8(wchar_t *wstr) {
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

}  // namespace wxhelper