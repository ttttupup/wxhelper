#include "pch.h"
#include "log.h"
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
namespace wxhelper {
Log::Log(/* args */) {}

Log::~Log() {

}

void Log::Initialize() {
    auto logger = spdlog::daily_logger_mt("daily_logger", "logs/daily.txt", 23, 59);
    logger->flush_on(spdlog::level::err);
    spdlog::set_default_logger(logger);
    spdlog::flush_every(std::chrono::seconds(3));
    spdlog::set_level(spdlog::level::debug); 
    spdlog::set_pattern("%Y-%m-%d %H:%M:%S [%l] [%t] - <%s>|<%#>|<%!>,%v");
}

}  // namespace wxhelper