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
prototype::WeChatString* BuildWechatString(const std::wstring& ws) {
  prototype::WeChatString* p =
      base::utils::WxHeapAlloc<prototype::WeChatString>(
          sizeof(prototype::WeChatString));
  wchar_t* p_chat_room_id =
      base::utils::WxHeapAlloc<wchar_t>((ws.size() + 1) * 2);
  wmemcpy(p_chat_room_id, ws.c_str(), ws.size() + 1);
  p->ptr = p_chat_room_id;
  p->length = static_cast<int32_t>(ws.size());
  p->max_length = static_cast<int32_t>(ws.size());
  p->c_len = 0;
  p->c_ptr = 0;
  return p;
}
std::string ChatController::SendTextMsg(std::string params) {
  int64_t base_addr = wxutils::GetWeChatWinBase();
  nlohmann::json jp = nlohmann::json::parse(params);
  SPDLOG_INFO("sendTextMsg wxid={},msg={}", jp["wxid"], jp["msg"]);
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
  uint64_t success = send(reinterpret_cast<uint64_t>(&chat_msg),
                          reinterpret_cast<uint64_t>(&to_user),
                          reinterpret_cast<uint64_t>(&text_msg),
                          reinterpret_cast<uint64_t>(&temp), 1, 1, 0, 0);
  free(reinterpret_cast<uint64_t>(&chat_msg));

  nlohmann::json ret_data = {
      {"code", success}, {"data", {}}, {"msg", "success"}};
  return ret_data.dump();
}

std::string ChatController::SendImageMsg(std::string params) {
  int64_t success = -1;
  int64_t base_addr = wxutils::GetWeChatWinBase();
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring wxid = jsonutils::GetWStringParam(jp, "wxid");
  std::wstring image_path = jsonutils::GetWStringParam(jp, "imagePath");
  prototype::WeChatString to_user(wxid);
  prototype::WeChatString image_full_path(image_path);
  uint64_t send_message_mgr_addr = base_addr + offset::kGetSendMessageMgr;
  uint64_t send_img_addr = base_addr + offset::kSendImageMsg;
  uint64_t new_chat_msg_addr = base_addr + offset::kChatMsgInstanceCounter;
  uint64_t free_chat_msg_addr = base_addr + offset::kFreeChatMsg;
  func::__NewChatMsg new_chat_msg = (func::__NewChatMsg)new_chat_msg_addr;
  func::__GetSendMessageMgr mgr =
      (func::__GetSendMessageMgr)send_message_mgr_addr;
  func::__SendImageMsg send_img = (func::__SendImageMsg)send_img_addr;
  func::__FreeChatMsg free = (func::__FreeChatMsg)free_chat_msg_addr;

  char chat_msg[0x460] = {0};
  char chat_msg_temp[0x460] = {0};

  uint64_t p_chat_msg_temp =
      new_chat_msg(reinterpret_cast<uint64_t>(&chat_msg_temp));
  uint64_t temp1 = 0;
  uint64_t temp2 = 0;
  uint64_t* flag[10] = {};
  flag[8] = &temp1;
  flag[9] = &temp2;
  flag[1] = reinterpret_cast<uint64_t*>(p_chat_msg_temp);

  uint64_t p_chat_msg = new_chat_msg(reinterpret_cast<uint64_t>(&chat_msg));
  uint64_t send_mgr = mgr();
  send_img(send_mgr, p_chat_msg, reinterpret_cast<uint64_t>(&to_user),
           reinterpret_cast<uint64_t>(&image_full_path),
           reinterpret_cast<uint64_t>(&flag));
  free(p_chat_msg);
  free(p_chat_msg_temp);
  success = 1;
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}

