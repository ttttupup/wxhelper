#ifndef WXHELPER_SYNC_MSG_HOOK_H_
#define WXHELPER_SYNC_MSG_HOOK_H_
#include "hook.h"
#include "singleton.h"
namespace wxhelper{
class SyncMsgHook : public hook::BaseHook,public  base::Singleton<SyncMsgHook> {
 public:
  void Init();
 private:
  static void HandleSyncMsg(INT64 param1, INT64 param2, INT64 param3);
};

}

#endif