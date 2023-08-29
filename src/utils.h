#ifndef WXHELPER_UTILS_H_
#define WXHELPER_UTILS_H_
#include <windows.h>

#include <string>
#include <vector>
#define STRING2INT(str) (Utils::IsDigit(str) ? stoi(str) : 0)
#define WS2LPWS(wstr) (LPWSTR) wstr.c_str()


namespace wxhelper {

class Utils {
 public:
  static std::wstring UTF8ToWstring(const std::string &str);

  static std::string WstringToUTF8(const std::wstring &str);

  static std::wstring AnsiToWstring(const std::string &input,
                                    INT64 locale = CP_ACP);

  static std::string WstringToAnsi(const std::wstring &input,
                                   INT64 locale = CP_ACP);

  static UINT64 GetWeChatWinBase();

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

  static bool IsTextUtf8(const char *str, INT64 length);

  static void Hide(HMODULE module);

  static std::string ReadSKBuiltinString(INT64 addr);
  static std::string ReadSKBuiltinBuffer(INT64 addr);
  static std::string ReadWeChatStr(INT64 addr);

  static std::string ImageXor(std::string buf);
  static std::wstring ReadWstring(INT64 addr);
  static std::string ReadWstringThenConvert(INT64 addr);
  
  static INT64 DecodeImage(const wchar_t* file_path,const wchar_t* save_dir);

  static std::vector<INT64> QWordScan(INT64 value, int align,
                                        const wchar_t *module);

  static std::vector<INT64> QWordScan(INT64 value, INT64 start,int align);
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

  template <typename T>
  static T *WxHeapAlloc(size_t n) {
    return (T *)HeapAlloc(GetProcessHeap(), 0, n);
  }
};

}  // namespace wxhelper
#endif