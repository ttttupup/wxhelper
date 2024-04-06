#include "config.h"

#include <Windows.h>

#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
namespace wxhelper {

void Config::init() {
  auto logger =
      spdlog::daily_logger_mt("daily_logger", "logs/daily.txt", 23, 59);
  logger->flush_on(spdlog::level::err);
  spdlog::set_default_logger(logger);
  spdlog::flush_every(std::chrono::seconds(3));
  spdlog::set_level(spdlog::level::info);
  spdlog::set_pattern("%Y-%m-%d %H:%M:%S [%l] [%t] - <%s>|<%#>|<%!>,%v");
  http_server_port_ =
      GetPrivateProfileInt("config", "HttpServerPort", 19088, "./config.ini");
  hidden_dll_ = GetPrivateProfileInt("config", "HiddenDll", 1, "./config.ini");
  TCHAR host[MAX_PATH];
  GetPrivateProfileStringA("config", "HttpServerHost", "http://0.0.0.0", host,
                           sizeof(host), "./config.ini");
  http_server_host_ = host;
  TCHAR mode[MAX_PATH];
  GetPrivateProfileString("config", "RecvMessageMode", "tcp", mode,
                          sizeof(mode), "./config.ini");
  recv_message_mode_ = mode;

  TCHAR tcp_ip[MAX_PATH];
  GetPrivateProfileString("config", "RecvTcpIp", "127.0.0.1", tcp_ip,
                          sizeof(tcp_ip), "./config.ini");
  recv_tcp_ip_ = tcp_ip;
  recv_tcp_port_ =
      GetPrivateProfileInt("config", "RecvTcpPort", 19099, "./config.ini");
  TCHAR url[MAX_PATH];
  GetPrivateProfileString("config", "RecvHttpUrl", "127.0.0.1", url,
                          sizeof(url), "./config.ini");
  recv_http_url_ = url;
  recv_http_timeout_ =
      GetPrivateProfileInt("config", "RecvHttpTimeout", 3000, "./config.ini");
  SPDLOG_INFO(
      "wxhelper config:"
      "HttpServerPort={},HttpServerHost={},RecvMessageMode={},RecvTcpIp="
      "{},RecvTcpPort={},RecvHttpUrl={},RecvHttpTimeout={}",
      http_server_port_, http_server_host_, recv_message_mode_, recv_tcp_ip_,
      recv_tcp_port_, recv_http_url_, recv_http_timeout_);
}

int Config::GetHideDll() { return hidden_dll_; }
std::string Config::GetHttpServerHost() { return http_server_host_; }
int Config::GetHttpServerPort() { return http_server_port_; }
std::string Config::GetRecvMessageMode() { return recv_message_mode_; }

std::string Config::GetRecvTcpIp() { return recv_tcp_ip_; }
int Config::GetRecvTcpPort() { return recv_tcp_port_; }

std::string Config::GetRecvHttpUrl() { return recv_http_url_; }
int Config::GetRecvHttpTimeout() { return recv_http_timeout_; }

}  // namespace wxhelper
