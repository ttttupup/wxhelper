#ifndef WXHELPER_HOOK_H_
#define WXHELPER_HOOK_H_
#include "noncopyable.h"
#include <Windows.h>
namespace hook {

static void SendHttpMsgCallback(PTP_CALLBACK_INSTANCE instance, PVOID context,
                                PTP_WORK Work);

static void SendTcpMsgCallback(PTP_CALLBACK_INSTANCE instance, PVOID context,
                               PTP_WORK Work);
class BaseHook : public NonCopyable {
  public:
  BaseHook(void* origin, void* detour);
  int Hook();
  int Unhook();

 protected:
  bool hook_flag_;
  void* origin_;
  void* detour_;
};

class SyncMsgHook : public BaseHook {
 public:
  explicit SyncMsgHook(void* origin, void* detour);
 private:
  static void HandleSyncMsg(INT64 param1, INT64 param2, INT64 param3);
};

}  // namespace hook
#endif