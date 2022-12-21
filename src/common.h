#ifndef COMMON_H_
#define COMMON_H_
#include <string>
using namespace std;
#define READ_WSTRING(addr, offset) ((*(DWORD *)(addr + offset + 0x4) == 0) ? wstring(L"") : wstring((wchar_t *)(*(DWORD *)(addr + offset)), *(DWORD *)(addr + offset + 0x4)))

/// @brief utf8 转换成unicode
/// @param buffer utf8
/// @return  unicode
wstring utf8_to_unicode(const char *buffer);

/// @brief unicode转换utf8
/// @param wstr unicode
/// @return utf8
string unicode_to_utf8(wchar_t *wstr);

/// @brief 获取WeChatWin.dll基址
/// @return 基址
DWORD GetWeChatWinBase();
/// @brief 创建窗口
/// @param  void
/// @return 创建结果
BOOL CreateConsole(void);
/// @brief hook任意地址
/// @param hook_addr 被hook的地址
/// @param jmp_addr 被hook的函数的地址
/// @param origin 原始code
void HookAnyAddress(DWORD hook_addr, LPVOID jmp_addr, char *origin);

/// @brief 取消hook
/// @param hook_addr 被hook的地址
/// @param origin 原始code
void UnHookAnyAddress(DWORD hook_addr, char *origin);

/// @brief get timeW
/// @param timestamp timestamp
/// @return str
wstring GetTimeW(long long timestamp);
/// @brief  unicode trans utf8
/// @param str unicode str
/// @return  utf8 str
std::string UnicodeToUtf8(const wchar_t *str);
/// @brief  string convert wstring
/// @param str 
/// @return 
wstring String2Wstring(string str);
/// @brief wstring convert string
/// @param str 
/// @return 
string Wstring2String(wstring wstr);

/// @brief create dir
/// @param path 
/// @return 
BOOL FindOrCreateDirectoryW(const wchar_t *path);


template <typename T1, typename T2>
vector<T1> split(T1 str, T2 letter) {
  vector<T1> arr;
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
T1 replace(T1 source, T2 replaced, T1 replaceto) {
  vector<T1> v_arr = split(source, replaced);
  if (v_arr.size() < 2) return source;
  T1 temp;
  for (unsigned int i = 0; i < v_arr.size() - 1; i++) {
    temp += v_arr[i];
    temp += replaceto;
  }
  temp += v_arr[v_arr.size() - 1];
  return temp;
}
#endif