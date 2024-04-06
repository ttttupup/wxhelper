#include "client_socket.h"

#include <WS2tcpip.h>
#include <Winsock2.h>

#include "spdlog/spdlog.h"
namespace wxhelper {

SocketInit::SocketInit() {
  WSADATA was_data = {0};
  if (WSAStartup(MAKEWORD(2, 2), &was_data)) {
    valid_ = true;
  }
}

SocketInit::~SocketInit() {
  if (valid_) {
    WSACleanup();
  }
}

TcpClient::TcpClient(std::string ip, int port) : ip_(ip), port_(port) {}

void TcpClient::SendAndCloseSocket(std::string& content) {
  SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (client_socket < 0) {
    SPDLOG_ERROR("socket init  fail");
    return;
  }
  BOOL status = false;
  sockaddr_in client_addr;
  memset(&client_addr, 0, sizeof(client_addr));
  client_addr.sin_family = AF_INET;
  client_addr.sin_port = htons((u_short)port_);
  InetPtonA(AF_INET, ip_.c_str(), &client_addr.sin_addr.s_addr);
  if (connect(client_socket, reinterpret_cast<sockaddr*>(&client_addr),
              sizeof(sockaddr)) < 0) {
    SPDLOG_ERROR("socket connect  fail. host:{} , port:{},", ip_, port_);
    closesocket(client_socket);
    return;
  }
  char recv_buf[1024] = {0};
  int ret =
      send(client_socket, content.c_str(), static_cast<int>(content.size()), 0);
  if (ret < 0) {
    SPDLOG_ERROR("socket send  fail ,ret:{}", ret);
    closesocket(client_socket);
    return;
  }
  ret = shutdown(client_socket, SD_SEND);
  if (ret == SOCKET_ERROR) {
    SPDLOG_ERROR("shutdown failed with erro:{}", ret);
    closesocket(client_socket);
    return;
  }
  do {
    ret = recv(client_socket, recv_buf, sizeof(recv_buf), 0);
    if (ret > 0) {
    } else if (ret == 0) {
      SPDLOG_INFO("Connection closed");
      closesocket(client_socket);
      return;
    } else {
      SPDLOG_ERROR("recv failed with error:{}", WSAGetLastError());
      closesocket(client_socket);
      return;
    }
  } while (ret > 0);
}
}  // namespace wxhelper