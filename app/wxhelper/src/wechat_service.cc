#include "wechat_service.h"

#include "json_utils.h"
#include "memory.h"
#include "spdlog/spdlog.h"
#include "tinyxml2.h"
#include "utils.h"
#include "wechat_db.h"
#include "wxutils.h"

#define BUFSIZE 1024
#define JPEG0 0xFF
#define JPEG1 0xD8
#define JPEG2 0xFF
#define PNG0 0x89
#define PNG1 0x50
#define PNG2 0x4E
#define BMP0 0x42
#define BMP1 0x4D
#define GIF0 0x47
#define GIF1 0x49
#define GIF2 0x46

namespace offset = wechat::offset;
namespace prototype = wechat::prototype;
namespace func = wechat::function;
namespace utils = base::utils;
namespace jsonutils = wxhelper::jsonutils;
namespace wxutils = wxhelper::wxutils;
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
wechat::WeChatService::~WeChatService() {}

void wechat::WeChatService::Init() { base_addr_ = wxutils::GetWeChatWinBase(); }

int64_t wechat::WeChatService::CheckLogin() {
  int64_t success = -1;

  uint64_t accout_service_addr = base_addr_ + offset::kGetAccountServiceMgr;
  func::__GetAccountService GetSevice =
      (func::__GetAccountService)accout_service_addr;
  uint64_t service_addr = GetSevice();
  if (service_addr) {
    success = *(uint64_t*)(service_addr + 0x7F8);
  }
  return success;
}

int64_t wechat::WeChatService::GetSelfInfo(SelfInfoInner& out) {
  int64_t success = -1;
  uint64_t accout_service_addr = base_addr_ + offset::kGetAccountServiceMgr;
  uint64_t get_app_data_save_path_addr =
      base_addr_ + offset::kGetAppDataSavePath;
  uint64_t get_current_data_path_addr =
      base_addr_ + offset::kGetCurrentDataPath;
  func::__GetAccountService GetSevice =
      (func::__GetAccountService)accout_service_addr;
  func::__GetDataSavePath GetDataSavePath =
      (func::__GetDataSavePath)get_app_data_save_path_addr;
  func::__GetCurrentDataPath GetCurrentDataPath =
      (func::__GetCurrentDataPath)get_current_data_path_addr;
  uint64_t service_addr = GetSevice();
  if (service_addr) {
    if (*(int64_t*)(service_addr + 0x80) == 0 ||
        *(int64_t*)(service_addr + 0x80 + 0x10) == 0) {
      out.wxid = std::string();
    } else {
      if (*(int64_t*)(service_addr + 0x80 + 0x18) == 0xF) {
        out.wxid = std::string((char*)(service_addr + 0x80),
                               *(int64_t*)(service_addr + 0x80 + 0x10));
      } else {
        out.wxid = std::string(*(char**)(service_addr + 0x80),
                               *(int64_t*)(service_addr + 0x80 + 0x10));
      }
    }

    if (*(int64_t*)(service_addr + 0x108) == 0 ||
        *(int64_t*)(service_addr + 0x108 + 0x10) == 0) {
      out.account = std::string();
    } else {
      if (*(int64_t*)(service_addr + 0x108 + 0x18) == 0xF) {
        out.account = std::string((char*)(service_addr + 0x108),
                                  *(int64_t*)(service_addr + 0x108 + 0x10));
      } else {
        out.account = std::string(*(char**)(service_addr + 0x108),
                                  *(int64_t*)(service_addr + 0x108 + 0x10));
      }
    }

    if (*(int64_t*)(service_addr + 0x128) == 0 ||
        *(int64_t*)(service_addr + 0x128 + 0x10) == 0) {
      out.mobile = std::string();
    } else {
      if (*(int64_t*)(service_addr + 0x128 + 0x18) == 0xF) {
        out.mobile = std::string((char*)(service_addr + 0x128),
                                 *(int64_t*)(service_addr + 0x128 + 0x10));
      } else {
        out.mobile = std::string(*(char**)(service_addr + 0x128),
                                 *(int64_t*)(service_addr + 0x128 + 0x10));
      }
    }

    if (*(int64_t*)(service_addr + 0x148) == 0 ||
        *(int64_t*)(service_addr + 0x148 + 0x10) == 0) {
      out.signature = std::string();
    } else {
      if (*(int64_t*)(service_addr + 0x148 + 0x18) == 0xF) {
        out.signature = std::string((char*)(service_addr + 0x148),
                                    *(int64_t*)(service_addr + 0x148 + 0x10));
      } else {
        out.signature = std::string(*(char**)(service_addr + 0x148),
                                    *(int64_t*)(service_addr + 0x148 + 0x10));
      }
    }

    if (*(int64_t*)(service_addr + 0x168) == 0 ||
        *(int64_t*)(service_addr + 0x168 + 0x10) == 0) {
      out.country = std::string();
    } else {
      if (*(int64_t*)(service_addr + 0x168 + 0x18) == 0xF) {
        out.country = std::string((char*)(service_addr + 0x168),
                                  *(int64_t*)(service_addr + 0x168 + 0x10));
      } else {
        out.country = std::string(*(char**)(service_addr + 0x168),
                                  *(int64_t*)(service_addr + 0x168 + 0x10));
      }
    }

    if (*(int64_t*)(service_addr + 0x188) == 0 ||
        *(int64_t*)(service_addr + 0x188 + 0x10) == 0) {
      out.province = std::string();
    } else {
      if (*(int64_t*)(service_addr + 0x188 + 0x18) == 0xF) {
        out.province = std::string((char*)(service_addr + 0x188),
                                   *(int64_t*)(service_addr + 0x188 + 0x10));
      } else {
        out.province = std::string(*(char**)(service_addr + 0x188),
                                   *(int64_t*)(service_addr + 0x188 + 0x10));
      }
    }

    if (*(int64_t*)(service_addr + 0x1A8) == 0 ||
        *(int64_t*)(service_addr + 0x1A8 + 0x10) == 0) {
      out.city = std::string();
    } else {
      if (*(int64_t*)(service_addr + 0x1A8 + 0x18) == 0xF) {
        out.city = std::string((char*)(service_addr + 0x1A8),
                               *(int64_t*)(service_addr + 0x1A8 + 0x10));
      } else {
        out.city = std::string(*(char**)(service_addr + 0x1A8),
                               *(int64_t*)(service_addr + 0x1A8 + 0x10));
      }
    }

    if (*(int64_t*)(service_addr + 0x1E8) == 0 ||
        *(int64_t*)(service_addr + 0x1E8 + 0x10) == 0) {
      out.name = std::string();
    } else {
      if (*(int64_t*)(service_addr + 0x1E8 + 0x18) == 0xF) {
        out.name = std::string((char*)(service_addr + 0x1E8),
                               *(int64_t*)(service_addr + 0x1E8 + 0x10));
      } else {
        out.name = std::string(*(char**)(service_addr + 0x1E8),
                               *(int64_t*)(service_addr + 0x1E8 + 0x10));
      }
    }

    if (*(int64_t*)(service_addr + 0x450) == 0 ||
        *(int64_t*)(service_addr + 0x450 + 0x10) == 0) {
      out.head_img = std::string();
    } else {
      out.head_img = std::string(*(char**)(service_addr + 0x450),
                                 *(int64_t*)(service_addr + 0x450 + 0x10));
    }

    if (*(int64_t*)(service_addr + 0x7B8) == 0 ||
        *(int64_t*)(service_addr + 0x7B8 + 0x10) == 0) {
      out.public_key = std::string();
    } else {
      out.public_key = std::string(*(char**)(service_addr + 0x7B8),
                                   *(int64_t*)(service_addr + 0x7B8 + 0x10));
    }

    if (*(int64_t*)(service_addr + 0x7D8) == 0 ||
        *(int64_t*)(service_addr + 0x7D8 + 0x10) == 0) {
      out.private_key = std::string();
    } else {
      out.private_key = std::string(*(char**)(service_addr + 0x7D8),
                                    *(int64_t*)(service_addr + 0x7D8 + 0x10));
    }

    if (*(int64_t*)(service_addr + 0x6E0) == 0 ||
        *(int64_t*)(service_addr + 0x6E8) == 0) {
      out.db_key = std::string();
    } else {
      int64_t byte_addr = *(int64_t*)(service_addr + 0x6E0);
      int64_t len = *(int64_t*)(service_addr + 0x6E8);
      out.db_key =
          base::utils::Bytes2Hex((BYTE*)byte_addr, static_cast<int>(len));
    }

    uint64_t flag = *(uint64_t*)(service_addr + 0x7F8);
    if (flag == 1) {
      prototype::WeChatString current_data_path;
      GetCurrentDataPath(reinterpret_cast<ULONG_PTR>(&current_data_path));
      if (current_data_path.ptr) {
        out.current_data_path = base::utils::WstringToUtf8(
            std::wstring(current_data_path.ptr, current_data_path.length));
      } else {
        out.current_data_path = std::string();
      }
    }
  }
  prototype::WeChatString data_save_path;
  GetCurrentDataPath(reinterpret_cast<ULONG_PTR>(&data_save_path));
  if (data_save_path.ptr) {
    out.data_save_path = base::utils::WstringToUtf8(
        std::wstring(data_save_path.ptr, data_save_path.length));
  } else {
    out.data_save_path = std::string();
  }
  success = 1;
  return success;
}

