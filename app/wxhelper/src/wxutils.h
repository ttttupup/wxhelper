#ifndef WXHELPER_WXUTILS_H_
#define WXHELPER_WXUTILS_H_
#include <windows.h>

#include <string>
namespace wxhelper {
namespace wxutils {
#ifdef _WIN64
int64_t GetWeChatWinBase();
#else
int32_t GetWeChatWinBase();
#endif
std::string ReadSKBuiltinString(INT64 addr);
std::string ReadSKBuiltinBuffer(INT64 addr);
std::string ReadWeChatStr(INT64 addr);

std::string ImageXor(std::string buf);
std::wstring ReadWstring(INT64 addr);
std::string ReadWstringThenConvert(INT64 addr);
int DecodeImage(const wchar_t* file_path, const wchar_t* save_dir);
}  // namespace wxutils

}  // namespace wxhelper
#endif