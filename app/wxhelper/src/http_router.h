#ifndef WXHELPER_HTTP_ROUTER_H_
#define WXHELPER_HTTP_ROUTER_H_
#include <functional>
#include <unordered_map>
#include <string>

#include "singleton.h"

namespace http {
typedef std::function<std::string(std::string)> HttpHandler;

class HttpRouter : public base::Singleton<HttpRouter> {
 public:
  void HttpRouter::AddPathRouting(const std::string &path, HttpHandler handler);

  std::string HandleHttpRequest(const std::string &path,
                                const std::string &param);

 private:
  std::unordered_map<std::string, HttpHandler> route_table_{};
};
}  // namespace http
#endif