int64_t wechat::WeChatService::SendTextMsg(const std::wstring& wxid,
                                           const std::wstring& msg) {
  prototype::WeChatString to_user(wxid);
  prototype::WeChatString text_msg(msg);
  uint64_t send_message_mgr_addr = base_addr_ + offset::kGetSendMessageMgr;
  uint64_t send_text_msg_addr = base_addr_ + offset::kSendTextMsg;
  uint64_t free_chat_msg_addr = base_addr_ + offset::kFreeChatMsg;
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
  return 0;
}

int64_t wechat::WeChatService::SendImageMsg(const std::wstring& wxid,
                                            const std::wstring& image_path) {
  int64_t success = -1;
  prototype::WeChatString to_user(wxid);
  prototype::WeChatString image_full_path(image_path);
  uint64_t send_message_mgr_addr = base_addr_ + offset::kGetSendMessageMgr;
  uint64_t send_img_addr = base_addr_ + offset::kSendImageMsg;
  uint64_t new_chat_msg_addr = base_addr_ + offset::kChatMsgInstanceCounter;
  uint64_t free_chat_msg_addr = base_addr_ + offset::kFreeChatMsg;
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
  uint64_t temp3 = 1;
  uint64_t* flag[10] = {};
  flag[0] = reinterpret_cast<uint64_t*>(temp3);
  flag[8] = &temp1;
  flag[9] = &temp2;
  flag[1] = reinterpret_cast<uint64_t*>(p_chat_msg_temp);

  uint64_t p_chat_msg = new_chat_msg(reinterpret_cast<uint64_t>(&chat_msg));
  uint64_t send_mgr = mgr();
  success = send_img(send_mgr, p_chat_msg, reinterpret_cast<uint64_t>(&to_user),
                     reinterpret_cast<uint64_t>(&image_full_path),
                     reinterpret_cast<uint64_t>(&flag));
  free(p_chat_msg);
  free(p_chat_msg_temp);
  return success;
}

int64_t wechat::WeChatService::SendFileMsg(const std::wstring& wxid,
                                           const std::wstring& file_path) {
  int64_t success = -1;
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

  uint64_t get_app_msg_mgr_addr = base_addr_ + offset::kGetAppMsgMgr;
  uint64_t send_file_addr = base_addr_ + offset::kSendFileMsg;
  uint64_t new_chat_msg_addr = base_addr_ + offset::kChatMsgInstanceCounter;
  uint64_t free_chat_msg_addr = base_addr_ + offset::kFreeChatMsg;
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

  ZeroMemory(temp1, sizeof(uint64_t) * 4);
  ZeroMemory(temp2, sizeof(uint64_t) * 4);
  ZeroMemory(temp3, sizeof(uint64_t) * 4);

  uint64_t app_mgr = get_app_mgr();
  success = send_file(app_mgr, reinterpret_cast<uint64_t>(chat_msg),
                      reinterpret_cast<uint64_t>(to_user),
                      reinterpret_cast<uint64_t>(file_full_path), 1,
                      reinterpret_cast<uint64_t>(temp1), 0,
                      reinterpret_cast<uint64_t>(temp2), 0,
                      reinterpret_cast<uint64_t>(temp3), 0, 0xC);
  free(reinterpret_cast<uint64_t>(chat_msg));
  HeapFree(GetProcessHeap(), 0, to_user);
  HeapFree(GetProcessHeap(), 0, file_full_path);
  HeapFree(GetProcessHeap(), 0, temp1);
  HeapFree(GetProcessHeap(), 0, temp2);
  HeapFree(GetProcessHeap(), 0, temp3);
  return success;
}

int64_t wechat::WeChatService::GetContacts(std::vector<ContactInner>& vec) {
  int64_t success = -1;
  int64_t base_addr = wxutils::GetWeChatWinBase();
  uint64_t get_contact_mgr_addr = base_addr + offset::kGetContactMgr;
  uint64_t get_contact_list_addr = base_addr + offset::kGetContactList;
  func::__GetContactMgr get_contact_mgr =
      (func::__GetContactMgr)get_contact_mgr_addr;
  func::__GetContactList get_contact_list =
      (func::__GetContactList)get_contact_list_addr;
  uint64_t mgr = get_contact_mgr();
  uint64_t contact_vec[3] = {0, 0, 0};
  success = get_contact_list(mgr, reinterpret_cast<uint64_t>(&contact_vec));

  uint64_t start = contact_vec[0];
  uint64_t end = contact_vec[2];
  while (start < end) {
    wechat::ContactInner temp;
    temp.wxid = wxutils::ReadWstringThenConvert(start + 0x10);
    temp.custom_account = wxutils::ReadWstringThenConvert(start + 0x30);
    temp.encrypt_name = wxutils::ReadWstringThenConvert(start + 0x50);
    temp.remark = wxutils::ReadWstringThenConvert(start + 0x80);
    temp.remark_pinyin = wxutils::ReadWstringThenConvert(start + 0x148);
    temp.remark_pinyin_all = wxutils::ReadWstringThenConvert(start + 0x168);
    temp.label_ids = wxutils::ReadWstringThenConvert(start + 0xc0);
    temp.nickname = wxutils::ReadWstringThenConvert(start + 0xA0);
    temp.pinyin = wxutils::ReadWstringThenConvert(start + 0x108);
    temp.pinyin_all = wxutils::ReadWstringThenConvert(start + 0x128);
    temp.verify_flag = *(int32_t*)(start + 0x70);
    temp.type = *(int32_t*)(start + 0x74);
    temp.reserved1 = *(int32_t*)(start + 0x1F0);
    temp.reserved2 = *(int32_t*)(start + 0x1F4);
    vec.push_back(temp);
    start += 0x6A8;
  }
  return success;
}

int64_t wechat::WeChatService::GetChatRoomDetailInfo(
    const std::wstring& room_id, ChatRoomInfoInner& room_info) {
  int64_t success = -1;
  prototype::WeChatString chat_room_id(room_id);
  int64_t base_addr = wxutils::GetWeChatWinBase();
  uint64_t get_chat_room_mgr_addr = base_addr + offset::kChatRoomMgr;
  uint64_t get_chat_room_detail_addr =
      base_addr + offset::kGetChatRoomDetailInfo;
  uint64_t create_chat_room_info_addr = base_addr + offset::kNewChatRoomInfo;
  uint64_t free_chat_room_info_addr = base_addr + offset::kFreeChatRoomInfo;

  func::__GetChatRoomMgr get_chat_room_mgr =
      (func::__GetChatRoomMgr)get_chat_room_mgr_addr;
  func::__NewChatRoomInfo new_chat_room_info =
      (func::__NewChatRoomInfo)create_chat_room_info_addr;
  func::__FreeChatRoomInfo free_chat_room_info =
      (func::__FreeChatRoomInfo)free_chat_room_info_addr;
  func::__GetChatRoomDetailInfo get_chat_room_detail =
      (func::__GetChatRoomDetailInfo)get_chat_room_detail_addr;

  char chat_room_info[0x144] = {0};

  uint64_t new_room_info =
      new_chat_room_info(reinterpret_cast<uint64_t>(&chat_room_info));

  uint64_t mgr = get_chat_room_mgr();
  success = get_chat_room_detail(mgr, reinterpret_cast<uint64_t>(&chat_room_id),
                                 new_room_info, 1);

  room_info.chat_room_id = wxutils::ReadWstringThenConvert(new_room_info + 0x8);
  room_info.notice = wxutils::ReadWstringThenConvert(new_room_info + 0x28);
  room_info.admin = wxutils::ReadWstringThenConvert(new_room_info + 0x48);
  room_info.xml = wxutils::ReadWstringThenConvert(new_room_info + 0x78);
  free_chat_room_info(new_room_info);

  return success;
}

