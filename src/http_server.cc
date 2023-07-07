#include "pch.h"
#include "http_server_callback.h"
#include "http_server.h"

namespace wxhelper {

HttpServer::HttpServer(int port) {
  port_ = port;
  running_ = false;
  mg_mgr_init(&mgr_);
}

HttpServer::~HttpServer() {
  if (thread_ != nullptr) {
    CloseHandle(thread_);
  }
  mg_mgr_free(&mgr_);
}

bool HttpServer::HttpStart() {
  if (running_) {
    return true;
  }
#ifdef _DEBUG
  Utils::CreateConsole();
#endif
  running_ = true;
  thread_ = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartHttpServer, this,
                         NULL, 0);
  return true;
}

bool HttpServer::HttpClose() {
  if (!running_) {
    return true;
  }
#ifdef _DEBUG
  Utils::CloseConsole();
#endif
  running_ = false;
  if (thread_) {
    WaitForSingleObject(thread_, -1);
    CloseHandle(thread_);
    thread_ = NULL;
  }
  return true;
}

int HttpServer::GetPort() { return port_; }
bool HttpServer::GetRunning() { return running_; }

const mg_mgr* HttpServer::GetMgr() { return &mgr_; }

}  // namespace wxhelper