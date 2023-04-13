#include "log.h"

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP
namespace wxhelper {
Log::Log(/* args */) {}

Log::~Log() {}

void Log::initialize() {

  el::Configurations conf;
  // 启用日志
  conf.setGlobally(el::ConfigurationType::Enabled, "true");
  // 设置日志文件目录以及文件名
  conf.setGlobally(el::ConfigurationType::Filename,
                   "log\\log_%datetime{%Y%M%d %H%m%s}.log");
  // 设置日志文件最大文件大小
  conf.setGlobally(el::ConfigurationType::MaxLogFileSize, "20971520");
  // 是否写入文件
  conf.setGlobally(el::ConfigurationType::ToFile, "true");
  // 是否输出控制台
  conf.setGlobally(el::ConfigurationType::ToStandardOutput, "true");
  // 设置日志输出格式
  conf.setGlobally(el::ConfigurationType::Format,
                   "[%datetime] [%thread] [%loc] [%level] : %msg");
  // 设置日志文件写入周期，如下每100条刷新到输出流中
  #ifdef _DEBUG
  conf.setGlobally(el::ConfigurationType::LogFlushThreshold, "1");
  #else
  conf.setGlobally(el::ConfigurationType::LogFlushThreshold, "100");
  #endif
  // 设置配置文件
  el::Loggers::reconfigureAllLoggers(conf);
}

}  // namespace wxhelper