int64_t wechat::WeChatService::AddMemberToChatRoom(
    const std::wstring& room_id, const std::vector<std::wstring>& members) {
  int64_t success = -1;
  uint64_t get_chat_room_mgr_addr = base_addr_ + offset::kChatRoomMgr;
  uint64_t add_members_addr = base_addr_ + offset::kDoAddMemberToChatRoom;
  func::__GetChatRoomMgr get_chat_room_mgr =
      (func::__GetChatRoomMgr)get_chat_room_mgr_addr;
  func::__DoAddMemberToChatRoom add_members =
      (func::__DoAddMemberToChatRoom)add_members_addr;

  prototype::WeChatString* chat_room_id = (prototype::WeChatString*)HeapAlloc(
      GetProcessHeap(), 0, sizeof(prototype::WeChatString));
  wchar_t* p_chat_room_id =
      (wchar_t*)HeapAlloc(GetProcessHeap(), 0, (room_id.size() + 1) * 2);
  wmemcpy(p_chat_room_id, room_id.c_str(), room_id.size() + 1);
  chat_room_id->ptr = p_chat_room_id;
  chat_room_id->length = static_cast<int32_t>(room_id.size());
  chat_room_id->max_length = static_cast<int32_t>(room_id.size());
  chat_room_id->c_len = 0;
  chat_room_id->c_ptr = 0;

  std::vector<prototype::WeChatString> member_list;
  uint64_t temp[2] = {0};
  wechat::VectorInner* list = (wechat::VectorInner*)&member_list;
  int64_t members_ptr = (int64_t)&list->start;
  for (int i = 0; i < members.size(); i++) {
    prototype::WeChatString member(members[i]);
    member_list.push_back(member);
  }
  uint64_t mgr = get_chat_room_mgr();
  success =
      add_members(mgr, members_ptr, reinterpret_cast<uint64_t>(chat_room_id),
                  reinterpret_cast<uint64_t>(&temp));
  return success;
}

int64_t wechat::WeChatService::ModChatRoomMemberNickName(
    const std::wstring& room_id, const std::wstring& wxid,
    const std::wstring& nickname) {
  int64_t success = -1;
  uint64_t mod_addr = base_addr_ + offset::kDoModChatRoomMemberNickName;
  func::__DoModChatRoomMemberNickName modify =
      (func::__DoModChatRoomMemberNickName)mod_addr;
  const wchar_t* p = room_id.c_str();
  prototype::WeChatString* chat_room_id = BuildWechatString(room_id);
  prototype::WeChatString* self_id = BuildWechatString(wxid);
  prototype::WeChatString* name = BuildWechatString(nickname);
  success = modify(
      reinterpret_cast<UINT64>(p), reinterpret_cast<UINT64>(chat_room_id),
      reinterpret_cast<UINT64>(self_id), reinterpret_cast<UINT64>(name));
  return success;
}

int64_t wechat::WeChatService::DelMemberFromChatRoom(
    const std::wstring& room_id, const std::vector<std::wstring>& members) {
  int64_t success = -1;
  uint64_t get_chat_room_mgr_addr = base_addr_ + offset::kChatRoomMgr;
  uint64_t del_members_addr = base_addr_ + offset::kDelMemberFromChatRoom;
  func::__GetChatRoomMgr get_chat_room_mgr =
      (func::__GetChatRoomMgr)get_chat_room_mgr_addr;
  func::__DoDelMemberFromChatRoom del_members =
      (func::__DoDelMemberFromChatRoom)del_members_addr;

  prototype::WeChatString* chat_room_id = BuildWechatString(room_id);
  std::vector<prototype::WeChatString> member_list;
  uint64_t temp[2] = {0};
  wechat::VectorInner* list = (wechat::VectorInner*)&member_list;
  int64_t members_ptr = (int64_t)&list->start;
  for (int i = 0; i < members.size(); i++) {
    prototype::WeChatString member(members[i]);
    member_list.push_back(member);
  }
  uint64_t mgr = get_chat_room_mgr();
  success =
      del_members(mgr, members_ptr, reinterpret_cast<uint64_t>(chat_room_id));
  return success;
}

int64_t wechat::WeChatService::GetMemberFromChatRoom(
    const std::wstring& room_id, ChatRoomMemberInner& member) {
  int64_t success = -1;
  uint64_t get_chat_room_mgr_addr = base_addr_ + offset::kChatRoomMgr;
  uint64_t get_members_addr = base_addr_ + offset::kGetMemberFromChatRoom;
  uint64_t new_chat_room_addr = base_addr_ + offset::kNewChatRoom;
  uint64_t free_chat_room_addr = base_addr_ + offset::kFreeChatRoom;
  func::__GetChatRoomMgr get_chat_room_mgr =
      (func::__GetChatRoomMgr)get_chat_room_mgr_addr;
  func::__GetMemberFromChatRoom get_members =
      (func::__GetMemberFromChatRoom)get_members_addr;
  func::__NewChatRoom new_chat_room = (func::__NewChatRoom)new_chat_room_addr;
  func::__FreeChatRoom free_chat_room =
      (func::__FreeChatRoom)free_chat_room_addr;

  prototype::WeChatString chat_room_id(room_id);
  char chat_room_info[0x308] = {0};
  uint64_t addr = new_chat_room(reinterpret_cast<uint64_t>(&chat_room_info));
  uint64_t mgr = get_chat_room_mgr();
  success = get_members(mgr, reinterpret_cast<uint64_t>(&chat_room_id), addr);
  member.chat_room_id = wxutils::ReadWstringThenConvert(addr + 0x10);
  member.admin = wxutils::ReadWstringThenConvert(addr + 0x78);
  member.member_nickname = wxutils::ReadWstringThenConvert(addr + 0x50);
  member.admin_nickname = wxutils::ReadWstringThenConvert(addr + 0xA0);
  member.member = wxutils::ReadWeChatStr(addr + 0x30);
  free_chat_room(addr);
  return success;
}

int64_t wechat::WeChatService::SetTopMsg(uint64_t msg_id) {
  int64_t success = -1;
  uint64_t top_addr = base_addr_ + offset::kTopMsg;
  func::__DoTopMsg top_msg = (func::__DoTopMsg)top_addr;
  int64_t index = 0;
  int64_t local_id =
      wechat::WeChatDb::GetInstance().GetLocalIdByMsgId(msg_id, index);
  if (local_id <= 0 || index >> 32 == 0) {
    success = -2;
    return success;
  }
  LARGE_INTEGER l;
  l.HighPart = index >> 32;
  l.LowPart = (DWORD)local_id;
  uint64_t ptr = reinterpret_cast<uint64_t>(&l);
  success = top_msg(ptr, 1);

  return success;
}

int64_t wechat::WeChatService::RemoveTopMsg(const std::wstring& room_id,
                                            uint64_t msg_id) {
  int64_t success = -1;
  uint64_t remove_addr = base_addr_ + offset::kRemoveTopMsg;
  func::__RemoveTopMsg remove_top_msg = (func::__RemoveTopMsg)remove_addr;
  prototype::WeChatString* chat_room_id = BuildWechatString(room_id);
  const wchar_t* w_room = room_id.c_str();
  success = remove_top_msg(reinterpret_cast<UINT64>(w_room), msg_id,
                           reinterpret_cast<UINT64>(chat_room_id));
  return success;
}
TODO("InviteMemberToChatRoom")
int64_t wechat::WeChatService::InviteMemberToChatRoom(
    const std::wstring& room_id, const std::vector<std::wstring>& wxids) {
  int64_t success = -1;
  uint64_t invite_addr = base_addr_ + offset::kInviteMember;
  func::__InviteMemberToChatRoom invite =
      (func::__InviteMemberToChatRoom)invite_addr;
  const wchar_t* w_room = room_id.c_str();
  prototype::WeChatString* chat_room_id = BuildWechatString(room_id);
  std::vector<prototype::WeChatString> wxid_list;
  wechat::VectorInner* list = (wechat::VectorInner*)&wxid_list;
  int64_t head = (int64_t)&list->start;
  for (int i = 0; i < wxids.size(); i++) {
    prototype::WeChatString id(wxids[i]);
    wxid_list.push_back(id);
  }
  uint64_t temp[2] = {0};
  success = invite(reinterpret_cast<uint64_t>(w_room), head,
                   reinterpret_cast<uint64_t>(chat_room_id),
                   reinterpret_cast<uint64_t>(&temp));
  return success;
}

