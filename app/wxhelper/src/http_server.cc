#include "http_server.h"

#include "http_router.h"
#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"
#include "utils.h"

namespace http {

HttpServer::~HttpServer() {
  if (thread_ != nullptr) {
    CloseHandle(thread_);
  }
  mg_mgr_free(&mgr_);
}

void HttpServer::init(std::string &&host, int port) {
  if (port < 1 || port > 65535) {
    throw std::invalid_argument("Port number is out of range.");
  }
  mg_mgr_init(&mgr_);
  host_ = std::move(host);
  port_ = port;
  SPDLOG_INFO("http init :host={},port={}",host_,port_);
}

bool HttpServer::Start() {
  if (running_) {
    SPDLOG_INFO("http server is already running");
    return true;
  }
#ifdef _DEBUG
  base::utils::CreateConsole();
#endif
  running_.store(true);
  thread_ = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LaunchServer, this,
                         NULL, 0);
  SPDLOG_INFO("CreateThread for http server ,the result is {}",thread_ != NULL);
  return thread_ != NULL;
}

bool HttpServer::Stop() {
  if (!running_) {
    return true;
  }
#ifdef _DEBUG
  base::utils::CloseConsole();
#endif
  running_ = false;
  if (thread_) {
    WaitForSingleObject(thread_, -1);
    CloseHandle(thread_);
    thread_ = NULL;
  }
  return true;
}

void HttpServer::LaunchServer(HttpServer *server) {
  int port = server->GetPort();
  std::string host = server->GetHost();
  const mg_mgr * mgr = server->GetMgr();
  std::string listen_addr = host +":"+ std::to_string(port);
  if (mg_http_listen(const_cast<mg_mgr *>(mgr), listen_addr.c_str(),
                     EventHandler, NULL) == NULL) {
    SPDLOG_ERROR("http server  listen fail.port:{}", port);
    return;
  }
  for (;;) {
    mg_mgr_poll(const_cast<mg_mgr *>(mgr), 100);
  }
}

void HttpServer::EventHandler(struct mg_connection *c, int ev, void *ev_data,
                              void *fn_data) {
  if (ev == MG_EV_OPEN) {
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *)ev_data;
    if (mg_http_match_uri(hm, "/websocket/*")) {
      mg_ws_upgrade(c, hm, NULL);
    } else if (mg_http_match_uri(hm, "/api/*")) {
      HandleHttpRequest(c, hm, fn_data);
    } else {
      nlohmann::json res = {{"code", 400},
                            {"msg", "invalid url, please check url"},
                            {"data", NULL}};
      std::string ret = res.dump();
      mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s\n",
                    ret.c_str());
    }
  } else if (ev == MG_EV_WS_MSG) {
    HandleWebsocketRequest(c, ev_data, fn_data);
  }
}

void HttpServer::HandleHttpRequest(struct mg_connection *c, void *ev_data,
                                   void *fn_data) {
  struct mg_http_message *hm = (struct mg_http_message *)ev_data;
  std::string ret = R"({"code":200,"msg":"success"})";
  try {
    if (mg_vcasecmp(&hm->method, "GET") == 0) {
      nlohmann::json ret_data = {
          {"code", 200},
          {"data", {}},
          {"msg", "the get method is not supported.please use post method."}};
      ret = ret_data.dump();
    } else if (mg_vcasecmp(&hm->method, "POST") == 0) {
      std::string url(hm->uri.ptr, hm->uri.len);
      std::string p = "{}";
      if (hm->body.len > 0){
        nlohmann::json params =
            nlohmann::json::parse(hm->body.ptr, hm->body.ptr + hm->body.len);
        p = params.dump();
      }
      ret = HttpRouter::GetInstance().HandleHttpRequest(url, p);
    }
  } catch (nlohmann::json::exception &e) {
    nlohmann::json res = {{"code", "500"}, {"msg", e.what()}, {"data", NULL}};
    ret = res.dump();
  }
  mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s\n",
                  ret.c_str());
}

void HttpServer::HandleWebsocketRequest(struct mg_connection *c, void *ev_data,
                                        void *fn_data) {
  struct mg_ws_message *wm = (struct mg_ws_message *)ev_data;
  std::string ret = R"({"code":200,"msg":"success"})";
  try {
    nlohmann::json params =
        nlohmann::json::parse(wm->data.ptr, wm->data.ptr + wm->data.len);
    std::string cmd = params["cmd"];
    std::string p = params.dump();
    ret = HttpRouter::GetInstance().HandleHttpRequest(cmd, p);
  } catch (nlohmann::json::exception &e) {
    nlohmann::json res = {{"code", "500"}, {"msg", e.what()}, {"data", NULL}};
    ret = res.dump();
  }
  mg_ws_send(c, ret.c_str(), ret.size(), WEBSOCKET_OP_TEXT);
}

}  // namespace http