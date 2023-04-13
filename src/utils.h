#ifndef WXHELPER_UTILS_H_
#define WXHELPER_UTILS_H_
#include <windows.h>

#include <string>
#include <vector>
#define STRING2INT(str) (Utils::IsDigit(str) ? stoi(str) : 0)
#define WS2LPWS(wstr) (LPWSTR) wstr.c_str()
#define READ_WSTRING(addr, offset) ((*(DWORD *)(addr + offset + 0x4) == 0) ? std::wstring(L"") : std::wstring((wchar_t *)(*(DWORD *)(addr + offset)), *(DWORD *)(addr + offset + 0x4)))


namespace wxhelper {

class Utils {
 public:
  static std::wstring UTF8ToWstring(const std::string &str);

  static std::string WstringToUTF8(const std::wstring &str);

  static std::wstring AnsiToWstring(const std::string &input,
                                    DWORD locale = CP_ACP);

  static std::string WstringToAnsi(const std::wstring &input,
                                   DWORD locale = CP_ACP);

  static DWORD GetWeChatWinBase();

  static bool CreateConsole();

  static void CloseConsole();

  static std::string EncodeHexString(const std::string &str);

  static std::string Hex2String(const std::string &hex_str);

  static std::string Bytes2Hex(const BYTE *bytes, const int length);

  static void Hex2Bytes(const std::string &hex, BYTE *bytes);

  static bool IsDigit(std::string str);

  static bool FindOrCreateDirectoryW(const wchar_t *path);

  static void HookAnyAddress(DWORD hook_addr, LPVOID jmp_addr, char *origin);
  static void UnHookAnyAddress(DWORD hook_addr, char *origin);
  static std::wstring GetTimeW(long long timestamp);

  static std::string WCharToUTF8(wchar_t *wstr);

  template <typename T1, typename T2>
 static std::vector<T1> split(T1 str, T2 letter) {
    std::vector<T1> arr;
    size_t pos;
    while ((pos = str.find_first_of(letter)) != T1::npos) {
      T1 str1 = str.substr(0, pos);
      arr.push_back(str1);
      str = str.substr(pos + 1, str.length() - pos - 1);
    }
    arr.push_back(str);
    return arr;
  }

  template <typename T1, typename T2>
 static T1 replace(T1 source, T2 replaced, T1 replaceto) {
    std::vector<T1> v_arr = split(source, replaced);
    if (v_arr.size() < 2) return source;
    T1 temp;
    for (unsigned int i = 0; i < v_arr.size() - 1; i++) {
      temp += v_arr[i];
      temp += replaceto;
    }
    temp += v_arr[v_arr.size() - 1];
    return temp;
  }
};
}  // namespace wxhelper
#endif