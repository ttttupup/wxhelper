#include "http_server.h"

#include <nlohmann/json.hpp>

#include "spdlog/spdlog.h"
#include "utils.h"
namespace http {

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

bool HttpServer::Start() {
  if (running_) {
    return true;
  }
#ifdef _DEBUG
  base::utils::CreateConsole();
#endif
  running_ = true;
  thread_ = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartHttpServer, this,
                         NULL, 0);
  return true;
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

int HttpServer::GetPort() { return port_; }
bool HttpServer::GetRunning() { return running_; }

const mg_mgr *HttpServer::GetMgr() { return &mgr_; }

void HttpServer::AddHttpApiUrl(const std::string &uri, HttpCbFunc func) {
  http_api_url_map_[uri] = func;
}

void HttpServer::StartHttpServer(HttpServer *server) {
  int port = server->GetPort();
  std::string lsten_addr = "http://0.0.0.0:" + std::to_string(port);
  if (mg_http_listen(const_cast<mg_mgr *>(server->GetMgr()), lsten_addr.c_str(),
                     EventHandler, server) == NULL) {
    SPDLOG_INFO("http server  listen fail.port:{}", port);
    return;
  }
  for (;;) {
    mg_mgr_poll(const_cast<mg_mgr *>(server->GetMgr()), 100);
  }
}

void HttpServer::EventHandler(struct mg_connection *c, int ev, void *ev_data,
                              void *fn_data) {
  if (ev == MG_EV_OPEN) {
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *)ev_data;
    if (mg_http_match_uri(hm, "/websocket")) {
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
  (void)fn_data;
}

void HttpServer::HandleHttpRequest(struct mg_connection *c, void *ev_data,
                                   void *fn_data) {
  struct mg_http_message *hm = (struct mg_http_message *)ev_data;
  std::string ret = R"({"code":200,"msg":"success"})";
  try {
    ret = HttpDispatch(c, hm, fn_data);
  } catch (nlohmann::json::exception &e) {
    nlohmann::json res = {{"code", "500"}, {"msg", e.what()}, {"data", NULL}};
    ret = res.dump();
  }
  if (ret != "") {
    mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s\n",
                  ret.c_str());
  }
}

void HttpServer::HandleWebsocketRequest(struct mg_connection *c, void *ev_data,
                                        void *fn_data) {
  // Got websocket frame. Received data is wm->data. Echo it back!
  struct mg_ws_message *wm = (struct mg_ws_message *)ev_data;
  mg_ws_send(c, wm->data.ptr, wm->data.len, WEBSOCKET_OP_TEXT);
}

std::string HttpServer::HttpDispatch(struct mg_connection *c,
                                     struct mg_http_message *hm,
                                     void *fn_data) {
  std::string ret;
  if (mg_vcasecmp(&hm->method, "GET") == 0) {
    nlohmann::json ret_data = {
        {"code", 200},
        {"data", {}},
        {"msg", "the get method is not supported.please use post method."}};
    ret = ret_data.dump();
    return ret;
  }
  std::string api_uri(hm->uri.ptr,hm->uri.len);
  HttpServer *server = (HttpServer *)fn_data;
  return server->ProcessHttpRequest(api_uri, hm);
}

std::string HttpServer::ProcessHttpRequest(const std::string &url,
                                           mg_http_message *hm) {
  SPDLOG_INFO("http server  process request url :{}", url);
  if (http_api_url_map_.find(url) != http_api_url_map_.end()) {
    return http_api_url_map_[url](hm);
  } else {
    nlohmann::json ret_data = {
        {"code", 200}, {"data", {}}, {"msg", "the url is not supported"}};
    return ret_data.dump();
  }
}

}  // namespace http