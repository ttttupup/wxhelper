#ifndef WXHELPER_HOOK_H_
#define WXHELPER_HOOK_H_
#include <Windows.h>
#include <stdint.h>

namespace hook {
    /**
     * @struct InnerMessageStruct 定义了一个名为 InnerMessageStruct 的结构体，用于在内部消息传递中存储缓冲区和长度信息
     */
    struct InnerMessageStruct {
        char* buffer;
        int64_t length;
        // 析构函数，用于释放缓冲区内存
        ~InnerMessageStruct() {
            if (this->buffer!= NULL) {
                delete[] this->buffer;
                this->buffer = NULL;
            }
        }
    };

    /**
     * @brief 发送 HTTP 消息的回调函数
     * @param instance 回调实例
     * @param context 上下文
     * @param Work 工作项
     */
    void SendHttpMsgCallback(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_WORK Work);

    /**
     * @brief 发送 TCP 消息的回调函数
     * @param instance 回调实例
     * @param context 上下文
     * @param Work 工作项
     */
    void SendTcpMsgCallback(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_WORK Work);

    /**
     * @class BaseHook 定义了一个名为 BaseHook 的类，作为钩子的基类
     */
    class BaseHook {
        public:
            // 构造函数
            BaseHook():hook_flag_(false),origin_(nullptr),detour_(nullptr){}
            // 构造函数，接收原始函数和替换函数的指针
            BaseHook(void* origin, void* detour);
            // 钩子函数，用于安装钩子
            int Hook();
            // 卸载钩子函数
            int Unhook();
            // 虚析构函数
            virtual ~BaseHook() {}

        protected:
            // 钩子标志
            bool hook_flag_;
            // 原始函数指针
            void* origin_;
            // 替换函数指针
            void* detour_;
    };

}  // namespace hook
#endif