std::string ChatController::SendFileMsg(std::string params) {
  int64_t success = -1;
  int64_t base_addr = wxutils::GetWeChatWinBase();
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring wxid = jsonutils::GetWStringParam(jp, "wxid");
  std::wstring file_path = jsonutils::GetWStringParam(jp, "filePath");
  prototype::WeChatString* to_user = (prototype::WeChatString*)HeapAlloc(
      GetProcessHeap(), 0, sizeof(prototype::WeChatString));
  wchar_t* ptr_wxid =
      (wchar_t*)HeapAlloc(GetProcessHeap(), 0, (wxid.length() + 1) * 2);
  wmemcpy(ptr_wxid, wxid.c_str(), wxid.length() + 1);
  to_user->ptr = ptr_wxid;
  to_user->length = static_cast<DWORD>(wxid.length());
  to_user->max_length = static_cast<DWORD>(wxid.length());
  to_user->c_len = 0;
  to_user->c_ptr = 0;
  prototype::WeChatString* file_full_path = (prototype::WeChatString*)HeapAlloc(
      GetProcessHeap(), 0, sizeof(prototype::WeChatString));
  wchar_t* ptr_path =
      (wchar_t*)HeapAlloc(GetProcessHeap(), 0, (file_path.length() + 1) * 2);
  wmemcpy(ptr_path, file_path.c_str(), file_path.length() + 1);
  file_full_path->ptr = ptr_path;
  file_full_path->length = static_cast<DWORD>(file_path.length());
  file_full_path->max_length = static_cast<DWORD>(file_path.length());
  file_full_path->c_len = 0;
  file_full_path->c_ptr = 0;

  uint64_t get_app_msg_mgr_addr = base_addr + offset::kGetAppMsgMgr;
  uint64_t send_file_addr = base_addr + offset::kSendFileMsg;
  uint64_t new_chat_msg_addr = base_addr + offset::kChatMsgInstanceCounter;
  uint64_t free_chat_msg_addr = base_addr + offset::kFreeChatMsg;
  func::__NewChatMsg new_chat_msg = (func::__NewChatMsg)new_chat_msg_addr;
  func::__GetAppMsgMgr get_app_mgr = (func::__GetAppMsgMgr)get_app_msg_mgr_addr;
  func::__SendFile send_file = (func::__SendFile)send_file_addr;
  func::__FreeChatMsg free = (func::__FreeChatMsg)free_chat_msg_addr;

  char* chat_msg = (char*)HeapAlloc(GetProcessHeap(), 0, 0x460);

  uint64_t* temp1 =
      (uint64_t*)HeapAlloc(GetProcessHeap(), 0, sizeof(uint64_t) * 4);
  uint64_t* temp2 =
      (uint64_t*)HeapAlloc(GetProcessHeap(), 0, sizeof(uint64_t) * 4);
  uint64_t* temp3 =
      (uint64_t*)HeapAlloc(GetProcessHeap(), 0, sizeof(uint64_t) * 4);
  uint64_t* temp4 =
      (uint64_t*)HeapAlloc(GetProcessHeap(), 0, sizeof(uint64_t) * 4);
  ZeroMemory(temp1, sizeof(uint64_t) * 4);
  ZeroMemory(temp2, sizeof(uint64_t) * 4);
  ZeroMemory(temp3, sizeof(uint64_t) * 4);
  ZeroMemory(temp4, sizeof(uint64_t) * 4);
  *temp4 = 0x1F;
  uint64_t app_mgr = get_app_mgr();
  send_file(app_mgr, reinterpret_cast<uint64_t>(chat_msg),
            reinterpret_cast<uint64_t>(to_user),
            reinterpret_cast<uint64_t>(file_full_path), 1,
            reinterpret_cast<uint64_t>(temp1), 0,
            reinterpret_cast<uint64_t>(temp2), 0,
            reinterpret_cast<uint64_t>(temp3), 0, 0);
  free(reinterpret_cast<uint64_t>(chat_msg));
  HeapFree(GetProcessHeap(), 0, to_user);
  HeapFree(GetProcessHeap(), 0, file_full_path);
  HeapFree(GetProcessHeap(), 0, temp1);
  HeapFree(GetProcessHeap(), 0, temp2);
  HeapFree(GetProcessHeap(), 0, temp3);
  HeapFree(GetProcessHeap(), 0, temp4);
  success = 1;
  nlohmann::json ret = {{"code", success}, {"data", {}}, {"msg", "success"}};
  return ret.dump();
}

std::string ChatController::SendAtText(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}

std::string ChatController::SendMultiAtText(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}

std::string ChatController::SendCustomEmotion(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}

std::string ChatController::SendApplet(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}

std::string ChatController::SendPatMsg(std::string params) {
  int64_t success = -1;
  int64_t base_addr = wxutils::GetWeChatWinBase();
  nlohmann::json jp = nlohmann::json::parse(params);
  std::wstring room_id = jsonutils::GetWStringParam(jp, "receiver");
  std::wstring wxid = jsonutils::GetWStringParam(jp, "wxid");
  UINT64 send_pat_msg_addr = base_addr + offset::kSendPatMsg;
  func::__SendPatMsg send_pat_msg = (func::__SendPatMsg)send_pat_msg_addr;
  prototype::WeChatString chat_room(room_id);
  prototype::WeChatString target(wxid);
  success = send_pat_msg(reinterpret_cast<UINT64>(&chat_room),
                         reinterpret_cast<UINT64>(&target));
  nlohmann::json ret_data = {
      {"code", success}, {"msg", "success"}, {"data", {}}};
  return ret_data.dump();
}

std::string ChatController::ForwardMsg(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}

std::string ChatController::ForwardPublicMsgByMsgId(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}

std::string ChatController::ForwardPublicMsg(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}

std::string ChatController::GetContactOrChatRoomNickname(std::string params) {
  nlohmann::json ret = {
      {"code", 200}, {"data", {}}, {"msg", "Not Implemented"}};
  return ret.dump();
}
}  // namespace wxhelper