#ifndef BASE_UTILS_H_
#define BASE_UTILS_H_
#include <windows.h>

#include <string>
#include <vector>
namespace base {

namespace utils {
std::wstring Utf8ToWstring(const std::string &str);

std::string WstringToUtf8(const std::wstring &str);

std::wstring AnsiToWstring(const std::string &input, INT64 locale = CP_ACP);

std::string WstringToAnsi(const std::wstring &input, INT64 locale = CP_ACP);

std::string WcharToUtf8(wchar_t *wstr);

std::string StringToHex(const std::string &str);

std::string HexToString(const std::string &hex_str);

std::string BytesToHex(const BYTE *bytes, const int length);

void HexToBytes(const std::string &hex, BYTE *bytes);

template <typename T1, typename T2>
std::vector<T1> split(T1 str, T2 letter) {
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
T1 replace(T1 source, T2 replaced, T1 replaceto) {
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

bool CreateConsole();

void CloseConsole();

void HideModule(HMODULE module);

bool IsDigit(const std::string &str);
}  // namespace utils
}  // namespace base
#endif