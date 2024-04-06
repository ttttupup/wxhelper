#include "chat_controller.h"

#include "json_utils.h"
#include "nlohmann/json.hpp"
#include "offset.h"
#include "spdlog/spdlog.h"
#include "utils.h"
#include "wechat_interface.h"
#include "wxutils.h"

namespace offset = wechat::offset;
namespace prototype = wechat::prototype;
namespace func = wechat::function;
namespace utils = base::utils;
namespace jsonutils = wxhelper::jsonutils;
namespace wxhelper {

std::string ChatController::SendTextMsg(std::string params) {
  int64_t base_addr = wxutils::GetWeChatWinBase();
  nlohmann::json jp = nlohmann::json::parse(params);
  SPDLOG_INFO("sendTextMsg wxid={},msg={}",jp["wxid"],jp["msg"]);
  std::wstring wxid = jsonutils::GetWStringParam(jp, "wxid");
  std::wstring msg = jsonutils::GetWStringParam(jp, "msg");
  prototype::WeChatString to_user(wxid);
  prototype::WeChatString text_msg(msg);
  uint64_t send_message_mgr_addr = base_addr + offset::kGetSendMessageMgr;
  uint64_t send_text_msg_addr = base_addr + offset::kSendTextMsg;
  uint64_t free_chat_msg_addr = base_addr + offset::kFreeChatMsg;
  char chat_msg[0x460] = {0};
  uint64_t temp[3] = {0};
  func::__GetSendMessageMgr mgr;
  mgr = (func::__GetSendMessageMgr)send_message_mgr_addr;
  func::__SendTextMsg send;
  send = (func::__SendTextMsg)send_text_msg_addr;
  func::__FreeChatMsg free;
  free = (func::__FreeChatMsg)free_chat_msg_addr;
  mgr();
  uint64_t success = send(reinterpret_cast<UINT64>(&chat_msg),
                          reinterpret_cast<UINT64>(&to_user),
                          reinterpret_cast<UINT64>(&text_msg),
                          reinterpret_cast<UINT64>(&temp), 1, 1, 0, 0);
  free(reinterpret_cast<UINT64>(&chat_msg));

  nlohmann::json ret_data = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret_data.dump();
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