TODO("CreateChatRoom")
int64_t wechat::WeChatService::CreateChatRoom(
    const std::vector<std::wstring>& wxids) {
  int64_t success = -1;
  uint64_t get_chat_room_mgr_addr = base_addr_ + offset::kChatRoomMgr;
  uint64_t create_chat_room_addr = base_addr_ + offset::kCreateChatRoom;
  func::__GetChatRoomMgr get_chat_room_mgr =
      (func::__GetChatRoomMgr)get_chat_room_mgr_addr;
  func::__CreateChatRoom create_chat_room =
      (func::__CreateChatRoom)create_chat_room_addr;
  std::vector<prototype::WeChatString> wxid_list;
  wechat::VectorInner* list = (wechat::VectorInner*)&wxid_list;
  int64_t head = (int64_t)&list->start;
  for (int i = 0; i < wxids.size(); i++) {
    prototype::WeChatString id(wxids[i]);
    wxid_list.push_back(id);
  }
  int64_t end = list->end;
  uint64_t mgr = get_chat_room_mgr();
  success = create_chat_room(mgr, head, end);
  return success;
}

TODO("QuitChatRoom")
int64_t wechat::WeChatService::QuitChatRoom(const std::wstring& room_id) {
  int64_t success = -1;
  uint64_t get_chat_room_mgr_addr = base_addr_ + offset::kChatRoomMgr;
  uint64_t quit_chat_room_addr = base_addr_ + offset::kQuitChatRoom;
  func::__GetChatRoomMgr get_chat_room_mgr =
      (func::__GetChatRoomMgr)get_chat_room_mgr_addr;
  func::__QuitChatRoom quit_chat_room =
      (func::__QuitChatRoom)quit_chat_room_addr;
  uint64_t mgr = get_chat_room_mgr();
  prototype::WeChatString chat_room_id(room_id);
  success = quit_chat_room(mgr, reinterpret_cast<uint64_t>(&chat_room_id), 0);
  return success;
}

int64_t wechat::WeChatService::ForwardMsg(uint64_t msg_id,
                                          const std::wstring& wxid) {
  int64_t success = -1;
  uint64_t forward_addr = base_addr_ + offset::kForwardMsg;
  func::__ForwardMsg forward_msg = (func::__ForwardMsg)forward_addr;
  int64_t index = 0;
  int64_t local_id =
      wechat::WeChatDb::GetInstance().GetLocalIdByMsgId(msg_id, index);
  if (local_id <= 0 || index >> 32 == 0) {
    success = -2;

    return success;
  }
  LARGE_INTEGER l;
  l.HighPart = index >> 32;
  l.LowPart = (DWORD)local_id;
  prototype::WeChatString* recv = BuildWechatString(wxid);
  success = forward_msg(reinterpret_cast<uint64_t>(recv), l.QuadPart, 0x4, 0x0);
  return success;
}

TODO("GetSNSFirstPage")
int64_t wechat::WeChatService::GetSNSFirstPage() {
  int64_t success = -1;
  uint64_t sns_data_mgr_addr = base_addr_ + offset::kSNSDataMgr;
  uint64_t sns_first_page_addr = base_addr_ + offset::kSNSGetFirstPage;
  func::__GetSNSDataMgr sns_data_mgr = (func::__GetSNSDataMgr)sns_data_mgr_addr;
  func::__GetSNSFirstPage sns_first_page =
      (func::__GetSNSFirstPage)sns_first_page_addr;
  uint64_t mgr = sns_data_mgr();
  int64_t buff[16] = {0};
  success = sns_first_page(mgr, reinterpret_cast<uint64_t>(&buff), 1);
  return success;
}

TODO("GetSNSNextPage")
int64_t wechat::WeChatService::GetSNSNextPage(uint64_t sns_id) {
  int64_t success = -1;
  uint64_t time_line_mgr_addr = base_addr_ + offset::kSNSTimeLineMgr;
  uint64_t sns_next_page_addr = base_addr_ + offset::kSNSGetNextPageScene;
  func::__GetSnsTimeLineMgr time_line_mgr =
      (func::__GetSnsTimeLineMgr)time_line_mgr_addr;
  func::__GetSNSNextPageScene sns_next_page =
      (func::__GetSNSNextPageScene)sns_next_page_addr;
  uint64_t mgr = time_line_mgr();
  success = sns_next_page(mgr, sns_id);
  return success;
}

TODO("AddFavFromMsg")
int64_t wechat::WeChatService::AddFavFromMsg(uint64_t msg_id) {
  int64_t success = -1;
  uint64_t get_chat_mgr_addr = base_addr_ + offset::kGetChatMgr;
  uint64_t get_by_local_id_addr = base_addr_ + offset::kGetMgrByPrefixLocalId;
  uint64_t add_fav_addr = base_addr_ + offset::kAddFavFromMsg;
  uint64_t get_favorite_mgr_addr = base_addr_ + offset::kGetFavoriteMgr;
  uint64_t free_chat_msg_addr = base_addr_ + offset::kFreeChatMsg;
  func::__GetMgrByPrefixLocalId get_by_local_id =
      (func::__GetMgrByPrefixLocalId)get_by_local_id_addr;
  uint64_t new_chat_msg_addr = base_addr_ + offset::kChatMsgInstanceCounter;

  func::__AddFavFromMsg add_fav = (func::__AddFavFromMsg)add_fav_addr;
  func::__GetChatMgr get_chat_mgr = (func::__GetChatMgr)get_chat_mgr_addr;
  func::__GetFavoriteMgr get_favorite_mgr =
      (func::__GetFavoriteMgr)get_favorite_mgr_addr;
  func::__FreeChatMsg free_chat_msg = (func::__FreeChatMsg)free_chat_msg_addr;
  func::__NewChatMsg new_chat_msg = (func::__NewChatMsg)new_chat_msg_addr;

  int64_t index = 0;
  int64_t local_id =
      wechat::WeChatDb::GetInstance().GetLocalIdByMsgId(msg_id, index);
  if (local_id <= 0 || index >> 32 == 0) {
    success = -2;
    return success;
  }
  char chat_msg[0x460] = {0};
  LARGE_INTEGER l;
  l.HighPart = index >> 32;
  l.LowPart = (DWORD)local_id;
  uint64_t p_chat_msg = new_chat_msg(reinterpret_cast<uint64_t>(&chat_msg));

  get_chat_mgr();
  get_by_local_id(l.QuadPart, p_chat_msg);
  uint64_t mgr = get_favorite_mgr();
  success = add_fav(mgr, p_chat_msg);
  free_chat_msg(p_chat_msg);
  return success;
}

TODO("AddFavFromImage")
int64_t wechat::WeChatService::AddFavFromImage(const std::wstring& wxid,
                                               const std::wstring& image_path) {
  int64_t success = -1;
  uint64_t get_favorite_mgr_addr = base_addr_ + offset::kGetFavoriteMgr;
  uint64_t add_fav_from_image_addr = base_addr_ + offset::kAddFavFromImage;
  prototype::WeChatString* send_id = BuildWechatString(wxid);
  prototype::WeChatString* path = BuildWechatString(image_path);
  func::__GetFavoriteMgr get_favorite_mgr =
      (func::__GetFavoriteMgr)get_favorite_mgr_addr;
  func::__AddFavFromImage add_fav_from_image =
      (func::__AddFavFromImage)add_fav_from_image_addr;
  uint64_t mgr = get_favorite_mgr();
  success = add_fav_from_image(mgr, reinterpret_cast<uint64_t>(path),
                               reinterpret_cast<uint64_t>(send_id));
  return success;
}
TODO("SendAtText")
int64_t wechat::WeChatService::SendAtText(
    const std::wstring& room_id, const std::vector<std::wstring>& wxids,
    const std::wstring& msg) {
  int64_t success = -1;
  std::vector<prototype::WeChatString> wxid_list;
  wechat::VectorInner* list = (wechat::VectorInner*)&wxid_list;
  std::wstring at_msg = L"";
  int number = 0;
  for (unsigned int i = 0; i < wxids.size(); i++) {
    std::wstring nickname;
    std::wstring at_all = L"notify@all";
    if (at_all.compare(wxids[i]) == 0) {
      nickname = L"\u6240\u6709\u4eba";
    } else {
      // nickname = GetContactOrChatRoomNickname(wxids[i]);
    }
    if (nickname.length() == 0) {
      continue;
    }
    prototype::WeChatString id(wxids[i]);
    wxid_list.push_back(id);
    at_msg = at_msg + L"@" + nickname + L" ";
    number++;
  }
  if (number < 1) {
    success = -2;
    return success;
  }
  at_msg += msg;

  INT64 head = (INT64)&list->start;
  prototype::WeChatString to_user(room_id);
  prototype::WeChatString text_msg(at_msg);
  uint64_t send_message_mgr_addr = base_addr_ + offset::kGetSendMessageMgr;
  uint64_t send_text_msg_addr = base_addr_ + offset::kSendTextMsg;
  uint64_t free_chat_msg_addr = base_addr_ + offset::kFreeChatMsg;
  char chat_msg[0x460] = {0};
  func::__GetSendMessageMgr mgr =
      (func::__GetSendMessageMgr)send_message_mgr_addr;
  func::__SendTextMsg send = (func::__SendTextMsg)send_text_msg_addr;
  func::__FreeChatMsg free = (func::__FreeChatMsg)free_chat_msg_addr;
  mgr();
  success = send(reinterpret_cast<uint64_t>(&chat_msg),
                 reinterpret_cast<uint64_t>(&to_user),
                 reinterpret_cast<uint64_t>(&text_msg), head, 1, 1, 0, 0);
  free(reinterpret_cast<uint64_t>(&chat_msg));
  return 0;
}
TODO("GetContactOrChatRoomNickname")
std::wstring wechat::WeChatService::GetContactOrChatRoomNickname(
    const std::wstring& wxid) {
  int64_t success = -1;
  prototype::WeChatString to_user(wxid);
  uint64_t get_contact_mgr_addr = base_addr_ + offset::kGetContactMgr;
  uint64_t new_contact_addr = base_addr_ + offset::kNewContact;
  uint64_t get_contact_addr = base_addr_ + offset::kGetContact;
  uint64_t free_contact_addr = base_addr_ + offset::kFreeContact;
  func::__GetContactMgr get_contact_mgr =
      (func::__GetContactMgr)get_contact_mgr_addr;
  func::__GetContact get_contact = (func::__GetContact)get_contact_addr;
  func::__NewContact new_contact = (func::__NewContact)new_contact_addr;
  func::__FreeContact free_contact = (func::__FreeContact)free_contact_addr;
  char buff[0x6A9] = {0};
  uint64_t contact = new_contact(reinterpret_cast<uint64_t>(&buff));
  uint64_t mgr = get_contact_mgr();
  success = get_contact(mgr, reinterpret_cast<uint64_t>(&to_user), contact);

  if (success == 1) {
    std::wstring nickname = wxutils::ReadWstring(contact + 0xA0);
    free_contact(contact);
    return nickname;
  } else {
    free_contact(contact);
    return L"";
  }
}

