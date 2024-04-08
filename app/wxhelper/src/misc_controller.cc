#include "misc_controller.h"

#include "nlohmann/json.hpp"
namespace wxhelper {
std::string MiscController::CheckLogin(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::GetUserInfo(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::GetSNSFirstPage(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::GetSNSNextPage(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::AddFavFromMsg(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::AddFavFromImage(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::DecodeImage(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::GetVoiceByMsgId(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::DoOcrTask(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::LockWeChat(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::UnlockWeChat(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::ClickEnterWeChat(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::GetLoginUrl(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::TranslateVoice(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::GetTranslateVoiceText(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::OpenUrlByWeChat(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::ConfirmReceipt(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string MiscController::DownloadAttach(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
}  // namespace wxhelper
