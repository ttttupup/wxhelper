#include "chat_room_controller.h"

#include "json_utils.h"
#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"
#include "wechat_interface.h"
#include "wechat_service.h"
namespace wxhelper {
std::string ChatRoomController::GetChatRoomDetailInfo(std::string params) {
  nlohmann::json jp = nlohmann::json::parse(params);
  SPDLOG_INFO("GetChatRoomDetailInfo chatRoomId={}", jp["chatRoomId"]);
  std::wstring room_id = jsonutils::GetWStringParam(jp, "chatRoomId");
  wechat::ChatRoomInfoInner chat_room_detail;
  int64_t success = wechat::WeChatService::GetInstance().GetChatRoomDetailInfo(
      room_id, chat_room_detail);
  nlohmann::json ret_data = {
      {"code", success}, {"msg", "success"}, {"data", {}}};
  nlohmann::json detail = {
      {"chatRoomId", chat_room_detail.chat_room_id},
      {"notice", chat_room_detail.notice},
      {"admin", chat_room_detail.admin},
      {"xml", chat_room_detail.xml},
  };
  ret_data["data"] = detail;
  return ret_data.dump();
}
std::string ChatRoomController::GetMemberFromChatRoom(std::string params) {
  nlohmann::json jp = nlohmann::json::parse(params);
  SPDLOG_INFO("GetMemberFromChatRoom chatRoomId={}", jp["chatRoomId"]);
  std::wstring room_id = jsonutils::GetWStringParam(jp, "chatRoomId");
  wechat::ChatRoomMemberInner member;
  int64_t success = wechat::WeChatService::GetInstance().GetMemberFromChatRoom(
      room_id, member);
  nlohmann::json ret_data = {{"code", success}, {"data", {}}, {"msg", "success"}};
  if (success > 0) {
    nlohmann::json member_info = {
        {"admin", member.admin},
        {"chatRoomId", member.chat_room_id},
        {"members", member.member},
        {"adminNickname", member.admin_nickname},
        {"memberNickname", member.member_nickname},
    };
    ret_data["data"] = member_info;
  }
  return ret_data.dump();
}
std::string ChatRoomController::AddMemberToChatRoom(std::string params) {
  nlohmann::json jp = nlohmann::json::parse(params);
  SPDLOG_INFO("AddMemberToChatRoom chatRoomId={},memberIds={}",
              jp["chatRoomId"], jp["memberIds"]);
  std::wstring room_id = jsonutils::GetWStringParam(jp, "chatRoomId");
  std::vector<std::wstring> wxids = jsonutils::GetArrayParam(jp, "memberIds");
  std::vector<std::wstring> wxid_list;
  for (unsigned int i = 0; i < wxids.size(); i++) {
    wxid_list.push_back(wxids[i]);
  }
  int64_t success = wechat::WeChatService::GetInstance().AddMemberToChatRoom(
      room_id, wxid_list);
  nlohmann::json ret = {
      {"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
std::string ChatRoomController::DelMemberFromChatRoom(std::string params) {
  nlohmann::json jp = nlohmann::json::parse(params);
  SPDLOG_INFO("DelMemberFromChatRoom chatRoomId={},memberIds={}",
              jp["chatRoomId"], jp["memberIds"]);
  std::wstring room_id = jsonutils::GetWStringParam(jp, "chatRoomId");
  std::vector<std::wstring> wxids = jsonutils::GetArrayParam(jp, "memberIds");
  std::vector<std::wstring> wxid_list;
  for (unsigned int i = 0; i < wxids.size(); i++) {
    wxid_list.push_back(wxids[i]);
  }
  INT64 success = wechat::WeChatService::GetInstance().DelMemberFromChatRoom(
      room_id, wxid_list);
  nlohmann::json ret_data = {
      {"code", success}, {"msg", "success"}, {"data", {}}};
  return ret_data.dump();
}
std::string ChatRoomController::CreateChatRoom(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string ChatRoomController::QuitChatRoom(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string ChatRoomController::InviteMemberToChatRoom(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string ChatRoomController::TopMsg(std::string params) {
  return std::string();
}
std::string ChatRoomController::RemoveTopMsg(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string ChatRoomController::GetChatRoomMemberNickname(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string ChatRoomController::ModifyChatRoomMemberNickName(
    std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
}  // namespace wxhelper
