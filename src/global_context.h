#ifndef GLOBAL_CONTEXT_H_
#define GLOBAL_CONTEXT_H_
#include "account_mgr.h"
#include "config.h"
#include "contact_mgr.h"
#include "db.h"
#include "hide_module.h"
#include "http_server.h"
#include "log.h"
#include "misc_mgr.h"
#include "send_message_mgr.h"
#include "chat_room_mgr.h"
#include "sns_mgr.h"
#include "singleton.h"

namespace wxhelper {

enum class GlobalContextState { NOT_INITIALIZED, INITIALIZING, INITIALIZED };

class GlobalContext :public Singleton<GlobalContext>{
 public:
  void initialize(HMODULE module);
  void finally();

 public:
  std::optional<Config> config;
  std::optional<HideModule> hide_module;
  std::optional<Log> log;
  std::optional<ContactMgr> contact_mgr;
  std::optional<MiscMgr> misc_mgr;
  std::optional<SendMessageMgr> send_mgr;
  std::optional<AccountMgr> account_mgr;
  std::optional<ChatRoomMgr> chat_room_mgr;
  std::optional<SNSMgr> sns_mgr;

  GlobalContextState state = GlobalContextState::INITIALIZED;

 private:
  HMODULE module_;
};

}  // namespace wxhelper
#endif