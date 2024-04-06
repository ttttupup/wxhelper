#include "chat_controller.h"

#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"
#include "utils.h"
#include "wechat_interface.h"
#include "wxutils.h"
#include "offset.h"
#include "json_utils.h"

namespace offset = wechat::offset;
namespace prototype = wechat::prototype;
namespace func = wechat::function;
namespace utils = base::utils;
namespace jsonutils = wxhelper::jsonutils;
namespace wxhelper {

std::string ChatController::SendTextMsg(std::string params) {
  return std::string();
}
std::string ChatController::SendImageMsg(std::string params) {
  return std::string();
}
std::string ChatController::SendFileMsg(std::string params) {
  return std::string();
}
std::string ChatController::SendAtText(std::string params) {
  return std::string();
}
std::string ChatController::SendMultiAtText(std::string params) {
  return std::string();
}
std::string ChatController::ForwardMsg(std::string params) {
  return std::string();
}
}  // namespace wxhelper