#include "wxhelper.h"

#include <mongoose.h>

#include "pch.h"

namespace wxhelper {
WXHelper& WXHelper::GetInstance() {
  static WXHelper p;
  return p;
}

int WXHelper::http_start(int port) {
  HttpServer::GetInstance().Init(port);
  bool ret = HttpServer::GetInstance().HttpStart();
  return ret == true ? 1 : 0;
}

int WXHelper::http_close() {
  bool ret = HttpServer::GetInstance().HttpClose();
  return ret == true ? 1 : 0;
}

}  // namespace wxhelper