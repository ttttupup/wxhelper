#ifndef WXHELPER_GLOBAL_MANAGER_H_
#define WXHELPER_GLOBAL_MANAGER_H_
#include <memory>

#include "config.h"
#include "http_server.h"
#include "singleton.h"

namespace wxhelper {

enum class GlobalContextState { NOT_INITIALIZED, INITIALIZING, INITIALIZED };

class GlobalManager : public base::Singleton<GlobalManager> {
  friend class base::Singleton<GlobalManager>;
  ~GlobalManager();

 public:
  void initialize(HMODULE module);
  void finally();

 public:
  std::unique_ptr<Config> config;
  std::unique_ptr<http::HttpServer> http_server;
  // std::unique_ptr<WechatService> service_;

  GlobalContextState state = GlobalContextState::NOT_INITIALIZED;

 private:
  HMODULE module_;
};

}  // namespace wxhelper
#endif