#include "wxutils.h"

#include "utils.h"
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

#include <iostream>
#include <locale>
#include <codecvt>
#include <string>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <__msvc_chrono.hpp>

namespace wxhelper
{
  namespace wxutils
  {
#ifdef _WIN64
    int64_t GetWeChatWinBase()
    {
      return (int64_t)GetModuleHandleA("WeChatWin.dll");
    }
#else
    int32_t GetWeChatWinBase()
    {
      return (int32_t)GetModuleHandleA("WeChatWin.dll");
    }
#endif

    std::string ReadSKBuiltinString(INT64 addr)
    {
      INT64 inner_string = *(INT64 *)(addr + 0x8);
      if (inner_string == 0)
      {
        return std::string();
      }
      return ReadWeChatStr(inner_string);
    }

    std::string ReadSKBuiltinBuffer(INT64 addr)
    {
      INT64 len = *(INT64 *)(addr + 0x10);
      if (len == 0)
      {
        return std::string();
      }
      INT64 inner_string = *(INT64 *)(addr + 0x8);
      if (inner_string == 0)
      {
        return std::string();
      }
      return ReadWeChatStr(inner_string);
    }

    std::string ReadWeChatStr(INT64 addr)
    {
      INT64 len = *(INT64 *)(addr + 0x10);
      if (len == 0)
      {
        return std::string();
      }
      INT64 max_len = *(INT64 *)(addr + 0x18);
      if ((max_len | 0xF) == 0xF)
      {
        return std::string((char *)addr, len);
      }
      char *char_from_user = *(char **)(addr);
      return std::string(char_from_user, len);
    }

    std::string ImageXor(std::string buf)
    {
      const char *origin = buf.c_str();
      short key = 0;
      if ((*origin ^ JPEG0) == (*(origin + 1) ^ JPEG1))
      {
        key = *origin ^ JPEG0;
      }
      else if ((*origin ^ PNG1) == (*(origin + 1) ^ PNG2))
      {
        key = *origin ^ PNG1;
      }
      else if ((*origin ^ GIF0) == (*(origin + 1) ^ GIF1))
      {
        key = *origin ^ GIF0;
      }
      else if ((*origin ^ BMP0) == (*(origin + 1) ^ BMP1))
      {
        key = *origin ^ BMP0;
      }
      else
      {
        key = -1;
      }
      if (key > 0)
      {
        char *img_buf = new char[buf.size()];
        for (unsigned int i = 0; i < buf.size(); i++)
        {
          img_buf[i] = *(origin + i) ^ key;
        }
        std::string str(img_buf);
        delete[] img_buf;
        img_buf = NULL;
        return str;
      }
      return std::string();
    }

    std::wstring ReadWstring(INT64 addr)
    {
      DWORD len = *(DWORD *)(addr + 0x8);
      if (len == 0)
      {
        return std::wstring();
      }
      wchar_t *str = *(wchar_t **)(addr);
      if (str == NULL)
      {
        return std::wstring();
      }
      return std::wstring(str, len);
    }
    std::string ReadWstringThenConvert(INT64 addr)
    {
      std::wstring wstr = ReadWstring(addr);
      return base::utils::WstringToUtf8(wstr);
    }

    int DecodeImage(const wchar_t *file_path, const wchar_t *save_dir)
    {
      return -1;
    }

    void print_utf8_to_console(const std::string &utf8_str)
    {
      // 获取当前时间
      auto now = std::chrono::system_clock::now();
      std::time_t now_time = std::chrono::system_clock::to_time_t(now);

      // 格式化时间为字符串
      std::ostringstream time_stream;
      time_stream << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S");

      // 将时间戳添加到日志消息前面
      std::string log_message = "[" + time_stream.str() + "] " + utf8_str;

      // 将 UTF-8 字符串转换为宽字符字符串
      std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
      std::wstring wide_str = converter.from_bytes(log_message);

      // 使用系统默认区域设置
      std::wcout.imbue(std::locale(""));

      // 输出日志消息
      std::wcout << wide_str << std::endl;
    }

  } // namespace wxutils
} // namespace wxhelper
