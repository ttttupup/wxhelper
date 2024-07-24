#include "misc_controller.h"

#include "json_utils.h"
#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"
#include "wechat_interface.h"
#include "wechat_service.h"

namespace jsonutils = wxhelper::jsonutils;

namespace wxhelper {
std::string MiscController::CheckLogin(std::string params) {
  int64_t success = wechat::WeChatService::GetInstance().CheckLogin();
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
std::string MiscController::GetUserInfo(std::string params) {
  wechat::SelfInfoInner out;
  int64_t success = wechat::WeChatService::GetInstance().GetSelfInfo(out);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  nlohmann::json j_info = {
      {"name", out.name},
      {"city", out.city},
      {"province", out.province},
      {"country", out.country},
      {"account", out.account},
      {"wxid", out.wxid},
      {"mobile", out.mobile},
      {"headImage", out.head_img},
      {"signature", out.signature},
      {"dataSavePath", out.data_save_path},
      {"currentDataPath", out.current_data_path},
      {"dbKey", out.db_key},
      {"publicKey", out.public_key},
      {"privateKey", out.private_key},
  };
  ret["data"] = j_info;
  return ret.dump();
}
std::string MiscController::GetSNSFirstPage(std::string params) {
  int64_t success = wechat::WeChatService::GetInstance().GetSNSFirstPage();
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
std::string MiscController::GetSNSNextPage(std::string params) {
  SPDLOG_INFO("GetSNSNextPage params:{}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  int64_t sns_id = jsonutils::GetInt64Param(jp, "snsId");
  int64_t success = wechat::WeChatService::GetInstance().GetSNSNextPage(sns_id);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
std::string MiscController::AddFavFromMsg(std::string params) {
  SPDLOG_INFO("AddFavFromMsg params:{}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  int64_t sns_id = jsonutils::GetInt64Param(jp, "msgId");
  int64_t success = wechat::WeChatService::GetInstance().AddFavFromMsg(sns_id);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
std::string MiscController::AddFavFromImage(std::string params) {
  SPDLOG_INFO("AddFavFromImage params:{}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring wxid = jsonutils::GetWStringParam(jp, "wxid");
  std::wstring image_path = jsonutils::GetWStringParam(jp, "imagePath");
  int64_t success =
      wechat::WeChatService::GetInstance().AddFavFromImage(wxid, image_path);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
std::string MiscController::DecodeImage(std::string params) {
  SPDLOG_INFO("DecodeImage params:{}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring file_path = jsonutils::GetWStringParam(jp, "filePath");
  std::wstring store_dir = jsonutils::GetWStringParam(jp, "storeDir");
  int64_t success =
      wechat::WeChatService::GetInstance().DecodeImage(file_path, store_dir);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
std::string MiscController::GetVoiceByMsgId(std::string params) {
  SPDLOG_INFO("GetVoiceByMsgId params:{}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  int64_t msg_id = jsonutils::GetInt64Param(jp, "msgId");
  std::wstring store_dir = jsonutils::GetWStringParam(jp, "storeDir");
  int64_t success =
      wechat::WeChatService::GetInstance().GetVoiceByDB(msg_id, store_dir);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
std::string MiscController::DoOcrTask(std::string params) {
  SPDLOG_INFO("DoOcrTask params:{}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring image_path = jsonutils::GetWStringParam(jp, "imagePath");
  std::string text("");
  int64_t success =
      wechat::WeChatService::GetInstance().DoOCRTask(image_path, text);
  nlohmann::json ret = {
      {"code", success}, {"data", {{"content", text}}}, {"msg", "success"}};
  return ret.dump();
}
std::string MiscController::LockWeChat(std::string params) {
  int64_t success = wechat::WeChatService::GetInstance().LockWeChat();
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
std::string MiscController::UnlockWeChat(std::string params) {
  int64_t success = wechat::WeChatService::GetInstance().UnlockWeChat();
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
std::string MiscController::ClickEnterWeChat(std::string params) {
  int64_t success = wechat::WeChatService::GetInstance().EnterWeChat();
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
std::string MiscController::GetLoginUrl(std::string params) {
  std::string login_url = wechat::WeChatService::GetInstance().GetLoginUrl();
  nlohmann::json ret = {
      {"code", 1}, {"data", {"loginUrl", login_url}}, {"msg", "success"}};
  return ret.dump();
}
std::string MiscController::TranslateVoice(std::string params) {
  SPDLOG_INFO("TranslateVoice params:{}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  int64_t msg_id = jsonutils::GetInt64Param(jp, "msgId");
  int64_t success = wechat::WeChatService::GetInstance().TranslateVoice(msg_id);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
std::string MiscController::GetTranslateVoiceText(std::string params) {
  nlohmann::json jp = nlohmann::json::parse(params);
  int64_t msg_id = jsonutils::GetInt64Param(jp, "msgId");
  std::string content =
      wechat::WeChatService::GetInstance().GetTranslateVoiceText(msg_id);
  nlohmann::json ret_data = {
      {"code", 1}, {"msg", "success"}, {"data", {{"transtext", content}}}};
  return ret_data.dump();
}
std::string MiscController::OpenUrlByWeChat(std::string params) {
  SPDLOG_INFO("OpenUrlByWeChat params:{}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring url = jsonutils::GetWStringParam(jp, "url");
  int flag = jsonutils::GetIntParam(jp, "flag");
  int64_t success =
      wechat::WeChatService::GetInstance().OpenUrlByWeChatBrowser(url, flag);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
std::string MiscController::ConfirmReceipt(std::string params) {
  SPDLOG_INFO("ConfirmReceipt params:{}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring wxid = jsonutils::GetWStringParam(jp, "wxid");
  std::wstring transcationid = jsonutils::GetWStringParam(jp, "transcationId");
  std::wstring transferid = jsonutils::GetWStringParam(jp, "transferId");
  int64_t success = wechat::WeChatService::GetInstance().DoConfirmReceipt(
      wxid, transcationid, transferid);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
std::string MiscController::DownloadAttach(std::string params) {
  SPDLOG_INFO("DownloadAttach params:{}", params);
  nlohmann::json jp = nlohmann::json::parse(params);
  int64_t msg_id = jsonutils::GetInt64Param(jp, "msgId");
  int64_t success = wechat::WeChatService::GetInstance().DoDownloadTask(msg_id);
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}
}  // namespace wxhelper
