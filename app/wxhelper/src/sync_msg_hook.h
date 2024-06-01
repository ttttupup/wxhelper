// 防止头文件重复包含，确保整个程序中只被包含一次
#ifndef WXHELPER_SYNC_MSG_HOOK_H_
#define WXHELPER_SYNC_MSG_HOOK_H_

// 引入hook.h，该头文件定义了hook相关函数和类
#include "hook.h"
// 引入singleton.h，该头文件提供了单例模式的实现
#include "singleton.h"
// 引入wechat_interface.h，该头文件定义了微信相关接口
#include "wechat_interface.h"

// 定义wxhelper命名空间，用于组织wxhelper相关的代码和类
namespace wxhelper {
    // 声明一个静态成员变量kDoAddMsg，它是一个函数指针，指向微信的__DoAddMsg函数
    // 该变量初始值为nullptr，表示未设置指向任何函数
    static wechat::function::__DoAddMsg kDoAddMsg = nullptr;

    // 定义SyncMsgHook类，它继承自hook::BaseHook和base::Singleton<SyncMsgHook>
    // BaseHook可能提供了hook相关的方法和属性
    // Singleton模板确保SyncMsgHook类只有一个实例
    class SyncMsgHook : public hook::BaseHook, public base::Singleton<SyncMsgHook> {
    public:
        // 定义一个公共成员函数Init，用于初始化SyncMsgHook类
        void Init();
    private:
        // 定义一个私有静态成员函数HandleSyncMsg，用于处理同步消息
        // 该函数接收三个int64_t类型的参数param1、param2、param3
        static void HandleSyncMsg(int64_t param1, int64_t param2, int64_t param3);
    };

    // 通过Singleton模板，可以获取SyncMsgHook类的唯一实例
    // 例如：auto syncMsgHook = wxhelper::SyncMsgHook::GetInstance();
}

// 结束头文件的声明
#endif