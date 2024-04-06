#ifndef WXHELPER_CLIENT_SOCKET_H_
#define WXHELPER_CLIENT_SOCKET_H_
#include <string>

#include "noncopyable.h"
namespace wxhelper {
class SocketInit : public NonCopyable {
 public:
  SocketInit();

  ~SocketInit();

  bool valid_{false};
};

static SocketInit kSocketInit;

class TcpClient : public NonCopyable {
 public:
  TcpClient()=default;
  TcpClient(std::string ip, int port);
  void SendAndCloseSocket(std::string& content);

 private:
  std::string ip_;
  int port_;
};

}  // namespace wxhelper
#endif