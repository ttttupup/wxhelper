#ifndef WXHELPER_HTTP_SERVER_H_
#define WXHELPER_HTTP_SERVER_H_

#include <functional>
#include <map>
#include <string>

#include "mongoose.h"
namespace http {
typedef std::function<std::string(struct mg_http_message *)> HttpCbFunc;
class HttpServer {
 public:
  explicit HttpServer(int port);
  HttpServer(const HttpServer &) = delete;
  HttpServer(HttpServer &&) = delete;
  HttpServer &operator=(const HttpServer &) = delete;
  ~HttpServer();

  bool Start();
  bool Stop();
  int GetPort();
  bool GetRunning();
  const mg_mgr *GetMgr();
  void AddHttpApiUrl(const std::string &uri, HttpCbFunc func);

 private:
  int port_;
  bool running_;
  struct mg_mgr mgr_;
  HANDLE thread_;
  std::map<const std::string, HttpCbFunc> http_api_url_map_;

  static void StartHttpServer(HttpServer *server);
  static void EventHandler(struct mg_connection *c, int ev, void *ev_data,
                           void *fn_data);

  static void HandleHttpRequest(struct mg_connection *c, void *ev_data,
                                void *fn_data);
  static void HandleWebsocketRequest(struct mg_connection *c, void *ev_data,
                                     void *fn_data);
  static std::string HttpDispatch(struct mg_connection *c,
                                  struct mg_http_message *hm, void *fn_data);
  std::string ProcessHttpRequest(const std::string &url,
                                 struct mg_http_message *hm);
};

}  // namespace http

#endif