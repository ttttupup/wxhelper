#include <winsock2.h>
#include "wxhelper.h"

#include "config.h"
#include "http_server.h"
#include "thread_pool.h"
#include "utils.h"
#include "wxutils.h"
#include "wechat_db.h"
#include "sync_msg_hook.h"
#include "wechat_service.h"
namespace wxhelper {

void WxHelper::init(HMODULE module) {
  Config::GetInstance().init();
  if (Config::GetInstance().GetHideDll()) {
    base::utils::HideModule(module);
  }
  wechat::WeChatDb::GetInstance().Init();
  wechat::WeChatService::GetInstance().Init();
  SyncMsgHook::GetInstance().Init();
  http::HttpServer::GetInstance().init(
      Config::GetInstance().GetHttpServerHost(),
      Config::GetInstance().GetHttpServerPort());
  http::HttpServer::GetInstance().Start();
  base::ThreadPool::GetInstance().Create(2, 8);
}
void WxHelper::finally() { http::HttpServer::GetInstance().Stop(); }
}  // namespace wxhelper