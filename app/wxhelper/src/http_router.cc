#include "http_router.h"

#include <iostream>

#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"
namespace http {
void HttpRouter::AddPathRouting(const std::string &path, HttpHandler handler) {
  route_table_[path] = handler;
  SPDLOG_INFO("route table size={}", route_table_.size());
}
std::string HttpRouter::HandleHttpRequest(const std::string &path,
                                          const std::string &param) {
  SPDLOG_INFO("route table size={}", route_table_.size());
  auto it = route_table_.find(path);
  if (it != route_table_.end()) {
    return it->second(param);
  } else {
    nlohmann::json ret_data = {
        {"code", 200}, {"data", {}}, {"msg", "the url is not supported"}};
    return ret_data.dump();
  }
}
}  // namespace http