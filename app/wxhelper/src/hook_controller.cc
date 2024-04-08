#include "hook_controller.h"

#include "json_utils.h"
#include "nlohmann/json.hpp"
#include "sync_msg_hook.h"

namespace jsonutils = wxhelper::jsonutils;
namespace wxhelper {
std::string HookController::HookSyncMsg(std::string params) {
  int success = SyncMsgHook::GetInstance().Hook();
  nlohmann::json ret_data = {
      {"code", success}, {"data", {}}, {"msg", "success"}};
  return ret_data.dump();
}
std::string HookController::UnHookSyncMsg(std::string params) {
  int success = SyncMsgHook::GetInstance().Unhook();
  nlohmann::json ret_data = {
      {"code", success}, {"data", {}}, {"msg", "success"}};
  return ret_data.dump();
}
std::string HookController::HookLog(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string HookController::UnHookLog(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
}  // namespace wxhelper