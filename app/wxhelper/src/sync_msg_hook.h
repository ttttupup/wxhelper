#ifndef WXHELPER_SYNC_MSG_HOOK_H_
#define WXHELPER_SYNC_MSG_HOOK_H_
#include "hook.h"
#include "singleton.h"
#include "wechat_interface.h"
namespace wxhelper{
static wechat::function::__DoAddMsg kDoAddMsg= nullptr; 
class SyncMsgHook : public hook::BaseHook,public  base::Singleton<SyncMsgHook> {
 public:
  void Init();
 private:
  static void HandleSyncMsg(int64_t param1, int64_t param2, int64_t param3);
};

}

#endif