#ifndef GET_DB_HANDLE_H_
#define GET_DB_HANDLE_H_
#include "windows.h"
#include <vector>

std::vector<void *> GetDbHandles();
DWORD GetDbHandleByDbName(wchar_t *dbname);
unsigned int GetLocalIdByMsgId(ULONG64 msgid, int &dbIndex);
#endif