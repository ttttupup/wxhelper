#include "chat_room_controller.h"

#include "nlohmann/json.hpp"
namespace wxhelper {
std::string ChatController::GetChatRoomDetailInfo(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string ChatController::GetMemberFromChatRoom(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string ChatController::AddMemberToChatRoom(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string ChatController::DelMemberFromChatRoom(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string ChatController::CreateChatRoom(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string ChatController::QuitChatRoom(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string ChatController::InviteMemberToChatRoom(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string ChatController::TopMsg(std::string params) { return std::string(); }
std::string ChatController::RemoveTopMsg(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string ChatController::GetChatRoomMemberNickname(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
std::string ChatController::ModifyChatRoomMemberNickName(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
}  // namespace wxhelper
