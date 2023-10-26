#include "wechat_service.h"
#include "wxutils.h"
namespace offset = wxhelper::V3_9_7_29::offset;
namespace prototype = wxhelper::V3_9_7_29::prototype;
namespace func = wxhelper::V3_9_7_29::function;
namespace wxhelper {
WechatService::~WechatService() {}

INT64 WechatService::CheckLogin() { return INT64(); }

INT64 WechatService::GetSelfInfo(common::SelfInfoInner& out) { return INT64(); }

INT64 WechatService::SendTextMsg(const std::wstring& wxid,
                                 const std::wstring& msg) {
  INT64 success = -1;
  prototype::WeChatString to_user(wxid);
  prototype::WeChatString text_msg(msg);
  UINT64 send_message_mgr_addr = base_addr_ + offset::kGetSendMessageMgr;
  UINT64 send_text_msg_addr = base_addr_ + offset::kSendTextMsg;
  UINT64 free_chat_msg_addr = base_addr_ + offset::kFreeChatMsg;
  char chat_msg[0x460] = {0};
  UINT64 temp[3] = {0};
  func::__GetSendMessageMgr mgr;
  mgr = (func::__GetSendMessageMgr)send_message_mgr_addr;
  func::__SendTextMsg send;
  send = (func::__SendTextMsg)send_text_msg_addr;
  func::__FreeChatMsg free;
  free = (func::__FreeChatMsg)free_chat_msg_addr;
  mgr();
  send(reinterpret_cast<UINT64>(&chat_msg), reinterpret_cast<UINT64>(&to_user),
       reinterpret_cast<UINT64>(&text_msg), reinterpret_cast<UINT64>(&temp), 1,
       1, 0, 0);
  free(reinterpret_cast<UINT64>(&chat_msg));
  success = 1;

  return success;
}

INT64 WechatService::SendImageMsg(const std::wstring& wxid,
                                  const std::wstring& image_path) {
  return INT64();
}

INT64 WechatService::SendFileMsg(const std::wstring& wxid,
                                 const std::wstring& file_path) {
  return INT64();
}

INT64 WechatService::GetContacts(std::vector<common::ContactInner>& vec) {
  INT64 success = -1;
  UINT64 get_contact_mgr_addr = base_addr_ + offset::kGetContactMgr;
  UINT64 get_contact_list_addr = base_addr_ + offset::kGetContactList;
  func::__GetContactMgr get_contact_mgr =
      (func::__GetContactMgr)get_contact_mgr_addr;
  func::__GetContactList get_contact_list =
      (func::__GetContactList)get_contact_list_addr;
  UINT64 mgr = get_contact_mgr();
  UINT64 contact_vec[3] = {0, 0, 0};
  success = get_contact_list(mgr, reinterpret_cast<UINT64>(&contact_vec));

  UINT64 start = contact_vec[0];
  UINT64 end = contact_vec[2];
  while (start < end) {
    common::ContactInner temp;
    temp.wxid = wxutils::ReadWstringThenConvert(start + 0x10);
    temp.custom_account = wxutils::ReadWstringThenConvert(start + 0x30);
    temp.encrypt_name = wxutils::ReadWstringThenConvert(start + 0x50);
    temp.nickname = wxutils::ReadWstringThenConvert(start + 0xA0);
    temp.pinyin = wxutils::ReadWstringThenConvert(start + 0x108);
    temp.pinyin_all = wxutils::ReadWstringThenConvert(start + 0x128);
    temp.verify_flag = *(DWORD *)(start + 0x70);
    temp.type = *(DWORD *)(start + 0x74);
    temp.reserved1 = *(DWORD *)(start + 0x1F0);
    temp.reserved2 = *(DWORD *)(start + 0x1F4);
    vec.push_back(temp);
    start += 0x6A8;
  }
  return success;
}

INT64 WechatService::GetChatRoomDetailInfo(
    const std::wstring& room_id, common::ChatRoomInfoInner& room_info) {
  return INT64();
}

INT64 WechatService::AddMemberToChatRoom(
    const std::wstring& room_id, const std::vector<std::wstring>& members) {
  return INT64();
}

INT64 WechatService::ModChatRoomMemberNickName(const std::wstring& room_id,
                                               const std::wstring& wxid,
                                               const std::wstring& nickname) {
  return INT64();
}

INT64 WechatService::DelMemberFromChatRoom(
    const std::wstring& room_id, const std::vector<std::wstring>& members) {
  return INT64();
}

INT64 WechatService::GetMemberFromChatRoom(
    const std::wstring& room_id, common::ChatRoomMemberInner& member) {
  return INT64();
}

INT64 WechatService::SetTopMsg(ULONG64 msg_id) { return INT64(); }

INT64 WechatService::RemoveTopMsg(const std::wstring& room_id, ULONG64 msg_id) {
  return INT64();
}

INT64 WechatService::InviteMemberToChatRoom(
    const std::wstring& room_id, const std::vector<std::wstring>& wxids) {
  return INT64();
}

INT64 WechatService::CreateChatRoom(const std::vector<std::wstring>& wxids) {
  return INT64();
}

INT64 WechatService::QuitChatRoom(const std::wstring& room_id) {
  return INT64();
}

INT64 WechatService::ForwardMsg(UINT64 msg_id, const std::wstring& wxid) {
  return INT64();
}

INT64 WechatService::GetSNSFirstPage() { return INT64(); }

INT64 WechatService::GetSNSNextPage(UINT64 sns_id) { return INT64(); }

INT64 WechatService::AddFavFromMsg(UINT64 msg_id) { return INT64(); }

INT64 WechatService::AddFavFromImage(const std::wstring& wxid,
                                     const std::wstring& image_path) {
  return INT64();
}

INT64 WechatService::SendAtText(const std::wstring& room_id,
                                const std::vector<std::wstring>& wxids,
                                const std::wstring& msg) {
  return INT64();
}

std::wstring WechatService::GetContactOrChatRoomNickname(
    const std::wstring& wxid) {
  return std::wstring();
}

INT64 WechatService::GetContactByWxid(const std::wstring& wxid,
                                      common::ContactProfileInner& profile) {
  return INT64();
}

INT64 WechatService::DoDownloadTask(UINT64 msg_id) { return INT64(); }

INT64 WechatService::ForwardPublicMsg(const std::wstring& wxid,
                                      const std::wstring& title,
                                      const std::wstring& url,
                                      const std::wstring& thumb_url,
                                      const std::wstring& sender_id,
                                      const std::wstring& sender_name,
                                      const std::wstring& digest) {
  return INT64();
}

INT64 WechatService::ForwardPublicMsgByMsgId(const std::wstring& wxid,
                                             UINT64 msg_id) {
  return INT64();
}

INT64 WechatService::DecodeImage(const std::wstring& file_path,
                                 const std::wstring& save_dir) {
  return INT64();
}

INT64 WechatService::GetVoiceByDB(ULONG64 msg_id, const std::wstring& dir) {
  return INT64();
}

INT64 WechatService::SendCustomEmotion(const std::wstring& file_path,
                                       const std::wstring& wxid) {
  return INT64();
}

INT64 WechatService::SendApplet(
    const std::wstring& recv_wxid, const std::wstring& waid_suff,
    const std::wstring& waid_w, const std::string& waid_s,
    const std::string& wa_wxid, const std::string& json_param,
    const std::string& head_image, const std::string& big_image,
    const std::string& index_page) {
  return INT64();
}

INT64 WechatService::SendPatMsg(const std::wstring& room_id,
                                const std::wstring& wxid) {
  return INT64();
}

INT64 WechatService::DoOCRTask(const std::wstring& img_path,
                               std::string& result) {
  return INT64();
}

void WechatService::SetBaseAddr(UINT64 addr) { base_addr_ = addr; }

void WechatService::SetJsApiAddr(UINT64 addr) { js_api_addr_ = addr; }


}  // namespace wxhelper