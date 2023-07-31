#include "pch.h"
#include "global_context.h"
#include "thread_pool.h"
#include "db.h"

namespace wxhelper {

GlobalContext::~GlobalContext() {
  if (config.has_value()) {
    config.reset();
  }
  if (log.has_value()) {
    log.reset();
  }
  
}
void GlobalContext::initialize(HMODULE module) {
  state =GlobalContextState::INITIALIZING;
  module_ = module;
  #ifndef _DEBUG
  Utils::Hide(module);
  #endif
  UINT64 base = Utils::GetWeChatWinBase();
  config.emplace();
  config->Initialize();
  log.emplace();
  log->Initialize();
  http_server = std::unique_ptr<HttpServer>( new HttpServer(config->GetPort()));
  http_server->HttpStart();
  ThreadPool::GetInstance().Create(2, 8);
  mgr = std::unique_ptr<Manager>(new Manager(base));
  DB::GetInstance().init(base);
  state =GlobalContextState::INITIALIZED;
}

void GlobalContext::finally() {
  if (http_server) {
    http_server->HttpClose();
  }
}
}  // namespace wxhelper