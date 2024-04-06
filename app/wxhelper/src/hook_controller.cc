#include "hook_controller.h"

#include "json_utils.h"
#include "nlohmann/json.hpp"
#include "sync_msg_hook.h"

namespace jsonutils = wxhelper::jsonutils;
namespace wxhelper {
std::string HookController::HookSyncMsg(std::string params) {
  SyncMsgHook::GetInstance().Init();
  int success = SyncMsgHook::GetInstance().Hook();
  nlohmann::json ret_data = {
      {"code", success}, {"data", {}}, {"msg", "success"}};
  return ret_data.dump();
}
}  // namespace wxhelper