#include "sync_msg_hook.h" // 引入同步消息钩子头文件

#include "base64.h" // 引入Base64编码库
#include "config.h" // 引入配置文件处理库
#include "nlohmann/json.hpp" // 引入nlohmann JSON库
#include "offset.h" // 引入偏移量定义文件
#include "spdlog/spdlog.h" // 引入日志库
#include "thread_pool.h" // 引入线程池库
#include "utils.h" // 引入工具函数库

#include "wxutils.h" // 引入微信工具函数库

namespace wxhelper
{
  
  // wxhelper命名空间下的SyncMsgHook类的初始化函数
  void SyncMsgHook::Init()
  {
    // 获取微信窗口的基础地址加上偏移量kDoAddMsg
    int64_t addr = wxutils::GetWeChatWinBase() + wechat::offset::kDoAddMsg;
    // 将获取到的地址转换为__DoAddMsg类型的函数指针
    kDoAddMsg = (wechat::function::__DoAddMsg)addr;
    // 保存原始的函数指针
    origin_ = &kDoAddMsg;
    // 设置新的函数指针，用于拦截
    detour_ = &HandleSyncMsg;
    // 初始化钩子标志为未激活
    hook_flag_ = false;
  }

  // 处理同步消息的函数，用于拦截微信消息并进行处理
  void SyncMsgHook::HandleSyncMsg(int64_t param1, int64_t param2, int64_t param3)
  {
    // 输出调试信息到控制台
    wxhelper::wxutils::print_utf8_to_console("HandleSyncMsg called");
    // 输出参数param2的值到控制台
    wxhelper::wxutils::print_utf8_to_console("param2:" + std::to_string(param2));

    // 创建JSON对象用于存储消息内容
    nlohmann::json msg;

    // 填充消息内容到JSON对象
    msg["pid"] = GetCurrentProcessId(); // 当前进程ID
    msg["fromUser"] = wxutils::ReadSKBuiltinString(*(int64_t *)(param2 + 0x18)); // 发送者
    msg["toUser"] = wxutils::ReadSKBuiltinString(*(int64_t *)(param2 + 0x28)); // 接收者
    msg["content"] = wxutils::ReadSKBuiltinString(*(int64_t *)(param2 + 0x30)); // 消息内容
    msg["signature"] = wxutils::ReadWeChatStr(*(int64_t *)(param2 + 0x48)); // 消息签名
    msg["msgId"] = *(int64_t *)(param2 + 0x60); // 消息ID
    msg["msgSequence"] = *(DWORD *)(param2 + 0x5C); // 消息序列号
    msg["createTime"] = *(DWORD *)(param2 + 0x58); // 创建时间
    msg["displayFullContent"] = wxutils::ReadWeChatStr(*(int64_t *)(param2 + 0x50)); // 是否展示完整内容
    DWORD type = *(DWORD *)(param2 + 0x24); // 消息类型
    msg["type"] = type; // 将消息类型添加到JSON对象

    // 根据消息类型处理图片消息
    if (type == 3)
    {
      std::string img = wxutils::ReadSKBuiltinBuffer(*(int64_t *)(param2 + 0x40)); // 读取图片数据
      SPDLOG_INFO("encode size:{}", img.size()); // 输出图片编码大小的日志
      msg["base64Img"] = base64_encode(img); // 将图片数据编码为Base64字符串
    }

    // 将JSON对象转换为字符串，并添加换行符
    std::string jstr = msg.dump() + '\n';
    // 创建InnerMessageStruct结构体用于存储消息
    hook::InnerMessageStruct *inner_msg = new hook::InnerMessageStruct;
    // 分配内存并复制JSON字符串到buffer中
    inner_msg->buffer = new char[jstr.size() + 1];
    memcpy(inner_msg->buffer, jstr.c_str(), jstr.size() + 1);
    // 设置消息长度
    inner_msg->length = jstr.size();

    // 根据配置的接收消息模式，选择不同的回调函数加入线程池
    std::string mode = Config::GetInstance().GetRecvMessageMode(); // 获取接收消息模式
    if (mode == "http")
    {
      bool add = base::ThreadPool::GetInstance().AddWork(
          hook::SendHttpMsgCallback, inner_msg); // 添加HTTP消息发送任务
      SPDLOG_INFO("add http msg work:{}", add); // 输出HTTP消息任务添加结果的日志
    }
    else if (mode == "tcp")
    {
      bool add = base::ThreadPool::GetInstance().AddWork(hook::SendTcpMsgCallback,
                                                         inner_msg); // 添加TCP消息发送任务
      wxhelper::wxutils::print_utf8_to_console("add tcp msg work:" + std::to_string(add)); // 输出TCP消息任务添加结果到控制台
      SPDLOG_INFO("add tcp msg work:{}", add); // 输出TCP消息任务添加结果的日志
    }

    // 确保原始的DoAddMsg函数指针不为空
    if (kDoAddMsg == nullptr)
    {
      int64_t addr = wxutils::GetWeChatWinBase() + wechat::offset::kDoAddMsg;
      kDoAddMsg = (wechat::function::__DoAddMsg)addr; // 重新获取原始函数指针
    }
    // 调用原始的DoAddMsg函数，完成消息的添加
    kDoAddMsg(param1, param2, param3);
  }
} // namespace wxhelper