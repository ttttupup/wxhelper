#ifndef WXHELPER_EXPORT_H_
#define WXHELPER_EXPORT_H_

extern "C" UINT64 _GetAccountService(UINT64 addr);
extern "C" UINT64 _GetDataSavePath(UINT64 addr,ULONG_PTR out);
extern "C" UINT64 _GetCurrentDataPath(UINT64 addr,ULONG_PTR out);
extern "C" UINT64 _SendTextMsg(UINT64 mgr_addr,UINT64 send_text_addr,UINT64 free_addr,UINT64 receiver,UINT64 msg,UINT64 chat_msg);
#endif