#include "config.h"

#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include "windows.h"
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
namespace wxhelper {
Config::Config(/* args */) {}

Config::~Config() {}

void Config::Initialize() {
  port_ = GetPrivateProfileInt("config", "Port", 19088, "./config.ini");
  hidden_dll_ =
      GetPrivateProfileInt("config", "HiddenDll", 1, "./config.ini");
  auto logger =
      spdlog::daily_logger_mt("daily_logger", "logs/daily.txt", 23, 59);
  logger->flush_on(spdlog::level::err);
  spdlog::set_default_logger(logger);
  spdlog::flush_every(std::chrono::seconds(3));
  spdlog::set_level(spdlog::level::debug);
  spdlog::set_pattern("%Y-%m-%d %H:%M:%S [%l] [%t] - <%s>|<%#>|<%!>,%v");
}
int Config::GetPort() { return port_; }

int Config::GetHideDll() { return hidden_dll_; }

}  // namespace wxhelper
