#ifndef WXHELPER_CONFIG_H_
#define WXHELPER_CONFIG_H_
#include <string>

#include "singleton.h"
namespace wxhelper {


class Config : public base::Singleton<Config> {
 public:
  void init();
  int GetHideDll();
  std::string GetHttpServerHost();
  int GetHttpServerPort();
  std::string GetRecvMessageMode();

  std::string GetRecvTcpIp();
  int GetRecvTcpPort();

  std::string GetRecvHttpUrl();
  int GetRecvHttpTimeout();

 private:
  int hidden_dll_;
  std::string http_server_host_;
  int http_server_port_;
  std::string recv_message_mode_;
  std::string recv_tcp_ip_;
  int recv_tcp_port_;
  std::string recv_http_url_;
  int recv_http_timeout_;
};
}  // namespace wxhelper
#endif