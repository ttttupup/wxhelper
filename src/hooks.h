#ifndef WXHELPER_HOOKS_H_
#define WXHELPER_HOOKS_H_
#include "Windows.h"
#include "wechat_function.h"
namespace wxhelper {
namespace hooks {
extern  UserInfo userinfo;
extern bool user_info_flag_ ;

int HookRecvMsg(char* client_ip, int port,char* url,uint64_t timeout,bool enable);

int UnHookRecvMsg();

void SendSocketMessage(InnerMessageStruct* msg);

int HookLog();
int UnHookLog();
int HookSearchContact();
int UnHookSearchContact();
void DeleteUserInfoCache();
}  // namespace hooks
}  // namespace wxhelper
#endif