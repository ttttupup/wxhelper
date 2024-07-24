#include "chat_controller.h"

#include "json_utils.h"
#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"
#include "wechat_interface.h"
#include "wechat_service.h"
#include "utils.h"
namespace jsonutils = wxhelper::jsonutils;
namespace wxhelper {

std::string ChatController::SendTextMsg(std::string params) {
  nlohmann::json jp = nlohmann::json::parse(params);
  SPDLOG_INFO("sendTextMsg wxid={},msg={}", jp["wxid"], jp["msg"]);
  std::wstring wxid = jsonutils::GetWStringParam(jp, "wxid");
  std::wstring msg = jsonutils::GetWStringParam(jp, "msg");
  int64_t success = wechat::WeChatService::GetInstance().SendTextMsg(wxid, msg);
  nlohmann::json ret_data = {
      {"code", success}, {"data", {}}, {"msg", "success"}};
  return ret_data.dump();
}

std::string ChatController::SendImageMsg(std::string params) {
  SPDLOG_INFO("SendImageMsg params={}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring wxid = jsonutils::GetWStringParam(jp, "wxid");
  std::wstring image_path = jsonutils::GetWStringParam(jp, "imagePath");
  int64_t success =
      wechat::WeChatService::GetInstance().SendImageMsg(wxid, image_path);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}

std::string ChatController::SendFileMsg(std::string params) {
  SPDLOG_INFO("SendFileMsg params={}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring wxid = jsonutils::GetWStringParam(jp, "wxid");
  std::wstring file_path = jsonutils::GetWStringParam(jp, "filePath");
  int64_t success =
      wechat::WeChatService::GetInstance().SendFileMsg(wxid, file_path);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}

std::string ChatController::SendAtText(std::string params) {
  SPDLOG_INFO("SendAtText params={}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring chat_room_id = jsonutils::GetWStringParam(jp, "chatRoomId");
  std::vector<std::wstring> wxids = jsonutils::GetArrayParam(jp, "wxids");
  std::wstring msg = jsonutils::GetWStringParam(jp, "msg");
  int64_t success =
      wechat::WeChatService::GetInstance().SendAtText(chat_room_id, wxids, msg);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}

std::string ChatController::SendMultiAtText(std::string params) {
  SPDLOG_INFO("SendMultiAtText params={}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  nlohmann::json array = jp["at"];
  std::vector<std::pair<std::wstring, std::wstring>> at;
  if (array.is_array()) {
    for (const auto& item : array) {
      at.push_back(std::make_pair(jsonutils::GetWStringParam(item, "wxid"),
                                  jsonutils::GetWStringParam(item, "msg")));
    }
  }
  std::wstring chat_room_id = jsonutils::GetWStringParam(jp, "chatRoomId");
  int64_t success =
      wechat::WeChatService::GetInstance().SendMultiAtText(chat_room_id, at);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}


std::string ChatController::SendCustomEmotion(std::string params) {
  SPDLOG_INFO("SendCustomEmotion params={}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring wxid = jsonutils::GetWStringParam(jp, "wxid");
  std::wstring file_path = jsonutils::GetWStringParam(jp, "filePath");
  int64_t success =
      wechat::WeChatService::GetInstance().SendCustomEmotion(file_path, wxid);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}

TODO("")
std::string ChatController::SendApplet(std::string params) {
  SPDLOG_INFO("SendApplet params={}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring wxid = jsonutils::GetWStringParam(jp, "wxid");
  std::wstring waid_concat = jsonutils::GetWStringParam(jp, "waidConcat");
  std::wstring waid = jsonutils::GetWStringParam(jp, "waid");
  std::wstring app_wxid = jsonutils::GetWStringParam(jp, "appletWxid");
  std::wstring json_param = jsonutils::GetWStringParam(jp, "jsonParam");
  std::wstring head_url = jsonutils::GetWStringParam(jp, "headImgUrl");
  std::wstring main_img = jsonutils::GetWStringParam(jp, "mainImg");
  std::wstring index_page = jsonutils::GetWStringParam(jp, "indexPage");
  int64_t success = wechat::WeChatService::GetInstance().SendApplet(
      wxid, waid_concat, waid, waid, app_wxid, json_param, head_url, main_img,
      index_page);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}

std::string ChatController::SendPatMsg(std::string params) {
  SPDLOG_INFO("SendPatMsg params={}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring room_id = jsonutils::GetWStringParam(jp, "receiver");
  std::wstring wxid = jsonutils::GetWStringParam(jp, "wxid");
  int64_t success =
      wechat::WeChatService::GetInstance().SendPatMsg(room_id, wxid);
  nlohmann::json ret_data = {
      {"code", success}, {"msg", "success"}, {"data", {}}};
  return ret_data.dump();
}

std::string ChatController::ForwardMsg(std::string params) {
  SPDLOG_INFO("ForwardMsg params={}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring wxid = jsonutils::GetWStringParam(jp, "wxid");
  int64_t msg_id = jsonutils::GetInt64Param(jp, "msgId");
  int64_t success =
      wechat::WeChatService::GetInstance().ForwardMsg(msg_id, wxid);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}

std::string ChatController::ForwardPublicMsgByMsgId(std::string params) {
  SPDLOG_INFO("ForwardPublicMsgByMsgId params={}", params);

  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring wxid = jsonutils::GetWStringParam(jp, "wxid");
  int64_t msg_id = jsonutils::GetInt64Param(jp, "msgId");
  int64_t success =
      wechat::WeChatService::GetInstance().ForwardPublicMsgByMsgId(wxid,
                                                                   msg_id);

  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}

std::string ChatController::ForwardPublicMsg(std::string params) {
  SPDLOG_INFO("ForwardPublicMsg params={}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring wxid = jsonutils::GetWStringParam(jp, "wxid");
  std::wstring appname = jsonutils::GetWStringParam(jp, "appName");
  std::wstring username = jsonutils::GetWStringParam(jp, "userName");
  std::wstring title = jsonutils::GetWStringParam(jp, "title");
  std::wstring url = jsonutils::GetWStringParam(jp, "url");
  std::wstring thumburl = jsonutils::GetWStringParam(jp, "thumbUrl");
  std::wstring digest = jsonutils::GetWStringParam(jp, "digest");
  INT64 success = wechat::WeChatService::GetInstance().ForwardPublicMsg(
      wxid, title, url, thumburl, username, appname, digest);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}

std::string ChatController::GetContactOrChatRoomNickname(std::string params) {
  SPDLOG_INFO("GetContactOrChatRoomNickname params={}", params);
  int64_t success = -1;
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring wxid = jsonutils::GetWStringParam(jp, "wxid");
  std::wstring nickname =
      wechat::WeChatService::GetInstance().GetContactOrChatRoomNickname(wxid);
  nlohmann::json ret = {
      {"code", success}, {"data", {"nickname", nickname}}, {"msg", "success"}};

  return ret.dump();
}

}  // namespace wxhelper