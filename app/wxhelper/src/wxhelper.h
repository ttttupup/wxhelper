// 定义了一个预处理宏，用于防止头文件被重复包含
#ifndef WXHELPER_WXHELPER_H_
#define WXHELPER_WXHELPER_H_

// 包含Windows.h头文件，用于Windows应用程序接口
#include <Windows.h>
// 引入单例模式的实现，singleton.h文件中定义了如何实现一个单例类
#include "singleton.h"

// 定义一个名为wxhelper的命名空间，用于组织相关的类和函数
namespace wxhelper
{

    // 定义了一个名为WxHelper的类，继承自base::Singleton<WxHelper>
    // 这意味着WxHelper类将遵循单例模式，即全局只有一个实例
    class WxHelper : public base::Singleton<WxHelper>
    {

    public:
        // WxHelper类的公有成员函数，用于初始化WxHelper实例
        void init(HMODULE module);
            // 初始化函数的实现，这里接受一个HMODULE类型的参数module
            // 这个参数通常是一个模块的句柄，用于加载或初始化相关资源

            // WxHelper类的公有成员函数，用于清理或释放WxHelper实例使用的资源
        void finally();
        // 最终化函数的实现，用于在程序结束前清理或释放资源
    };
}

// 结束命名空间wxhelper
#endif // WXHELPER_WXHELPER_H_