TODO("GetContactByWxid")
int64_t wechat::WeChatService::GetContactByWxid(const std::wstring& wxid,
                                                ContactProfileInner& profile) {
  int64_t success = -1;
  prototype::WeChatString to_user(wxid);
  uint64_t get_contact_mgr_addr = base_addr_ + offset::kGetContactMgr;
  uint64_t new_contact_addr = base_addr_ + offset::kNewContact;
  uint64_t get_contact_addr = base_addr_ + offset::kGetContact;
  uint64_t free_contact_addr = base_addr_ + offset::kFreeContact;
  func::__GetContactMgr get_contact_mgr =
      (func::__GetContactMgr)get_contact_mgr_addr;
  func::__GetContact get_contact = (func::__GetContact)get_contact_addr;
  func::__NewContact new_contact = (func::__NewContact)new_contact_addr;
  func::__FreeContact free_contact = (func::__FreeContact)free_contact_addr;
  char buff[0x6A9] = {0};
  uint64_t contact = new_contact(reinterpret_cast<uint64_t>(&buff));
  uint64_t mgr = get_contact_mgr();
  success = get_contact(mgr, reinterpret_cast<uint64_t>(&to_user), contact);
  profile.wxid = wxutils::ReadWstringThenConvert(contact + 0x10);
  profile.account = wxutils::ReadWstringThenConvert(contact + 0x30);
  profile.v3 = wxutils::ReadWstringThenConvert(contact + 0x50);
  profile.nickname = wxutils::ReadWstringThenConvert(contact + 0xA0);
  profile.head_image = wxutils::ReadWstringThenConvert(contact + 0x188);
  free_contact(contact);
  return success;
}

TODO("DoDownloadTask")
int64_t wechat::WeChatService::DoDownloadTask(uint64_t msg_id) {
  int64_t success = -1;
  uint64_t get_by_local_id_addr = base_addr_ + offset::kGetMgrByPrefixLocalId;
  func::__GetMgrByPrefixLocalId get_by_local_id =
      (func::__GetMgrByPrefixLocalId)get_by_local_id_addr;

  uint64_t get_chat_mgr_addr = base_addr_ + offset::kGetChatMgr;
  func::__GetChatMgr get_chat_mgr = (func::__GetChatMgr)get_chat_mgr_addr;

  uint64_t free_chat_msg_addr = base_addr_ + offset::kFreeChatMsg;
  func::__FreeChatMsg free_chat_msg = (func::__FreeChatMsg)free_chat_msg_addr;

  uint64_t new_chat_msg_addr = base_addr_ + offset::kChatMsgInstanceCounter;
  func::__NewChatMsg new_chat_msg = (func::__NewChatMsg)new_chat_msg_addr;

  uint64_t get_current_data_path_addr =
      base_addr_ + offset::kGetCurrentDataPath;
  func::__GetCurrentDataPath GetCurrentDataPath =
      (func::__GetCurrentDataPath)get_current_data_path_addr;

  uint64_t new_app_msg_info_addr = base_addr_ + offset::kNewAppMsgInfo;
  func::__NewAppMsgInfo new_app_msg_info =
      (func::__NewAppMsgInfo)new_app_msg_info_addr;

  uint64_t free_app_msg_info_addr = base_addr_ + offset::kFreeAppMsgInfo;
  func::__FreeAppMsgInfo free_app_msg_info =
      (func::__NewAppMsgInfo)free_app_msg_info_addr;

  uint64_t xml_to_app_info_addr = base_addr_ + offset::kParseAppMsgXml;
  func::__ParseAppMsgXml xml_to_app_info =
      (func::__ParseAppMsgXml)xml_to_app_info_addr;

  uint64_t get_pre_download_mgr_addr = base_addr_ + offset::kGetPreDownLoadMgr;
  func::__GetPreDownLoadMgr get_pre_download_mgr =
      (func::__GetPreDownLoadMgr)get_pre_download_mgr_addr;

  uint64_t push_attach_task_addr = base_addr_ + offset::kPushAttachTask;
  func::__PushAttachTask push_attach_task =
      (func::__PushAttachTask)push_attach_task_addr;

  int64_t index = 0;
  int64_t local_id =
      wechat::WeChatDb::GetInstance().GetLocalIdByMsgId(msg_id, index);
  if (local_id <= 0 || index >> 32 == 0) {
    success = -2;
    return success;
  }
  char* chat_msg = base::utils::WxHeapAlloc<char>(0x460);
  LARGE_INTEGER l;
  l.HighPart = index >> 32;
  l.LowPart = (DWORD)local_id;
  uint64_t p_chat_msg = new_chat_msg(reinterpret_cast<uint64_t>(chat_msg));

  get_chat_mgr();
  get_by_local_id(l.QuadPart, p_chat_msg);

  std::wstring save_path = L"";
  std::wstring thumb_path = L"";

  prototype::WeChatString current_data_path;
  GetCurrentDataPath(reinterpret_cast<ULONG_PTR>(&current_data_path));

  if (current_data_path.length > 0) {
    save_path += current_data_path.ptr;
    save_path += L"wxhelper";
  } else {
    return -1;
  }

  if (!wxutils::FindOrCreateDirectory(save_path)) {
    return -3;
  }
  int64_t type = *(int64_t*)(chat_msg + 0x38);
  wchar_t* content = *(wchar_t**)(chat_msg + 0x88);
  DWORD len = *(DWORD*)(chat_msg + 0x94);
  std::wstring tmp_content(content, len);
  prototype::WeChatString* we_content = BuildWechatString(tmp_content);

  switch (type) {
    case 0x3: {
      save_path += L"\\image";
      if (!wxutils::FindOrCreateDirectory(save_path)) {
        return -3;
      }
      thumb_path = save_path + L"\\" + std::to_wstring(msg_id) + L"_t.dat";
      save_path = save_path + L"\\" + std::to_wstring(msg_id) + L".dat";
      break;
    }
    case 0x3E:
    case 0x2B: {
      save_path += L"\\video";
      if (!wxutils::FindOrCreateDirectory(save_path)) {
        return -3;
      }
      thumb_path = save_path + L"\\" + std::to_wstring(msg_id) + L".jpg";
      save_path = save_path + L"\\" + std::to_wstring(msg_id) + L".mp4";

      break;
    }
    case 0x31: {
      save_path += L"\\file";
      if (!wxutils::FindOrCreateDirectory(save_path)) {
        return -3;
      }
      char* p_xml_app_msg = base::utils::WxHeapAlloc<char>(0x3000);
      uint64_t xml_msg =
          new_app_msg_info(reinterpret_cast<uint64_t>(p_xml_app_msg));
      uint64_t result =
          xml_to_app_info(xml_msg, reinterpret_cast<uint64_t>(we_content), 1);
      if (result != 1) {
        return -4;
      }
      std::wstring file_name = wxutils::ReadWstring(xml_msg + 0x70);
      save_path =
          save_path + L"\\" + std::to_wstring(msg_id) + L"_" + file_name;
      free_app_msg_info(xml_msg);
      break;
    }
    default:
      break;
  }
  prototype::WeChatString* we_save_path = BuildWechatString(save_path);
  prototype::WeChatString* we_thumb_path = BuildWechatString(thumb_path);
  int temp = 1;
  memcpy(chat_msg + 0x280, we_thumb_path, sizeof(prototype::WeChatString));
  memcpy(chat_msg + 0x2A0, we_save_path, sizeof(prototype::WeChatString));
  memcpy(chat_msg + 0x40C, &temp, sizeof(temp));
  UINT64 mgr = get_pre_download_mgr();
  success = push_attach_task(mgr, p_chat_msg, 0, 1);
  free_chat_msg(p_chat_msg);

  return success;
}

