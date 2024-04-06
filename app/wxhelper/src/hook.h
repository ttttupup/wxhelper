#ifndef WXHELPER_HOOK_H_
#define WXHELPER_HOOK_H_
#include <Windows.h>
#include <stdint.h>

namespace hook {
struct InnerMessageStruct {
  char* buffer;
  int64_t length;
  ~InnerMessageStruct() {
    if (this->buffer != NULL) {
      delete[] this->buffer;
      this->buffer = NULL;
    }
  }
};

void SendHttpMsgCallback(PTP_CALLBACK_INSTANCE instance, PVOID context,
                         PTP_WORK Work);

void SendTcpMsgCallback(PTP_CALLBACK_INSTANCE instance, PVOID context,
                        PTP_WORK Work);
class BaseHook {
 public:
  BaseHook():hook_flag_(false),origin_(nullptr),detour_(nullptr){}
  BaseHook(void* origin, void* detour);
  int Hook();
  int Unhook();
  virtual ~BaseHook() {}

 protected:
  bool hook_flag_;
  void* origin_;
  void* detour_;
};

}  // namespace hook
#endif