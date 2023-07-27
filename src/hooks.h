#ifndef WXHELPER_HOOKS_H_
#define WXHELPER_HOOKS_H_
#include "Windows.h"
#include "wechat_function.h"
namespace wxhelper {
namespace hooks {

int HookSyncMsg(std::string client_ip, int port, std::string url, uint64_t timeout,
                bool enable);

int UnHookSyncMsg();

int HookLog();

int UnHookLog();

}  // namespace hooks
}  // namespace wxhelper
#endif