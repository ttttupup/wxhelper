#ifndef WXHELPER_HTTP_SERVER_H_
#define WXHELPER_HTTP_SERVER_H_

#include <mongoose.h>

#include "http_handler.h"
namespace wxhelper {
class HttpServer {
 public:
  static HttpServer &GetInstance();
  bool HttpStart();
  bool HttpClose();
  void Init(int port);

 private:
  HttpServer(){};
  HttpServer(const HttpServer &) = delete;
  HttpServer &operator=(const HttpServer &) = delete;
  ~HttpServer();
  static void StartHttpServer();
  static void EventHandler(struct mg_connection *c, int ev, void *ev_data,
                           void *fn_data);
  void HandleHttpRequest(struct mg_connection *c, void *ev_data);
  void HandleWebsocketRequest(struct mg_connection *c, void *ev_data);

 private:
  int port_;
  bool running_;
  struct mg_mgr mgr_;
  HttpHandler *http_handler_;
  HANDLE thread_;
};
}  // namespace wxhelper

#endif
