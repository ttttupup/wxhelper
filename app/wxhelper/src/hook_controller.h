#ifndef WXHELPER_HOOK_CONTROLLER_H_
#define WXHELPER_HOOK_CONTROLLER_H_

#include "http_controller.h"

namespace wxhelper {
class HookController : public http::HttpController<HookController> {
 public:
  PATHS_BEGIN
  ADD_PATH("/api/hookSyncMsg", HookSyncMsg);
  PATHS_END

 public:
  static std::string HookSyncMsg(std::string params);
};
}  // namespace wxhelper

#endif