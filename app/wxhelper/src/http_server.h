#ifndef WXHELPER_HTTP_SERVER_H_
#define WXHELPER_HTTP_SERVER_H_
#include <atomic>
#include <functional>
#include <map>
#include <string>
#include "mongoose.h"
#include "singleton.h"
namespace http {

class HttpServer : public base::Singleton<HttpServer> {
 public:
  ~HttpServer();
  void init(std::string &&host, int port);
  bool Start();
  bool Stop();
  const mg_mgr *GetMgr() { return &mgr_; }
  int GetPort() { return port_; }
  bool GetRunning() { return running_; }
  std::string GetHost() { return host_; }

 private:
  static void LaunchServer(HttpServer *server);
  static void EventHandler(struct mg_connection *c, int ev, void *ev_data,
                           void *fn_data);
  static void HandleHttpRequest(struct mg_connection *c, void *ev_data,
                                void *fn_data);
  static void HandleWebsocketRequest(struct mg_connection *c, void *ev_data,
                                     void *fn_data);

 private:
  struct mg_mgr mgr_;
  std::string host_;
  int port_;
  std::atomic<bool> running_{false};
  HANDLE thread_;
};

}  // namespace http

#endif