TODO("ForwardPublicMsg")
int64_t wechat::WeChatService::ForwardPublicMsg(const std::wstring& wxid,
                                                const std::wstring& title,
                                                const std::wstring& url,
                                                const std::wstring& thumb_url,
                                                const std::wstring& sender_id,
                                                const std::wstring& sender_name,
                                                const std::wstring& digest) {
  return 0;
}

TODO("ForwardPublicMsgByMsgId")
int64_t wechat::WeChatService::ForwardPublicMsgByMsgId(const std::wstring& wxid,
                                                       uint64_t msg_id) {
  return 0;
}
TODO("DecodeImage")
int64_t wechat::WeChatService::DecodeImage(const std::wstring& file_path,
                                           const std::wstring& save_dir) {
  if (!wxutils::FindOrCreateDirectory(save_dir)) {
    return 0;
  }

  int64_t pos_begin = file_path.find_last_of(L"\\") + 1;
  int64_t pos_end = file_path.find_last_of(L".");
  std::wstring file_name = file_path.substr(pos_begin, pos_end - pos_begin);
  HANDLE h_origin_file =
      CreateFileW(file_path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING,
                  FILE_ATTRIBUTE_NORMAL, NULL);
  char buffer[BUFSIZE] = {0};
  DWORD bytes_read = 0;
  DWORD bytes_write = 0;
  unsigned char magic_head[4] = {0};
  std::wstring suffix;
  short key = 0;
  if (ReadFile(h_origin_file, buffer, BUFSIZE, &bytes_read, NULL)) {
    memcpy(magic_head, buffer, 3);
  } else {
    CloseHandle(h_origin_file);
    return 0;
  }
  if ((magic_head[0] ^ JPEG0) == (magic_head[1] ^ JPEG1)) {
    key = magic_head[0] ^ JPEG0;
    suffix = L".jpg";
  } else if ((magic_head[0] ^ PNG1) == (magic_head[1] ^ PNG2)) {
    key = magic_head[0] ^ PNG1;
    suffix = L".png";
  } else if ((magic_head[0] ^ GIF0) == (magic_head[1] ^ GIF1)) {
    key = magic_head[0] ^ GIF0;
    suffix = L".gif";
  } else if ((magic_head[0] ^ BMP0) == (magic_head[1] ^ BMP1)) {
    key = magic_head[0] ^ BMP0;
    suffix = L".bmp";
  } else {
    key = -1;
    suffix = L".dat";
  }
  std::wstring save_img_path = save_dir + L"\\" + file_name + suffix;
  HANDLE save_img = CreateFileW(save_img_path.c_str(), GENERIC_ALL, 0, NULL,
                                CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (save_img == INVALID_HANDLE_VALUE) {
    return 0;
  }
  if (key > 0) {
    for (unsigned int i = 0; i < bytes_read; i++) {
      buffer[i] ^= key;
    }
  }
  if (!WriteFile(save_img, (LPCVOID)buffer, bytes_read, &bytes_write, 0)) {
    CloseHandle(h_origin_file);
    CloseHandle(save_img);
    return 0;
  }

  do {
    if (ReadFile(h_origin_file, buffer, BUFSIZE, &bytes_read, NULL)) {
      if (key > 0) {
        for (unsigned int i = 0; i < bytes_read; i++) {
          buffer[i] ^= key;
        }
      }
      if (!WriteFile(save_img, (LPCVOID)buffer, bytes_read, &bytes_write, 0)) {
        CloseHandle(h_origin_file);
        CloseHandle(save_img);
        return 0;
      }
    }
  } while (bytes_read == BUFSIZE);
  CloseHandle(h_origin_file);
  CloseHandle(save_img);
  return 1;
}
TODO("GetVoiceByDB")
int64_t wechat::WeChatService::GetVoiceByDB(ULONG64 msg_id,
                                            const std::wstring& dir) {
  return 0;
}
TODO("SendCustomEmotion")
int64_t wechat::WeChatService::SendCustomEmotion(const std::wstring& file_path,
                                                 const std::wstring& wxid) {
  int64_t success = -1;
  uint64_t get_custom_smiley_mgr_addr =
      base_addr_ + offset::kGetCustomSmileyMgr;
  func::__GetCustomSmileyMgr get_custom_smiley_mgr =
      (func::__GetCustomSmileyMgr)get_custom_smiley_mgr_addr;
  uint64_t send_custom_emotion_addr = base_addr_ + offset::kSendCustomEmotion;
  func::__SendCustomEmotion send_custom_emotion =
      (func::__SendCustomEmotion)send_custom_emotion_addr;
  prototype::WeChatString* path = BuildWechatString(file_path);
  prototype::WeChatString* recv = BuildWechatString(wxid);
  int64_t* temp = base::utils::WxHeapAlloc<int64_t>(0x20);
  memset(temp, 0, 0x20);
  uint64_t mgr = get_custom_smiley_mgr();
  success = send_custom_emotion(
      mgr, reinterpret_cast<uint64_t>(path), reinterpret_cast<uint64_t>(temp),
      reinterpret_cast<uint64_t>(recv), 2, reinterpret_cast<uint64_t>(temp), 0,
      reinterpret_cast<uint64_t>(temp));
  return success;
}

TODO("SendApplet")
int64_t wechat::WeChatService::SendApplet(
    const std::wstring& recv_wxid, const std::wstring& waid_suff,
    const std::wstring& waid_w, const std::wstring& waid_s,
    const std::wstring& wa_wxid, const std::wstring& json_param,
    const std::wstring& head_image, const std::wstring& big_image,
    const std::wstring& index_page) {
  int64_t success = -1;
  if (js_api_addr_ == 0) {
    auto vec2 =
        base::memory::ScanAndMatchValue(base_addr_ + 0x32D1318, 0x1000, 0x8);
    for (int i = 0; i < vec2.size(); i++) {
      int64_t ptr = vec2.at(i);
      if (*(int64_t*)ptr == base_addr_ + 0x32D1318) {
        js_api_addr_ = ptr;
        break;
      }
    }
  }
  if (js_api_addr_ == 0) {
    success = -2;
    return success;
  }

  uint64_t share_app_msg_addr = base_addr_ + offset::kNewJsApiShareAppMessage;
  func::__JsApiShareAppMessage share_app_msg =
      (func::__JsApiShareAppMessage)share_app_msg_addr;

  uint64_t init_addr = base_addr_ + offset::kInitJsConfig;
  func::__InitJsConfig init = (func::__InitJsConfig)init_addr;

  uint64_t send_applet_addr = base_addr_ + offset::kSendApplet;
  func::__SendApplet send_applet = (func::__SendApplet)send_applet_addr;

  uint64_t get_by_waid_addr = base_addr_ + offset::kGetAppInfoByWaid;
  func::__GetAppInfoByWaid get_app_info =
      (func::__GetAppInfoByWaid)get_by_waid_addr;

  uint64_t copy_app_req_addr = base_addr_ + offset::kCopyShareAppMessageRequest;
  func::__CopyShareAppMessageRequest copy_app_req =
      (func::__CopyShareAppMessageRequest)copy_app_req_addr;

  uint64_t new_wa_msg_addr = base_addr_ + offset::kNewWAUpdatableMsgInfo;
  func::__NewWAUpdatableMsgInfo new_wa_msg =
      (func::__NewWAUpdatableMsgInfo)new_wa_msg_addr;

  uint64_t free_wa_msg_addr = base_addr_ + offset::kFreeWAUpdatableMsgInfo;
  func::__FreeWAUpdatableMsgInfo free_wa_msg =
      (func::__FreeWAUpdatableMsgInfo)free_wa_msg_addr;

  std::vector<prototype::WeChatString>* temp =
      base::utils::WxHeapAlloc<std::vector<prototype::WeChatString>>(0x20);
  // std::vector<prototype::WeChatString>*  temp = new
  // std::vector<prototype::WeChatString>();
  wechat::VectorInner* list = (wechat::VectorInner*)temp;

  prototype::WeChatString* member = BuildWechatString(recv_wxid);
#ifdef _DEBUG
  list->head = reinterpret_cast<uint64_t>(member);
#endif
  list->start = reinterpret_cast<uint64_t>(member);
  list->finsh = reinterpret_cast<uint64_t>(member) + 0x20;
  list->end = reinterpret_cast<uint64_t>(member) + 0x20;

  uint64_t head = reinterpret_cast<uint64_t>(&(list->start));

  prototype::WeChatString* waid_cat = BuildWechatString(waid_suff);
  prototype::WeChatString* waid = BuildWechatString(waid_w);

  prototype::WeChatString* waid_2 = BuildWechatString(waid_suff);

  prototype::WeChatString* waid_str = BuildWechatString(waid_s);
  prototype::WeChatString* app_wxid = BuildWechatString(wa_wxid);
  prototype::WeChatString* json_str = BuildWechatString(json_param);
  prototype::WeChatString* head_image_url = BuildWechatString(head_image);
  prototype::WeChatString* image = BuildWechatString(big_image);
  prototype::WeChatString* index = BuildWechatString(index_page);

  uint64_t app_msg = js_api_addr_;

  uint64_t data = *(uint64_t*)(app_msg + 0x8);
  char* share_req = base::utils::WxHeapAlloc<char>(0x2000);

  char* mid_ptr = base::utils::WxHeapAlloc<char>(0x18);
  memcpy(mid_ptr, &share_req, sizeof(INT64));
  memcpy(mid_ptr + 0x8, &share_req, sizeof(INT64));
  memcpy(mid_ptr + 0x10, &share_req, sizeof(INT64));

  memcpy((void*)data, mid_ptr, 0x18);

  memcpy(share_req, (void*)(app_msg + 0x8), sizeof(uint64_t));
  memcpy(share_req + 0x8, (void*)(app_msg + 0x8), sizeof(uint64_t));
  memcpy(share_req + 0x10, (void*)(app_msg + 0x8), sizeof(uint64_t));
  memcpy(share_req + 0x20, waid_2, sizeof(prototype::WeChatString));
  memcpy(share_req + 0x48, waid_str, sizeof(prototype::WeChatStr));
  memcpy(share_req + 0x98, app_wxid, sizeof(prototype::WeChatStr));
  memcpy(share_req + 0xF8, json_str, sizeof(prototype::WeChatStr));
  memcpy(share_req + 0x178, head_image_url, sizeof(prototype::WeChatStr));
  memcpy(share_req + 0x198, image, sizeof(prototype::WeChatStr));
  memcpy(share_req + 0x1c0, index, sizeof(prototype::WeChatStr));

  success = send_applet(app_msg, reinterpret_cast<uint64_t>(waid_cat), head, 0);
  return success;
}

int64_t wechat::WeChatService::SendPatMsg(const std::wstring& room_id,
                                          const std::wstring& wxid) {
  int64_t success = -1;
  uint64_t send_pat_msg_addr = base_addr_ + offset::kSendPatMsg;
  func::__SendPatMsg send_pat_msg = (func::__SendPatMsg)send_pat_msg_addr;
  prototype::WeChatString chat_room(room_id);
  prototype::WeChatString target(wxid);
  success = send_pat_msg(reinterpret_cast<uint64_t>(&chat_room),
                         reinterpret_cast<uint64_t>(&target));
  return success;
}

TODO("DoOCRTask")
int64_t wechat::WeChatService::DoOCRTask(const std::wstring& img_path,
                                         std::string& result) {
  int64_t success = -1;
  uint64_t ocr_manager_addr = base_addr_ + offset::kGetOCRManager;
  func::__GetOCRManager ocr_manager = (func::__GetOCRManager)ocr_manager_addr;

  uint64_t do_ocr_task_addr = base_addr_ + offset::kDoOCRTask;
  func::__DoOCRTask do_ocr_task = (func::__DoOCRTask)do_ocr_task_addr;

  prototype::WeChatString img(img_path);
  std::vector<INT64>* temp =
      base::utils::WxHeapAlloc<std::vector<int64_t>>(0x20);
  int64_t unkonwn = 0;
  wechat::VectorInner* list = (wechat::VectorInner*)temp;
  list->start = reinterpret_cast<INT64>(&list->start);
  list->finsh = list->start;
  char buff[0x28] = {0};
  memcpy(buff, &list->start, sizeof(INT64));
  uint64_t mgr = ocr_manager();
  success = do_ocr_task(mgr, reinterpret_cast<uint64_t>(&img), 1,
                        reinterpret_cast<uint64_t>(buff),
                        reinterpret_cast<uint64_t>(&unkonwn));
  int64_t number = *(int64_t*)(buff + 0x8);
  if (number > 0) {
    int64_t header = *(int64_t*)(buff);
    for (unsigned int i = 0; i < number; i++) {
      int64_t content = *(int64_t*)header;
      result += wxutils::ReadWstringThenConvert(content + 0x28);
      result += "\r\n";
      header = content;
    }
  }
  return success;
}

int64_t wechat::WeChatService::LockWeChat() {
  int64_t success = -1;
  uint64_t lock_mgr_addr = base_addr_ + offset::kGetLockWechatMgr;
  uint64_t request_lock_addr = base_addr_ + offset::kRequestLockWechat;
  func::__GetLockWechatMgr get_lock_mgr =
      (func::__GetLockWechatMgr)lock_mgr_addr;
  func::__RequestLockWechat request_lock =
      (func::__RequestLockWechat)request_lock_addr;
  uint64_t mgr = get_lock_mgr();
  success = request_lock(mgr);
  return success;
}

int64_t wechat::WeChatService::UnlockWeChat() {
  int64_t success = -1;
  uint64_t lock_mgr_addr = base_addr_ + offset::kGetLockWechatMgr;
  uint64_t request_unlock_addr = base_addr_ + offset::kRequestUnLockWechat;
  func::__GetLockWechatMgr get_lock_mgr =
      (func::__GetLockWechatMgr)lock_mgr_addr;
  func::__RequestUnLockWechat request_unlock =
      (func::__RequestUnLockWechat)request_unlock_addr;
  uint64_t mgr = get_lock_mgr();
  success = request_unlock(mgr);

  return success;
}

int64_t wechat::WeChatService::EnterWeChat() {
  int64_t success = -1;
  int64_t base_addr = wxutils::GetWeChatWinBase();
  uint64_t click_cb_addr = base_addr + offset::kOnLoginBtnClick;
  func::__OnLoginBtnClick cb = (func::__OnLoginBtnClick)click_cb_addr;
  auto vec =
      base::memory::ScanAndMatchValue(base_addr + 0x4ecedf8, 0x1000, 0x8);
  for (int i = 0; i < vec.size(); i++) {
    int64_t ptr = vec.at(i);
    if (*(int64_t*)ptr == base_addr + 0x4ecedf8) {
      int64_t login_wnd = ptr;
      success = cb(ptr);
      break;
    }
  }
  return success;
}

TODO("SendMultiAtText")
int64_t wechat::WeChatService::SendMultiAtText(
    const std::wstring& room_id,
    const std::vector<std::pair<std::wstring, std::wstring>>& at) {
  int64_t success = -1;
  std::vector<prototype::WeChatString> wxid_list;
  wechat::VectorInner* list = (wechat::VectorInner*)&wxid_list;
  std::wstring at_msg = L"";
  int number = 0;
  for (unsigned int i = 0; i < at.size(); i++) {
    std::wstring nickname;
    std::wstring at_all = L"notify@all";
    if (at_all.compare(at[i].first) == 0) {
      nickname = L"\u6240\u6709\u4eba";
    } else {
      // nickname = GetContactOrChatRoomNickname(at[i].first);
      nickname = L"";
    }
    if (nickname.length() == 0) {
      continue;
    }
    prototype::WeChatString id(at[i].first);
    wxid_list.push_back(id);
    at_msg = at_msg + L"@" + nickname + L" " + at[i].second + L" ";
    number++;
  }
  if (number < 1) {
    success = -2;
    return success;
  }
  int64_t head = (int64_t)&list->start;
  prototype::WeChatString to_user(room_id);
  prototype::WeChatString text_msg(at_msg);
  uint64_t send_message_mgr_addr = base_addr_ + offset::kGetSendMessageMgr;
  uint64_t send_text_msg_addr = base_addr_ + offset::kSendTextMsg;
  uint64_t free_chat_msg_addr = base_addr_ + offset::kFreeChatMsg;
  char chat_msg[0x460] = {0};
  func::__GetSendMessageMgr mgr =
      (func::__GetSendMessageMgr)send_message_mgr_addr;
  func::__SendTextMsg send = (func::__SendTextMsg)send_text_msg_addr;
  func::__FreeChatMsg free = (func::__FreeChatMsg)free_chat_msg_addr;
  mgr();
  success = send(reinterpret_cast<uint64_t>(&chat_msg),
                 reinterpret_cast<uint64_t>(&to_user),
                 reinterpret_cast<uint64_t>(&text_msg), head, 1, 1, 0, 0);
  free(reinterpret_cast<uint64_t>(&chat_msg));
  return success;
}

std::string wechat::WeChatService::GetLoginUrl() {
  uint64_t login_mgr_addr = base_addr_ + offset::kGetQRCodeLoginMgr;
  func::__GetQRCodeLoginMgr get = (func::__GetQRCodeLoginMgr)login_mgr_addr;
  uint64_t addr = get();
  std::string login_url = wxutils::ReadWeChatStr(addr + 0x68);
  return "http://weixin.qq.com/x/" + login_url;
}

void wechat::WeChatService::SetBaseAddr(uint64_t addr) {
  this->base_addr_ = addr;
}

void wechat::WeChatService::SetJsApiAddr(uint64_t addr) {
  this->js_api_addr_ = addr;
}

TODO("TranslateVoice")
int64_t wechat::WeChatService::TranslateVoice(uint64_t msg_id) {
  int64_t success = -1;
  uint64_t get_by_local_id_addr = base_addr_ + offset::kGetMgrByPrefixLocalId;
  func::__GetMgrByPrefixLocalId get_by_local_id =
      (func::__GetMgrByPrefixLocalId)get_by_local_id_addr;

  uint64_t get_chat_mgr_addr = base_addr_ + offset::kGetChatMgr;
  func::__GetChatMgr get_chat_mgr = (func::__GetChatMgr)get_chat_mgr_addr;

  uint64_t free_chat_msg_addr = base_addr_ + offset::kFreeChatMsg;
  func::__FreeChatMsg free_chat_msg = (func::__FreeChatMsg)free_chat_msg_addr;

  uint64_t new_chat_msg_addr = base_addr_ + offset::kChatMsgInstanceCounter;
  func::__NewChatMsg new_chat_msg = (func::__NewChatMsg)new_chat_msg_addr;

  uint64_t update_addr = base_addr_ + offset::kUpdateMsg;
  func::__UpdateMsg update = (func::__UpdateMsg)update_addr;

  uint64_t get_voice_mgr_addr = base_addr_ + offset::kGetVoiceMgr;
  func::__GetVoiceMgr get_voice_mgr = (func::__GetVoiceMgr)get_voice_mgr_addr;

  uint64_t to_msg_addr = base_addr_ + offset::kChatMsg2NetSceneSendMsg;
  func::__ChatMsg2NetSceneSendMsg to_msg =
      (func::__ChatMsg2NetSceneSendMsg)to_msg_addr;

  uint64_t trans_addr = base_addr_ + offset::kTranslateVoice;
  func::__TranslateVoice translate_voice = (func::__TranslateVoice)trans_addr;

  char temp_msg[0x460] = {0};

  char* chat_msg = base::utils::WxHeapAlloc<char>(0x460);
  int64_t index = 0;
  int64_t local_id =
      wechat::WeChatDb::GetInstance().GetLocalIdByMsgId(msg_id, index);
  if (local_id <= 0 || index >> 32 == 0) {
    success = -2;

    return success;
  }
  LARGE_INTEGER l;
  l.HighPart = index >> 32;
  l.LowPart = (DWORD)local_id;
  uint64_t p_chat_msg = new_chat_msg(reinterpret_cast<uint64_t>(chat_msg));
  get_chat_mgr();
  get_by_local_id(l.QuadPart, p_chat_msg);
  uint64_t mgr = get_chat_mgr();
  update(mgr, p_chat_msg, 0);

  uint64_t voice_mgr = get_voice_mgr();
  uint64_t msg = to_msg(reinterpret_cast<uint64_t>(&temp_msg), p_chat_msg);

  success = translate_voice(voice_mgr, msg, 0);

  return success;
}

TODO("GetTranslateVoiceText")
std::string wechat::WeChatService::GetTranslateVoiceText(uint64_t msg_id) {
  std::string content =
      wechat::WeChatDb::GetInstance().GetChatMsgStrContentByMsgId(msg_id);
  if (content.empty()) {
    return {};
  }

  tinyxml2::XMLDocument doc;
  if (doc.Parse(content.c_str(), content.size()) != 0) {
    SPDLOG_INFO("tinyxml2 parse error");
    return {};
  }
  tinyxml2::XMLElement* msg = doc.FirstChildElement("msg");
  if (msg != nullptr) {
    tinyxml2::XMLElement* voicetrans = msg->FirstChildElement("voicetrans");
    if (voicetrans != nullptr) {
      const char* value = voicetrans->Attribute("transtext", nullptr);
      return value;
    }
  }
  return "";
}

TODO("OpenUrlByWeChatBrowser")
int64_t wechat::WeChatService::OpenUrlByWeChatBrowser(const std::wstring& url,
                                                      int flag) {
  int64_t success = -1;
  uint64_t config_addr = base_addr_ + offset::kNewWebViewPageConfig;
  func::__NewWebViewPageConfig config =
      (func::__NewWebViewPageConfig)config_addr;

  uint64_t free_config_addr = base_addr_ + offset::kFreeWebViewPageConfig;
  func::__FreeWebViewPageConfig free_config =
      (func::__FreeWebViewPageConfig)free_config_addr;

  uint64_t web_view_mgr_addr = base_addr_ + offset::kGetWebViewMgr;
  func::__GetWebViewMgr web_view_mgr = (func::__GetWebViewMgr)web_view_mgr_addr;

  uint64_t show_addr = base_addr_ + offset::kShowWebView;
  func::__ShowWebView show_web_view = (func::__ShowWebView)show_addr;

  uint64_t set_url_addr = base_addr_ + offset::kSetUrl;
  func::__SetUrl set_url = (func::__SetUrl)set_url_addr;

  int a = flag >> 4;
  int b = flag & 0x1;
  int c = flag & 0x2;
  int d = flag & 0x4;
  int e = flag & 0x8;
  char* web_config = (char*)HeapAlloc(GetProcessHeap(), 0, 0xA20);

  uint64_t ptr = config(reinterpret_cast<uint64_t>(web_config));
  set_url(ptr + 0x868, reinterpret_cast<uint64_t>(url.c_str()), url.size());
  web_view_mgr();
  success = show_web_view(ptr, a, b, c, d, e);
  free_config(ptr);
  return success;
}

TODO("GetChatRoomMemberNickname")
std::wstring wechat::WeChatService::GetChatRoomMemberNickname(
    const std::wstring& room_id, const std::wstring& member_id) {
  return std::wstring();
}

TODO("DelContact")
int64_t wechat::WeChatService::DelContact(const std::wstring& wxid) {
  return 0;
}

TODO("SearchContact")
int64_t wechat::WeChatService::SearchContact(
    const std::wstring& keyword, wechat::SearchContactInner& contact) {
  return 0;
}

TODO("AddFriendByWxid")
int64_t wechat::WeChatService::AddFriendByWxid(const std::wstring& wxid,
                                               const std::wstring& msg) {
  return 0;
}

TODO("VerifyApply")
int64_t wechat::WeChatService::VerifyApply(const std::wstring& v3,
                                           const std::wstring& v4,
                                           int32_t permission) {
  return 0;
}

TODO("DoConfirmReceipt")
int64_t wechat::WeChatService::DoConfirmReceipt(
    const std::wstring& wxid, const std::wstring& transcationid,
    const std::wstring& transferid) {
  int success = -1;
  prototype::WeChatString recv_id(wxid);
  prototype::WeChatString transcation_id(transcationid);
  prototype::WeChatString transfer_id(transferid);

  return success;
}

TODO("DoRefuseReceipt")
int64_t wechat::WeChatService::DoRefuseReceipt(
    const std::wstring& wxid, const std::wstring& transcationid,
    const std::wstring& transferid) {
  return 0;
}
