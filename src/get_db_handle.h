#ifndef GET_DB_HANDLE_H_
#define GET_DB_HANDLE_H_
#include "windows.h"
#include <vector>
#include <string>

std::vector<void *> GetDbHandles();
DWORD GetDbHandleByDbName(wchar_t *dbname);
unsigned int GetLocalIdByMsgId(ULONG64 msgid, int &dbIndex);
std::vector<std::string> GetChatMsgByMsgId(ULONG64 msgid);

std::string GetVoiceBuffByMsgId(ULONG64 msgid);
#endif