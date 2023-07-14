#include "pch.h"
#include "manager.h"

#include "export.h"
#include "wechat_function.h"

namespace offset = wxhelper::V3_9_5_81::offset;
namespace prototype = wxhelper::V3_9_5_81::prototype;
namespace func = wxhelper::V3_9_5_81::function;


namespace wxhelper {

prototype::WeChatString * BuildWechatString(const std::wstring &ws){
  prototype::WeChatString *p = Utils::WxHeapAlloc<prototype::WeChatString>(
      sizeof(prototype::WeChatString));
  wchar_t *p_chat_room_id = Utils::WxHeapAlloc<wchar_t>((ws.size() + 1) * 2);
  wmemcpy(p_chat_room_id, ws.c_str(), ws.size() + 1);
  p->ptr = p_chat_room_id;
  p->length = static_cast<DWORD>(ws.size());
  p->max_length = static_cast<DWORD>(ws.size());
  p->c_len = 0;
  p->c_ptr = 0;
  return p;
}

Manager::Manager(UINT64 base) : base_addr_(base) {}
Manager::~Manager() {}
INT64 Manager::CheckLogin() {
  INT64 success = -1;
  UINT64 accout_service_addr = base_addr_ + offset::kGetAccountServiceMgr;
  func::__GetAccountService GetSevice = (func::__GetAccountService)accout_service_addr;
  // UINT64 service_addr = _GetAccountService(accout_service_addr);
  UINT64 service_addr = GetSevice();
  if (service_addr) {
    success = *(UINT64 *)(service_addr + 0x7F8);
  }
  return success;
}

INT64 Manager::GetSelfInfo(common::SelfInfoInner &out) {
  INT64 success = -1;
  UINT64 accout_service_addr = base_addr_ + offset::kGetAccountServiceMgr;
  UINT64 get_app_data_save_path_addr = base_addr_ + offset::kGetAppDataSavePath;
  UINT64 get_current_data_path_addr = base_addr_ + offset::kGetCurrentDataPath;
  // UINT64 service_addr = _GetAccountService(accout_service_addr);
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

    if (*(INT64 *)(service_addr + 0x6E0) == 0 ||
        *(INT64 *)(service_addr + 0x6E8) == 0) {
      out.db_key = std::string();
    } else {
      INT64 byte_addr = *(INT64 *)(service_addr + 0x6E0);
      INT64 len = *(INT64 *)(service_addr + 0x6E8);
      out.db_key = Utils::Bytes2Hex((BYTE *)byte_addr, static_cast<int>(len));
    }

    UINT64 flag = *(UINT64 *)(service_addr + 0x7F8);
    if (flag == 1) {
      prototype::WeChatString current_data_path;
      // _GetCurrentDataPath(get_current_data_path_addr,
      //                     reinterpret_cast<ULONG_PTR>(&current_data_path));
      GetCurrentDataPath(reinterpret_cast<ULONG_PTR>(&current_data_path));
      if (current_data_path.ptr) {
        out.current_data_path = Utils::WstringToUTF8(
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
    out.data_save_path = Utils::WstringToUTF8(
        std::wstring(data_save_path.ptr, data_save_path.length));
  } else {
    out.data_save_path = std::string();
  }

  success = 1;
  return success;
}



INT64 Manager::SendTextMsg(const std::wstring& wxid, const std::wstring& msg){
  INT64 success = -1;
  prototype::WeChatString to_user(wxid);
  prototype::WeChatString text_msg(msg);
  UINT64 send_message_mgr_addr = base_addr_ + offset::kGetSendMessageMgr;
  UINT64 send_text_msg_addr = base_addr_ + offset::kSendTextMsg;
  UINT64 free_chat_msg_addr = base_addr_ + offset::kFreeChatMsg;
  char chat_msg[0x460] = {0};
  UINT64 temp[3] ={0};
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

INT64 Manager::SendImageMsg(const std::wstring& wxid, const std::wstring& image_path){
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


INT64 Manager::SendFileMsg(const std::wstring& wxid, const std::wstring& file_path){
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

INT64 Manager::GetContacts(std::vector<common::ContactInner> &vec) {
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
    temp.wxid = Utils::ReadWstringThenConvert(start + 0x10);
    temp.custom_account = Utils::ReadWstringThenConvert(start + 0x30);
    temp.encrypt_name = Utils::ReadWstringThenConvert(start + 0x50);
    temp.nickname = Utils::ReadWstringThenConvert(start + 0xA0);
    temp.pinyin = Utils::ReadWstringThenConvert(start + 0x108);
    temp.pinyin_all = Utils::ReadWstringThenConvert(start + 0x128);
    temp.verify_flag = *(DWORD *)(start + 0x70);
    temp.type = *(DWORD *)(start + 0x74);
    temp.reserved1 = *(DWORD *)(start + 0x1F0);
    temp.reserved2 = *(DWORD *)(start + 0x1F4);
    vec.push_back(temp);
    start += 0x698;
  }
  return success;
}

INT64 Manager::GetChatRoomDetailInfo(const std::wstring &room_id,
                                     common::ChatRoomInfoInner &room_info) {
  INT64 success = -1;
  UINT64 get_chat_room_mgr_addr = base_addr_ + offset::kChatRoomMgr;
  UINT64 get_chat_room_detail_addr =
      base_addr_ + offset::kGetChatRoomDetailInfo;
  UINT64 new_chat_room_info_addr = base_addr_ + offset::kNewChatRoomInfo;
  UINT64 free_chat_room_info_addr = base_addr_ + offset::kFreeChatRoomInfo;
  func::__GetChatRoomMgr get_chat_room_mgr =
      (func::__GetChatRoomMgr)get_chat_room_mgr_addr;
  func::__GetChatRoomDetailInfo get_chat_room_detail =
      (func::__GetChatRoomDetailInfo)get_chat_room_detail_addr;
  func::__NewChatRoomInfo new_chat_room_info =
      (func::__NewChatRoomInfo)new_chat_room_info_addr;
  func::__FreeChatRoomInfo free_chat_room_info =
      (func::__FreeChatRoomInfo)free_chat_room_info_addr;

  prototype::WeChatString chat_room_id(room_id);
  char chat_room_info[0x148] = {0};
  UINT64 p_chat_room_info =
      new_chat_room_info(reinterpret_cast<UINT64>(&chat_room_info));
  UINT64 mgr = get_chat_room_mgr();
  success = get_chat_room_detail(mgr, reinterpret_cast<UINT64>(&chat_room_id),
                                 p_chat_room_info, 1);
  room_info.admin = Utils::ReadWstringThenConvert(p_chat_room_info + 0x48);
  room_info.chat_room_id =
      Utils::ReadWstringThenConvert(p_chat_room_info + 0x8);
  room_info.notice = Utils::ReadWstringThenConvert(p_chat_room_info + 0x28);
  room_info.xml = Utils::ReadWstringThenConvert(p_chat_room_info + 0x78);
  free_chat_room_info(p_chat_room_info);
  return success;
}

INT64 Manager::AddMemberToChatRoom(const std::wstring &room_id,
                                   const std::vector<std::wstring> &members) {
  INT64 success = -1;
  UINT64 get_chat_room_mgr_addr = base_addr_ + offset::kChatRoomMgr;
  UINT64 add_members_addr = base_addr_ + offset::kDoAddMemberToChatRoom;
  func::__GetChatRoomMgr get_chat_room_mgr =
      (func::__GetChatRoomMgr)get_chat_room_mgr_addr;
  func::__DoAddMemberToChatRoom add_members =
      (func::__DoAddMemberToChatRoom)add_members_addr;

  prototype::WeChatString *chat_room_id = (prototype::WeChatString *)HeapAlloc(
      GetProcessHeap(), 0, sizeof(prototype::WeChatString));
  wchar_t *p_chat_room_id =
      (wchar_t *)HeapAlloc(GetProcessHeap(), 0, (room_id.size() + 1) * 2);
  wmemcpy(p_chat_room_id, room_id.c_str(), room_id.size() + 1);
  chat_room_id->ptr = p_chat_room_id;
  chat_room_id->length = static_cast<DWORD>(room_id.size());
  chat_room_id->max_length = static_cast<DWORD>(room_id.size());
  chat_room_id->c_len = 0;
  chat_room_id->c_ptr = 0;

  std::vector<prototype::WeChatString> member_list;
  UINT64 temp[2] = {0};
  common::VectorInner *list = (common::VectorInner *)&member_list;
  INT64 members_ptr = (INT64)&list->start;
  for (int i = 0; i < members.size(); i++) {
    prototype::WeChatString member(members[i]);
    member_list.push_back(member);
  }
  UINT64 mgr = get_chat_room_mgr();
  success =
      add_members(mgr, members_ptr, reinterpret_cast<UINT64>(chat_room_id),
                  reinterpret_cast<UINT64>(&temp));
  return success;
}



INT64 Manager::ModChatRoomMemberNickName(const std::wstring &room_id,
                                         const std::wstring &wxid,
                                         const std::wstring &nickname) {
  INT64 success = -1;
  UINT64 mod_addr = base_addr_ + offset::kDoModChatRoomMemberNickName;
  func::__DoModChatRoomMemberNickName modify =
      (func::__DoModChatRoomMemberNickName)mod_addr;
  const wchar_t *p = room_id.c_str();
  prototype::WeChatString *chat_room_id = BuildWechatString(room_id);
  prototype::WeChatString *self_id = BuildWechatString(wxid);
  prototype::WeChatString *name = BuildWechatString(nickname);
  success = modify(
      reinterpret_cast<UINT64>(p), reinterpret_cast<UINT64>(chat_room_id),
      reinterpret_cast<UINT64>(self_id), reinterpret_cast<UINT64>(name));
  return success;
}

INT64 Manager::DelMemberFromChatRoom(const std::wstring &room_id,
                                     const std::vector<std::wstring> &members) {
  INT64 success = -1;
  UINT64 get_chat_room_mgr_addr = base_addr_ + offset::kChatRoomMgr;
  UINT64 del_members_addr = base_addr_ + offset::kDelMemberFromChatRoom;
  func::__GetChatRoomMgr get_chat_room_mgr =
      (func::__GetChatRoomMgr)get_chat_room_mgr_addr;
  func::__DoDelMemberFromChatRoom del_members =
      (func::__DoDelMemberFromChatRoom)del_members_addr;

  prototype::WeChatString *chat_room_id = BuildWechatString(room_id);
  std::vector<prototype::WeChatString> member_list;
  UINT64 temp[2] = {0};
  common::VectorInner *list = (common::VectorInner *)&member_list;
  INT64 members_ptr = (INT64)&list->start;
  for (int i = 0; i < members.size(); i++) {
    prototype::WeChatString member(members[i]);
    member_list.push_back(member);
  }
  UINT64 mgr = get_chat_room_mgr();
  success =
      del_members(mgr, members_ptr, reinterpret_cast<UINT64>(chat_room_id));
  return success;
}

INT64 Manager::GetMemberFromChatRoom(const std::wstring &room_id,
                                     common::ChatRoomMemberInner &member) {
  INT64 success = -1;
  UINT64 get_chat_room_mgr_addr = base_addr_ + offset::kChatRoomMgr;
  UINT64 get_members_addr = base_addr_ + offset::kGetMemberFromChatRoom;
  UINT64 new_chat_room_addr = base_addr_ + offset::kNewChatRoom;
  UINT64 free_chat_room_addr = base_addr_ + offset::kFreeChatRoom;
  func::__GetChatRoomMgr get_chat_room_mgr =
      (func::__GetChatRoomMgr)get_chat_room_mgr_addr;
  func::__GetMemberFromChatRoom get_members =
      (func::__GetMemberFromChatRoom)get_members_addr;
  func::__NewChatRoom new_chat_room = (func::__NewChatRoom)new_chat_room_addr;
  func::__FreeChatRoom free_chat_room =
      (func::__FreeChatRoom)free_chat_room_addr;

  prototype::WeChatString chat_room_id(room_id);
  char chat_room_info[0x2E0] = {0};
  UINT64 addr = reinterpret_cast<UINT64>(&chat_room_info);
  new_chat_room(addr);

  UINT64 mgr = get_chat_room_mgr();
  success = get_members(mgr, reinterpret_cast<UINT64>(&chat_room_id), addr);
  member.chat_room_id = Utils::ReadWstringThenConvert(addr + 0x10);        
  member.admin = Utils::ReadWstringThenConvert(addr + 0x78);        
  member.member_nickname = Utils::ReadWstringThenConvert(addr + 0x50);        
  member.admin_nickname = Utils::ReadWstringThenConvert(addr + 0xA0);        
  member.member = Utils::ReadWeChatStr(addr + 0x30);   
  free_chat_room(addr);
  return success;     
}
} // namespace wxhelper