#include <winsock2.h>
#include "wxhelper.h"



#include "config.h"
#include "http_server.h"
#include "thread_pool.h"
#include "utils.h"
#include "wxutils.h"
#include "chat_controller.h"
namespace wxhelper {

void WxHelper::init(HMODULE module) {
  Config::GetInstance().init();
  if (Config::GetInstance().GetHideDll()) {
    // base::utils::HideModule(module);
  }
    // ChatController chatController;
  // http::HttpRouter::GetInstance().init();
  http::HttpServer::GetInstance().init(
      Config::GetInstance().GetHttpServerHost(),
      Config::GetInstance().GetHttpServerPort());
  http::HttpServer::GetInstance().Start();
  base::ThreadPool::GetInstance().Create(2, 8);
}
void WxHelper::finally() { http::HttpServer::GetInstance().Stop(); }
}  // namespace wxhelper