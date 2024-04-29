#include "wechat_service.h"

#include "json_utils.h"
#include "memory.h"
#include "spdlog/spdlog.h"
#include "tinyxml2.h"
#include "utils.h"
#include "wechat_db.h"
#include "wxutils.h"
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
  flag[0] =  reinterpret_cast<uint64_t*>(temp3);
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
  return 0;
}

int64_t wechat::WeChatService::AddMemberToChatRoom(
    const std::wstring& room_id, const std::vector<std::wstring>& members) {
  return 0;
}

int64_t wechat::WeChatService::ModChatRoomMemberNickName(
    const std::wstring& room_id, const std::wstring& wxid,
    const std::wstring& nickname) {
  return 0;
}

int64_t wechat::WeChatService::DelMemberFromChatRoom(
    const std::wstring& room_id, const std::vector<std::wstring>& members) {
  return 0;
}

int64_t wechat::WeChatService::GetMemberFromChatRoom(
    const std::wstring& room_id, ChatRoomMemberInner& member) {
  return 0;
}

int64_t wechat::WeChatService::SetTopMsg(uint64_t msg_id) { return 0; }

int64_t wechat::WeChatService::RemoveTopMsg(const std::wstring& room_id,
                                            ULONG64 msg_id) {
  return 0;
}

int64_t wechat::WeChatService::InviteMemberToChatRoom(
    const std::wstring& room_id, const std::vector<std::wstring>& wxids) {
  return 0;
}

int64_t wechat::WeChatService::CreateChatRoom(
    const std::vector<std::wstring>& wxids) {
  return 0;
}

int64_t wechat::WeChatService::QuitChatRoom(const std::wstring& room_id) {
  return 0;
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

int64_t wechat::WeChatService::GetSNSFirstPage() { return 0; }

int64_t wechat::WeChatService::GetSNSNextPage(uint64_t sns_id) { return 0; }

int64_t wechat::WeChatService::AddFavFromMsg(uint64_t msg_id) { return 0; }

int64_t wechat::WeChatService::AddFavFromImage(const std::wstring& wxid,
                                               const std::wstring& image_path) {
  return 0;
}

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

int64_t wechat::WeChatService::GetContactByWxid(const std::wstring& wxid,
                                                ContactProfileInner& profile) {
  return 0;
}

int64_t wechat::WeChatService::DoDownloadTask(uint64_t msg_id) { return 0; }

int64_t wechat::WeChatService::ForwardPublicMsg(const std::wstring& wxid,
                                                const std::wstring& title,
                                                const std::wstring& url,
                                                const std::wstring& thumb_url,
                                                const std::wstring& sender_id,
                                                const std::wstring& sender_name,
                                                const std::wstring& digest) {
  return 0;
}

int64_t wechat::WeChatService::ForwardPublicMsgByMsgId(const std::wstring& wxid,
                                                       uint64_t msg_id) {
  return 0;
}

int64_t wechat::WeChatService::DecodeImage(const std::wstring& file_path,
                                           const std::wstring& save_dir) {
  return 0;
}

int64_t wechat::WeChatService::GetVoiceByDB(ULONG64 msg_id,
                                            const std::wstring& dir) {
  return 0;
}

int64_t wechat::WeChatService::SendCustomEmotion(const std::wstring& file_path,
                                                 const std::wstring& wxid) {
  return 0;
}

int64_t wechat::WeChatService::SendApplet(
    const std::wstring& recv_wxid, const std::wstring& waid_suff,
    const std::wstring& waid_w, const std::string& waid_s,
    const std::string& wa_wxid, const std::string& json_param,
    const std::string& head_image, const std::string& big_image,
    const std::string& index_page) {
  return 0;
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

int64_t wechat::WeChatService::DoOCRTask(const std::wstring& img_path,
                                         std::string& result) {
  return 0;
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
      base::memory::ScanAndMatchValue(base_addr + 0x34e0c18, 0x1000, 0x8);
  for (int i = 0; i < vec.size(); i++) {
    int64_t ptr = vec.at(i);
    if (*(int64_t*)ptr == base_addr + 0x34e0c18) {
      int64_t login_wnd = ptr;
      success = cb(ptr);
      break;
    }
  }
  return success;
}

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

void wechat::WeChatService::SetBaseAddr(uint64_t addr) {}

void wechat::WeChatService::SetJsApiAddr(uint64_t addr) {}

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
