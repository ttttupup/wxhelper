#ifndef WXHELPER_WECHAT_HOOK_H_
#define WXHELPER_WECHAT_HOOK_H_
#include "wechat_function.h"
#include "singleton.h"

namespace hook {
struct WechatHookParam {
  std::string server_ip = "127.0.0.1";
  std::string http_url = "http:://127.0.0.1:19088";
  int server_port = 19099;
  bool enable_http = false;
  uint64_t http_time_out = 3000;
};

class WechatHook :public base::Singleton<WechatHook>{
 public:
  void Init(WechatHookParam param);

  int HookSyncMsg();
  int UnHookSyncMsg();

  int HookLog();

  int UnHookLog();

 private:
  WechatHookParam param_;
  bool sync_msg_flag_;
  bool init_;
};

}  // namespace hook

#endif