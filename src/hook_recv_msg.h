#ifndef HOOK_RECV_MSG_H_
#define HOOK_RECV_MSG_H_

/// @brief hook any address   address+0x5
/// @param port 端口
/// @return 成功返回1,已经hook返回2
int  HookRecvMsg(char* client_ip,int port);

int UnHookRecvMsg();
#endif