#include "pch.h"
#include "http_server.h"

#include <nlohmann/json.hpp>

#include "api_route.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;
using namespace nlohmann;

namespace wxhelper {
HttpServer& HttpServer::GetInstance() {
  static HttpServer p;
  return p;
}

void HttpServer::Init(int port) {
  port_ = port;
  running_ = false;
  http_handler_ = new HttpHandler();
  mg_mgr_init(&mgr_);

}

HttpServer::~HttpServer() { 
  mg_mgr_free(&mgr_); 
  delete http_handler_;
}

bool HttpServer::HttpStart() {
  if (running_) {
    return true;
  }
#ifdef _DEBUG
  Utils::CreateConsole();
#endif
  running_ = true;
  thread_ = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartHttpServer, NULL,
               NULL, 0);
  return true;
}

void HttpServer::StartHttpServer() {
  string lsten_addr = "http://0.0.0.0:" + to_string(GetInstance().port_);
  mg_http_listen(&GetInstance().mgr_, lsten_addr.c_str(), EventHandler,
                 &GetInstance().mgr_);
  for (;;) mg_mgr_poll(&GetInstance().mgr_, 1000);
}

void HttpServer::EventHandler(struct mg_connection *c,
                              int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_OPEN) {
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *)ev_data;
    if (mg_http_match_uri(hm, "/websocket")) {
      mg_ws_upgrade(c, hm, NULL);
    } else if (mg_http_match_uri(hm, "/api/")) {
      GetInstance().HandleHttpRequest(c,hm);
    } else {
      mg_http_reply(c, 500, NULL, "%s", "Invalid URI");
    }
  } else if (ev == MG_EV_WS_MSG) {
   
    GetInstance().HandleWebsocketRequest(c,ev_data);
  }
  (void)fn_data;
}

void HttpServer::HandleHttpRequest(struct mg_connection *c,
                                    void *ev_data) {

  http_handler_->HandlerRequest(c,ev_data);
}

void HttpServer::HandleWebsocketRequest(struct mg_connection *c,
                                         void *ev_data) {
  // Got websocket frame. Received data is wm->data. Echo it back!
   struct mg_ws_message *wm = (struct mg_ws_message *)ev_data;
  mg_ws_send(c, wm->data.ptr, wm->data.len, WEBSOCKET_OP_TEXT);
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

}  // namespace wxhelper