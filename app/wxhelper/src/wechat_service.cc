#include "wechat_service.h"
#include "wxutils.h"
#include "utils.h"
#include "memory.h"
#include "db.h"
namespace offset = wxhelper::V3_9_8_25::offset;
namespace prototype = wxhelper::V3_9_8_25::prototype;
namespace func = wxhelper::V3_9_8_25::function;
namespace wxhelper {

prototype::WeChatString *BuildWechatString(const std::wstring &ws) {
  prototype::WeChatString *p =
      base::utils::WxHeapAlloc<prototype::WeChatString>(
          sizeof(prototype::WeChatString));
  wchar_t *p_chat_room_id =
      base::utils::WxHeapAlloc<wchar_t>((ws.size() + 1) * 2);
  wmemcpy(p_chat_room_id, ws.c_str(), ws.size() + 1);
  p->ptr = p_chat_room_id;
  p->length = static_cast<DWORD>(ws.size());
  p->max_length = static_cast<DWORD>(ws.size());
  p->c_len = 0;
  p->c_ptr = 0;
  return p;
}
WechatService::~WechatService() {}

INT64 WechatService::CheckLogin() {
  INT64 success = -1;
  UINT64 accout_service_addr = base_addr_ + offset::kGetAccountServiceMgr;
  func::__GetAccountService GetSevice =
      (func::__GetAccountService)accout_service_addr;
  UINT64 service_addr = GetSevice();
  if (service_addr) {
    success = *(UINT64*)(service_addr + 0x7F8);
  }
  return success;
}

INT64 WechatService::GetSelfInfo(common::SelfInfoInner& out) { 
  INT64 success = -1;
  UINT64 accout_service_addr = base_addr_ + offset::kGetAccountServiceMgr;
  UINT64 get_app_data_save_path_addr = base_addr_ + offset::kGetAppDataSavePath;
  UINT64 get_current_data_path_addr = base_addr_ + offset::kGetCurrentDataPath;
  func::__GetAccountService GetSevice = (func::__GetAccountService)accout_service_addr;
  func::__GetDataSavePath GetDataSavePath = (func::__GetDataSavePath)get_app_data_save_path_addr;
  func::__GetCurrentDataPath GetCurrentDataPath = (func::__GetCurrentDataPath)get_current_data_path_addr;

  UINT64 service_addr = GetSevice();
  if (service_addr) {
    if (*(INT64 *)(service_addr + 0x80) == 0 ||
        *(INT64 *)(service_addr + 0x80 + 0x10) == 0) {
      out.wxid = std::string();
    } else {
      if (*(INT64 *)(service_addr + 0x80 + 0x18) == 0xF) {
        out.wxid = std::string((char *)(service_addr + 0x80),
                               *(INT64 *)(service_addr + 0x80 + 0x10));
      } else {
        out.wxid = std::string(*(char **)(service_addr + 0x80),
                               *(INT64 *)(service_addr + 0x80 + 0x10));
      }
    }

    if (*(INT64 *)(service_addr + 0x108) == 0 ||
        *(INT64 *)(service_addr + 0x108 + 0x10) == 0) {
      out.account = std::string();
    } else {
      if (*(INT64 *)(service_addr + 0x108 + 0x18) == 0xF) {
        out.account = std::string((char *)(service_addr + 0x108),
                                  *(INT64 *)(service_addr + 0x108 + 0x10));
      } else {
        out.account = std::string(*(char **)(service_addr + 0x108),
                                  *(INT64 *)(service_addr + 0x108 + 0x10));
      }
    }

    if (*(INT64 *)(service_addr + 0x128) == 0 ||
        *(INT64 *)(service_addr + 0x128 + 0x10) == 0) {
      out.mobile = std::string();
    } else {
      if (*(INT64 *)(service_addr + 0x128 + 0x18) == 0xF) {
        out.mobile = std::string((char *)(service_addr + 0x128),
                                 *(INT64 *)(service_addr + 0x128 + 0x10));
      } else {
        out.mobile = std::string(*(char **)(service_addr + 0x128),
                                 *(INT64 *)(service_addr + 0x128 + 0x10));
      }
    }

    if (*(INT64 *)(service_addr + 0x148) == 0 ||
        *(INT64 *)(service_addr + 0x148 + 0x10) == 0) {
      out.signature = std::string();
    } else {
      if (*(INT64 *)(service_addr + 0x148 + 0x18) == 0xF) {
        out.signature = std::string((char *)(service_addr + 0x148),
                                    *(INT64 *)(service_addr + 0x148 + 0x10));
      } else {
        out.signature = std::string(*(char **)(service_addr + 0x148),
                                    *(INT64 *)(service_addr + 0x148 + 0x10));
      }
    }

    if (*(INT64 *)(service_addr + 0x168) == 0 ||
        *(INT64 *)(service_addr + 0x168 + 0x10) == 0) {
      out.country = std::string();
    } else {
      if (*(INT64 *)(service_addr + 0x168 + 0x18) == 0xF) {
        out.country = std::string((char *)(service_addr + 0x168),
                                  *(INT64 *)(service_addr + 0x168 + 0x10));
      } else {
        out.country = std::string(*(char **)(service_addr + 0x168),
                                  *(INT64 *)(service_addr + 0x168 + 0x10));
      }
    }

    if (*(INT64 *)(service_addr + 0x188) == 0 ||
        *(INT64 *)(service_addr + 0x188 + 0x10) == 0) {
      out.province = std::string();
    } else {
      if (*(INT64 *)(service_addr + 0x188 + 0x18) == 0xF) {
        out.province = std::string((char *)(service_addr + 0x188),
                                   *(INT64 *)(service_addr + 0x188 + 0x10));
      } else {
        out.province = std::string(*(char **)(service_addr + 0x188),
                                   *(INT64 *)(service_addr + 0x188 + 0x10));
      }
    }

    if (*(INT64 *)(service_addr + 0x1A8) == 0 ||
        *(INT64 *)(service_addr + 0x1A8 + 0x10) == 0) {
      out.city = std::string();
    } else {
      if (*(INT64 *)(service_addr + 0x1A8 + 0x18) == 0xF) {
        out.city = std::string((char *)(service_addr + 0x1A8),
                               *(INT64 *)(service_addr + 0x1A8 + 0x10));
      } else {
        out.city = std::string(*(char **)(service_addr + 0x1A8),
                               *(INT64 *)(service_addr + 0x1A8 + 0x10));
      }
    }

    if (*(INT64 *)(service_addr + 0x1E8) == 0 ||
        *(INT64 *)(service_addr + 0x1E8 + 0x10) == 0) {
      out.name = std::string();
    } else {
      if (*(INT64 *)(service_addr + 0x1E8 + 0x18) == 0xF) {
        out.name = std::string((char *)(service_addr + 0x1E8),
                               *(INT64 *)(service_addr + 0x1E8 + 0x10));
      } else {
        out.name = std::string(*(char **)(service_addr + 0x1E8),
                               *(INT64 *)(service_addr + 0x1E8 + 0x10));
      }
    }

    if (*(INT64 *)(service_addr + 0x450) == 0 ||
        *(INT64 *)(service_addr + 0x450 + 0x10) == 0) {
      out.head_img = std::string();
    } else {
      out.head_img = std::string(*(char **)(service_addr + 0x450),
                                   *(INT64 *)(service_addr + 0x450 + 0x10));
    }

    if (*(INT64 *)(service_addr + 0x7B8) == 0 ||
        *(INT64 *)(service_addr + 0x7B8 + 0x10) == 0) {
      out.public_key = std::string();
    } else {
      out.public_key = std::string(*(char **)(service_addr + 0x7B8),
                                 *(INT64 *)(service_addr + 0x7B8 + 0x10));
    }

    if (*(INT64 *)(service_addr + 0x7D8) == 0 ||
        *(INT64 *)(service_addr + 0x7D8 + 0x10) == 0) {
      out.private_key = std::string();
    } else {
      out.private_key = std::string(*(char **)(service_addr + 0x7D8),
                                 *(INT64 *)(service_addr + 0x7D8 + 0x10));
    }

    if (*(INT64 *)(service_addr + 0x6E0) == 0 ||
        *(INT64 *)(service_addr + 0x6E8) == 0) {
      out.db_key = std::string();
    } else {
      INT64 byte_addr = *(INT64 *)(service_addr + 0x6E0);
      INT64 len = *(INT64 *)(service_addr + 0x6E8);
      out.db_key = base::utils::Bytes2Hex((BYTE *)byte_addr, static_cast<int>(len));
    }

    UINT64 flag = *(UINT64 *)(service_addr + 0x7F8);
    if (flag == 1) {
      prototype::WeChatString current_data_path;
      // _GetCurrentDataPath(get_current_data_path_addr,
      //                     reinterpret_cast<ULONG_PTR>(&current_data_path));
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
  // _GetDataSavePath(get_app_data_save_path_addr,
  //                  reinterpret_cast<ULONG_PTR>(&data_save_path));
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
  INT64 success = -1;
  prototype::WeChatString to_user(wxid);
  prototype::WeChatString image_full_path(image_path);
  UINT64 send_message_mgr_addr = base_addr_ + offset::kGetSendMessageMgr;
  UINT64 send_img_addr = base_addr_ + offset::kSendImageMsg;
  UINT64 new_chat_msg_addr = base_addr_ + offset::kChatMsgInstanceCounter;
  UINT64 free_chat_msg_addr = base_addr_ + offset::kFreeChatMsg;
  func::__NewChatMsg new_chat_msg = (func::__NewChatMsg)new_chat_msg_addr;
  func::__GetSendMessageMgr mgr =
      (func::__GetSendMessageMgr)send_message_mgr_addr;
  func::__SendImageMsg send_img = (func::__SendImageMsg)send_img_addr;
  func::__FreeChatMsg free = (func::__FreeChatMsg)free_chat_msg_addr;

  char chat_msg[0x460] = {0};
  char chat_msg_temp[0x460] = {0};

  UINT64 p_chat_msg_temp = new_chat_msg(reinterpret_cast<UINT64>(&chat_msg_temp));
  UINT64 temp1 =0;
  UINT64 temp2 =0;
  UINT64* flag[10] = {};
  flag[8] = &temp1;
  flag[9] = &temp2;
  flag[1] = reinterpret_cast<UINT64*>(p_chat_msg_temp);
  
  UINT64 p_chat_msg = new_chat_msg(reinterpret_cast<UINT64>(&chat_msg));
  UINT64 send_mgr = mgr();
  send_img(send_mgr, p_chat_msg,
           reinterpret_cast<UINT64>(&to_user),
           reinterpret_cast<UINT64>(&image_full_path),
           reinterpret_cast<UINT64>(&flag));
  free(p_chat_msg);
  free(p_chat_msg_temp);
  success = 1;
  return success;
}

INT64 WechatService::SendFileMsg(const std::wstring& wxid,
                                 const std::wstring& file_path) {
  INT64 success = -1;
  prototype::WeChatString* to_user= (prototype::WeChatString*)HeapAlloc(GetProcessHeap(),0,sizeof(prototype::WeChatString));
  wchar_t * ptr_wxid = (wchar_t*)HeapAlloc(GetProcessHeap(),0,(wxid.length()+1)*2);
  wmemcpy(ptr_wxid,wxid.c_str(),wxid.length()+1);
  to_user->ptr = ptr_wxid;
  to_user->length = static_cast<DWORD>(wxid.length());
  to_user->max_length = static_cast<DWORD>(wxid.length());
  to_user->c_len=0;
  to_user->c_ptr=0;
  prototype::WeChatString* file_full_path= (prototype::WeChatString*)HeapAlloc(GetProcessHeap(),0,sizeof(prototype::WeChatString));
  wchar_t * ptr_path = (wchar_t*)HeapAlloc(GetProcessHeap(),0,(file_path.length()+1)*2);
  wmemcpy(ptr_path,file_path.c_str(),file_path.length()+1);
  file_full_path->ptr = ptr_path;
  file_full_path->length = static_cast<DWORD>(file_path.length());
  file_full_path->max_length = static_cast<DWORD>(file_path.length());
  file_full_path->c_len = 0;
  file_full_path->c_ptr = 0;

  UINT64 get_app_msg_mgr_addr = base_addr_ + offset::kGetAppMsgMgr;
  UINT64 send_file_addr = base_addr_ + offset::kSendFileMsg;
  UINT64 new_chat_msg_addr = base_addr_ + offset::kChatMsgInstanceCounter;
  UINT64 free_chat_msg_addr = base_addr_ + offset::kFreeChatMsg;
  func::__NewChatMsg new_chat_msg = (func::__NewChatMsg)new_chat_msg_addr;
  func::__GetAppMsgMgr get_app_mgr =
      (func::__GetAppMsgMgr)get_app_msg_mgr_addr;
  func::__SendFile send_file = (func::__SendFile)send_file_addr;
  func::__FreeChatMsg free = (func::__FreeChatMsg)free_chat_msg_addr;

 
  char* chat_msg= (char*)HeapAlloc(GetProcessHeap(),0,0x460);

  UINT64* temp1 = (UINT64*)HeapAlloc(GetProcessHeap(),0,sizeof(UINT64)*4);
  UINT64* temp2 = (UINT64*)HeapAlloc(GetProcessHeap(),0,sizeof(UINT64)*4);
  UINT64* temp3 = (UINT64*)HeapAlloc(GetProcessHeap(),0,sizeof(UINT64)*4);
  UINT64* temp4 = (UINT64*)HeapAlloc(GetProcessHeap(),0,sizeof(UINT64)*4);
  ZeroMemory(temp1,sizeof(UINT64)*4);
  ZeroMemory(temp2,sizeof(UINT64)*4);
  ZeroMemory(temp3,sizeof(UINT64)*4);
  ZeroMemory(temp4,sizeof(UINT64)*4);
  *temp4=0x1F;
  UINT64 app_mgr = get_app_mgr();
  send_file(app_mgr, reinterpret_cast<UINT64>(chat_msg),
            reinterpret_cast<UINT64>(to_user),
            reinterpret_cast<UINT64>(file_full_path), 1,
            reinterpret_cast<UINT64>(temp1), 0, reinterpret_cast<UINT64>(temp2),
            0, reinterpret_cast<UINT64>(temp3), 0, 0);
  free(reinterpret_cast<UINT64>(chat_msg));  
  HeapFree(GetProcessHeap(),0,to_user);
  HeapFree(GetProcessHeap(),0,file_full_path);
  HeapFree(GetProcessHeap(),0,temp1);
  HeapFree(GetProcessHeap(),0,temp2);
  HeapFree(GetProcessHeap(),0,temp3);
  HeapFree(GetProcessHeap(),0,temp4);
  success = 1;
  return success;
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
  INT64 success = -1;
  UINT64 forward_addr = base_addr_ + offset::kForwardMsg;
  func::__ForwardMsg forward_msg = (func::__ForwardMsg)forward_addr;
  INT64 index = 0;
  INT64 local_id = wxhelper::DB::GetInstance().GetLocalIdByMsgId(msg_id, index);
  if (local_id <= 0 || index >> 32 == 0) {
    success = -2;
    return success;
  }
  LARGE_INTEGER l;
  l.HighPart = index >> 32;
  l.LowPart = (DWORD)local_id;
  prototype::WeChatString *recv = BuildWechatString(wxid);
  success = forward_msg(reinterpret_cast<UINT64>(recv), l.QuadPart, 0x4, 0x0);
  return success;
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
  INT64 success = -1;
  std::vector<prototype::WeChatString> wxid_list;
  common::VectorInner *list = (common::VectorInner *)&wxid_list;
  std::wstring at_msg = L"";
  int number = 0;
  for (unsigned int i = 0; i < wxids.size(); i++) {
    std::wstring nickname;
    std::wstring at_all = L"notify@all";
    if (at_all.compare(wxids[i]) == 0) {
      nickname = L"\u6240\u6709\u4eba";
    } else {
      nickname = GetContactOrChatRoomNickname(wxids[i]);
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
    return success;
  }
  at_msg += msg;

  INT64 head = (INT64)&list->start;
  prototype::WeChatString to_user(room_id);
  prototype::WeChatString text_msg(at_msg);
  UINT64 send_message_mgr_addr = base_addr_ + offset::kGetSendMessageMgr;
  UINT64 send_text_msg_addr = base_addr_ + offset::kSendTextMsg;
  UINT64 free_chat_msg_addr = base_addr_ + offset::kFreeChatMsg;
  char chat_msg[0x460] = {0};
  func::__GetSendMessageMgr mgr= (func::__GetSendMessageMgr)send_message_mgr_addr;
  func::__SendTextMsg send= (func::__SendTextMsg)send_text_msg_addr;
  func::__FreeChatMsg free= (func::__FreeChatMsg)free_chat_msg_addr;
  mgr();
  success = send(reinterpret_cast<UINT64>(&chat_msg),
                 reinterpret_cast<UINT64>(&to_user),
                 reinterpret_cast<UINT64>(&text_msg), head, 1, 1, 0, 0);
  free(reinterpret_cast<UINT64>(&chat_msg));
  return success;
}

std::wstring WechatService::GetContactOrChatRoomNickname(
    const std::wstring& wxid) {
  prototype::WeChatString to_user(wxid);
  UINT64 get_contact_mgr_addr = base_addr_ + offset::kGetContactMgr;
  UINT64 new_contact_addr = base_addr_ + offset::kNewContact;
  UINT64 get_contact_addr = base_addr_ + offset::kGetContact;
  UINT64 free_contact_addr = base_addr_ + offset::kFreeContact;
  func::__GetContactMgr get_contact_mgr =
      (func::__GetContactMgr)get_contact_mgr_addr;
  func::__GetContact get_contact = (func::__GetContact)get_contact_addr;
  func::__NewContact new_contact = (func::__NewContact)new_contact_addr;
  func::__FreeContact free_contact = (func::__FreeContact)free_contact_addr;
  char buff[0x6A9] = {0};
  UINT64 contact = new_contact(reinterpret_cast<UINT64>(&buff));
  UINT64 mgr = get_contact_mgr();
  INT64 success = get_contact(mgr, reinterpret_cast<UINT64>(&to_user), contact);
  if (success == 1) {
    std::wstring nickname = wxutils::ReadWstring(contact + 0xA0);
    free_contact(contact);
    return nickname;
  } else {
    free_contact(contact);
    return L"";
  }
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

INT64 WechatService::LockWeChat() {
  INT64 success = -1;
  UINT64 lock_mgr_addr = base_addr_ + offset::kGetLockWechatMgr;
  UINT64 request_lock_addr = base_addr_ + offset::kRequestLockWechat;
  func::__GetLockWechatMgr GetLockMgr = (func::__GetLockWechatMgr)lock_mgr_addr;
  func::__RequestLockWechat request_lock =
      (func::__RequestLockWechat)request_lock_addr;
  UINT64 mgr = GetLockMgr();
  success = request_lock(mgr);
  return success;
}

INT64 WechatService::UnLockWeChat() {
  INT64 success = -1;
  UINT64 lock_mgr_addr = base_addr_ + offset::kGetLockWechatMgr;
  UINT64 request_unlock_addr = base_addr_ + offset::kRequestUnLockWechat;
  func::__GetLockWechatMgr GetLockMgr = (func::__GetLockWechatMgr)lock_mgr_addr;
  func::__RequestUnLockWechat request_unlock =
      (func::__RequestUnLockWechat)request_unlock_addr;
  UINT64 mgr = GetLockMgr();
  success = request_unlock(mgr);
  return success;
}

INT64 WechatService::EnterWeChat() {
  INT64 success = -1;
  UINT64 click_cb_addr = base_addr_ + offset::kOnLoginBtnClick;
  func::__OnLoginBtnClick cb = (func::__OnLoginBtnClick)click_cb_addr;
  auto vec =
      base::memory::ScanAndMatchValue(base_addr_ + 0x34e0c18, 0x1000, 0x8);
  for (int i = 0; i < vec.size(); i++) {
    INT64 ptr = vec.at(i);
    if (*(INT64 *)ptr == base_addr_ + 0x34e0c18) {
      INT64 login_wnd = ptr;
      success = cb(ptr);
      break;
    }
  }
  return success;
}

INT64 WechatService::SendMultiAtText(
    const std::wstring &room_id,
    const std::vector<std::pair<std::wstring, std::wstring>> &at) {
  INT64 success = -1;
  std::vector<prototype::WeChatString> wxid_list;
  common::VectorInner *list = (common::VectorInner *)&wxid_list;
  std::wstring at_msg = L"";
  int number = 0;
  for (unsigned int i = 0; i < at.size(); i++) {
    std::wstring nickname;
    std::wstring at_all = L"notify@all";
    if (at_all.compare(at[i].first) == 0) {
      nickname = L"\u6240\u6709\u4eba";
    } else {
      nickname = GetContactOrChatRoomNickname(at[i].first);
    }
    if (nickname.length() == 0) {
      continue;
    }
    prototype::WeChatString id(at[i].first);
    wxid_list.push_back(id);
    at_msg = at_msg + L"@" + nickname + L" "+ at[i].second + L" ";
    number++;
  }
  if (number < 1) {
    return success;
  }
  INT64 head = (INT64)&list->start;
  prototype::WeChatString to_user(room_id);
  prototype::WeChatString text_msg(at_msg);
  UINT64 send_message_mgr_addr = base_addr_ + offset::kGetSendMessageMgr;
  UINT64 send_text_msg_addr = base_addr_ + offset::kSendTextMsg;
  UINT64 free_chat_msg_addr = base_addr_ + offset::kFreeChatMsg;
  char chat_msg[0x460] = {0};
  func::__GetSendMessageMgr mgr =
      (func::__GetSendMessageMgr)send_message_mgr_addr;
  func::__SendTextMsg send = (func::__SendTextMsg)send_text_msg_addr;
  func::__FreeChatMsg free = (func::__FreeChatMsg)free_chat_msg_addr;
  mgr();
  success = send(reinterpret_cast<UINT64>(&chat_msg),
                 reinterpret_cast<UINT64>(&to_user),
                 reinterpret_cast<UINT64>(&text_msg), head, 1, 1, 0, 0);
  free(reinterpret_cast<UINT64>(&chat_msg));
  return success;
}

std::string WechatService::GetLoginUrl(){
  INT64 success = -1;
  UINT64 login_mgr_addr = base_addr_ + offset::kGetQRCodeLoginMgr;
  func::__GetQRCodeLoginMgr get = (func::__GetQRCodeLoginMgr)login_mgr_addr;
  UINT64 addr = get();
  std::string login_url = wxutils::ReadWeChatStr(addr + 0x68);
  return "http://weixin.qq.com/x/" + login_url;
}

void WechatService::SetBaseAddr(UINT64 addr) { base_addr_ = addr; }

void WechatService::SetJsApiAddr(UINT64 addr) { js_api_addr_ = addr; }


}  // namespace wxhelper