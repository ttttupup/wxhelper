// 引入wxutils.h文件，该文件可能包含与wxWidgets相关的工具函数
#include "wxutils.h"

// 引入utils.h文件，该文件可能包含一些通用的工具函数
#include "utils.h"

// 定义缓冲区大小为1024字节
#define BUFSIZE 1024

// 定义JPEG文件的魔术数字
#define JPEG0 0xFF
#define JPEG1 0xD8
#define JPEG2 0xFF

// 定义PNG文件的魔术数字
#define PNG0 0x89
#define PNG1 0x50
#define PNG2 0x4E

// 定义BMP文件的魔术数字
#define BMP0 0x42
#define BMP1 0x4D

// 定义GIF文件的魔术数字
#define GIF0 0x47
#define GIF1 0x49
#define GIF2 0x46

// 引入标准输入输出库
#include <iostream>
// 引入本地化设置库
#include <locale>
// 引入codecvt库，用于字符编码转换
#include <codecvt>
// 引入字符串库
#include <string>
// 引入时间库，用于时间函数
#include <chrono>
// 引入iomanip库，用于控制输入输出格式
#include <iomanip>
// 引入字符串流库
#include <sstream>
// 引入__msvc_chrono.hpp库，用于MSVC特有的时间函数
#include <__msvc_chrono.hpp>

// 定义wxhelper命名空间，用于封装wxWidgets相关的辅助功能
namespace wxhelper
{
  // 定义wxutils命名空间，用于封装微信相关的工具函数
  namespace wxutils
  {
    // 根据操作系统平台定义GetWeChatWinBase函数的返回类型
#ifdef _WIN64
    // 在64位Windows平台上，返回类型为int64_t
    int64_t GetWeChatWinBase()
    {
      // 使用GetModuleHandleA函数获取WeChatWin.dll的模块句柄
      return (int64_t)GetModuleHandleA("WeChatWin.dll");
    }
#else
    // 在32位Windows平台上，返回类型为int32_t
    int32_t GetWeChatWinBase()
    {
      // 使用GetModuleHandleA函数获取WeChatWin.dll的模块句柄
      return (int32_t)GetModuleHandleA("WeChatWin.dll");
    }
#endif

    // 定义ReadSKBuiltinString函数，用于读取内置字符串
    std::string ReadSKBuiltinString(INT64 addr)
    {
      // 读取地址偏移0x8处的内部字符串地址
      INT64 inner_string = *(INT64 *)(addr + 0x8);
      // 如果内部字符串地址为0，则返回空字符串
      if (inner_string == 0)
      {
        return std::string();
      }
      // 否则，调用ReadWeChatStr函数读取字符串
      return ReadWeChatStr(inner_string);
    }

    // 定义ReadSKBuiltinBuffer函数，用于读取内置缓冲区
    std::string ReadSKBuiltinBuffer(INT64 addr)
    {
      // 读取地址偏移0x10处的长度
      INT64 len = *(INT64 *)(addr + 0x10);
      // 如果长度为0，则返回空字符串
      if (len == 0)
      {
        return std::string();
      }
      // 读取地址偏移0x8处的内部字符串地址
      INT64 inner_string = *(INT64 *)(addr + 0x8);
      // 如果内部字符串地址为0，则返回空字符串
      if (inner_string == 0)
      {
        return std::string();
      }
      // 否则，调用ReadWeChatStr函数读取字符串
      return ReadWeChatStr(inner_string);
    }

    // 定义ReadWeChatStr函数，用于读取微信字符串
    std::string ReadWeChatStr(INT64 addr)
    {
      // 读取地址偏移0x10处的长度
      INT64 len = *(INT64 *)(addr + 0x10);
      // 如果长度为0，则返回空字符串
      if (len == 0)
      {
        return std::string();
      }
      // 读取地址偏移0x18处的最大长度
      INT64 max_len = *(INT64 *)(addr + 0x18);
      // 如果最大长度的低4位为0xF，则直接从地址处读取字符串
      if ((max_len | 0xF) == 0xF)
      {
        return std::string((char *)addr, len);
      }
      // 否则，读取地址处指向的字符指针，并从该指针处读取字符串
      char *char_from_user = *(char **)(addr);
      return std::string(char_from_user, len);
    }

    // 定义ImageXor函数，用于对图像数据进行异或解密
    std::string ImageXor(std::string buf)
    {
      // 获取原始数据的C风格字符串表示
      const char *origin = buf.c_str();
      short key = 0;
      // 尝试使用JPEG的魔术数字进行解密
      if ((*origin ^ JPEG0) == (*(origin + 1) ^ JPEG1))
      {
        key = *origin ^ JPEG0;
      }
      // 尝试使用PNG的魔术数字进行解密
      else if ((*origin ^ PNG1) == (*(origin + 1) ^ PNG2))
      {
        key = *origin ^ PNG1;
      }
      // 尝试使用GIF的魔术数字进行解密
      else if ((*origin ^ GIF0) == (*(origin + 1) ^ GIF1))
      {
        key = *origin ^ GIF0;
      }
      // 尝试使用BMP的魔术数字进行解密
      else if ((*origin ^ BMP0) == (*(origin + 1) ^ BMP1))
      {
        key = *origin ^ BMP0;
      }
      // 如果无法解密，则返回空字符串
      else
      {
        key = -1;
      }
      // 如果找到了解密密钥，则进行解密
      if (key > 0)
      {
        // 分配内存用于存放解密后的图像数据
        char *img_buf = new char[buf.size()];
        // 对图像数据进行异或操作进行解密
        for (unsigned int i = 0; i < buf.size(); i++)
        {
          img_buf[i] = *(origin + i) ^ key;
        }
        // 创建解密后的字符串并返回
        std::string str(img_buf);
        // 释放分配的内存
        delete[] img_buf;
        img_buf = NULL;
        return str;
      }
      // 如果没有找到解密密钥，则返回空字符串
      return std::string();
    }

    // 定义ReadWstring函数，用于读取宽字符字符串
    std::wstring ReadWstring(INT64 addr)
    {
      // 读取地址偏移0x8处的长度
      DWORD len = *(DWORD *)(addr + 0x8);
      // 如果长度为0，则返回空宽字符串
      if (len == 0)
      {
        return std::wstring();
      }
      // 读取地址处指向的宽字符指针
      wchar_t *str = *(wchar_t **)(addr);
      // 如果宽字符指针为空，则返回空宽字符串
      if (str == NULL)
      {
        return std::wstring();
      }
      // 否则，从宽字符指针处读取宽字符串
      return std::wstring(str, len);
    }

    // 定义ReadWstringThenConvert函数，用于读取宽字符串并转换为UTF-8字符串
    std::string ReadWstringThenConvert(INT64 addr)
    {
      // 调用ReadWstring函数读取宽字符串
      std::wstring wstr = ReadWstring(addr);
      // 调用WstringToUtf8函数将宽字符串转换为UTF-8字符串
      return base::utils::WstringToUtf8(wstr);
    }

    // 定义DecodeImage函数，用于解码图像数据
    int DecodeImage(const wchar_t *file_path, const wchar_t *save_dir)
    {
      // 该函数目前尚未实现，返回-1表示失败
      return -1;
    }

    // 定义print_utf8_to_console函数，用于将UTF-8字符串打印到控制台
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