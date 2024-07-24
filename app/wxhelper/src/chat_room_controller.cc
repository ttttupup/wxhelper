#include "chat_room_controller.h"

#include "json_utils.h"
#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"
#include "wechat_interface.h"
#include "wechat_service.h"
#include "utils.h"
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
  nlohmann::json ret_data = {
      {"code", success}, {"data", {}}, {"msg", "success"}};
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
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
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
  int64_t success = wechat::WeChatService::GetInstance().DelMemberFromChatRoom(
      room_id, wxid_list);
  nlohmann::json ret_data = {
      {"code", success}, {"msg", "success"}, {"data", {}}};
  return ret_data.dump();
}
std::string ChatRoomController::CreateChatRoom(std::string params) {
  SPDLOG_INFO("CreateChatRoom params:{}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::vector<std::wstring> wxids = jsonutils::GetArrayParam(jp, "memberIds");
  int64_t success = wechat::WeChatService::GetInstance().CreateChatRoom(wxids);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
std::string ChatRoomController::QuitChatRoom(std::string params) {
  SPDLOG_INFO("QuitChatRoom params:{}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring room_id = jsonutils::GetWStringParam(jp, "chatRoomId");
  int64_t success = wechat::WeChatService::GetInstance().QuitChatRoom(room_id);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
std::string ChatRoomController::InviteMemberToChatRoom(std::string params) {
  SPDLOG_INFO("InviteMemberToChatRoom params:{}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring room_id = jsonutils::GetWStringParam(jp, "chatRoomId");
  std::vector<std::wstring> wxids = jsonutils::GetArrayParam(jp, "memberIds");
  int64_t success = wechat::WeChatService::GetInstance().InviteMemberToChatRoom(
      room_id, wxids);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
std::string ChatRoomController::TopMsg(std::string params) {
  SPDLOG_INFO("TopMsg params:{}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  int64_t msg_id = jsonutils::GetInt64Param(jp, "msgId");
  int64_t success = wechat::WeChatService::GetInstance().SetTopMsg(msg_id);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
std::string ChatRoomController::RemoveTopMsg(std::string params) {
  SPDLOG_INFO("RemoveTopMsg params:{}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring room_id = jsonutils::GetWStringParam(jp, "chatRoomId");
  int64_t msg_id = jsonutils::GetInt64Param(jp, "msgId");
  int64_t success =
      wechat::WeChatService::GetInstance().RemoveTopMsg(room_id, msg_id);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
std::string ChatRoomController::GetChatRoomMemberNickname(std::string params) {
  SPDLOG_INFO("GetChatRoomMemberNickname params:{}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring room_id = jsonutils::GetWStringParam(jp, "chatRoomId");
  std::wstring member_id = jsonutils::GetWStringParam(jp, "memberId");
  std::wstring name = wechat::WeChatService::GetInstance().GetChatRoomMemberNickname(room_id,
                                                                     member_id);
  nlohmann::json ret = {
      {"code", 1},
      {"data", {{"nickname", base::utils::WstringToUtf8(name)}}},
      {"msg", "success"}};
  return ret.dump();
}
std::string ChatRoomController::ModifyChatRoomMemberNickName(
    std::string params) {
  SPDLOG_INFO("ModifyChatRoomMemberNickName params:{}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring room_id = jsonutils::GetWStringParam(jp, "chatRoomId");
  std::wstring wxid = jsonutils::GetWStringParam(jp, "wxid");
  std::wstring nickName = jsonutils::GetWStringParam(jp, "nickName");
  int64_t success =
      wechat::WeChatService::GetInstance().ModChatRoomMemberNickName(
          room_id, wxid, nickName);
  nlohmann::json ret = {{"code", success}, {"msg", "success"}, {"data", {}}};
  return ret.dump();
}
}  // namespace wxhelper
