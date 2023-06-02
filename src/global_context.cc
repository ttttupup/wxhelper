#include "pch.h"
#include "global_context.h"
#include "http_server.h"
#include "hooks.h"
#include "thread_pool.h"


namespace wxhelper {

void GlobalContext::initialize(HMODULE module) {
  module_ = module;
  DWORD base = Utils::GetWeChatWinBase();
  config.emplace();
  config->Initialize();
  log.emplace();
  log->Initialize();
  hide_module.emplace();
  #ifndef _DEBUG
  hide_module->Hide(module_);
  #endif
  ThreadPool::GetInstance().Create(2,8);
  HttpServer::GetInstance().Init(config->GetPort());
  HttpServer::GetInstance().HttpStart();
  DB::GetInstance().init(base);
  contact_mgr.emplace(ContactMgr{base});
  misc_mgr.emplace(MiscMgr{base});
  send_mgr.emplace(SendMessageMgr{base});
  account_mgr.emplace(AccountMgr{base});
  chat_room_mgr.emplace(ChatRoomMgr{base});
  sns_mgr.emplace(SNSMgr{base});
}

void GlobalContext::finally() { 
  HttpServer::GetInstance().HttpClose(); 
  hooks::UnHookLog();
  hooks::UnHookRecvMsg();
  hooks::UnHookSearchContact();
}
}  // namespace wxhelper