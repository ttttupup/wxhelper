#ifndef WXHELPER_HOOK_CONTROLLER_H_
#define WXHELPER_HOOK_CONTROLLER_H_

#include "http_controller.h"

namespace wxhelper {
class HookController : public http::HttpController<HookController> {
 public:
  PATHS_BEGIN
  ADD_PATH("/api/hookSyncMsg", HookSyncMsg);
  ADD_PATH("/api/unhookSyncMsg", UnHookSyncMsg);
  ADD_PATH("/api/hookLog", HookLog);
  ADD_PATH("/api/unhookLog", UnHookLog);
  PATHS_END

 public:
  /// @brief hook消息
  /// @param params json
  /// @return json
  static std::string HookSyncMsg(std::string params);
  /// @brief 取消hook消息
  /// @param params json
  /// @return json
  static std::string UnHookSyncMsg(std::string params);
  /// @brief hook日志消息
  /// @param params json
  /// @return json
  static std::string HookLog(std::string params);
  /// @brief 取消hook日志消息
  /// @param params json
  /// @return json
  static std::string UnHookLog(std::string params);
};
}  // namespace wxhelper

#endif