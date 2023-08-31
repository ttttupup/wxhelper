#include "pch.h"
#include "manager.h"

#include "export.h"
#include "wechat_function.h"
#include "db.h"
#include "lz4.h"
#include "base64.h"
#include "tinyxml2.h"

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

prototype::WeChatStr * BuildWechatStr(const std::string &str){
  prototype::WeChatStr *p = Utils::WxHeapAlloc<prototype::WeChatStr>(
      sizeof(prototype::WeChatStr));
  char *p_chat_room_id = Utils::WxHeapAlloc<char>(str.size() + 1);
  memcpy(p_chat_room_id, str.c_str(), str.size() + 1);
  p->ptr = p_chat_room_id;
  p->len = static_cast<DWORD>(str.size());
  p->maxlen = static_cast<DWORD>(str.size());
  p->buf = NULL;
  return p;
}

Manager::Manager(UINT64 base) : base_addr_(base),js_api_addr_(0) {}
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
INT64 Manager::SetTopMsg(ULONG64 msg_id) {
  INT64 success = -1;
  UINT64 top_addr = base_addr_ + offset::kTopMsg;
  func::__DoTopMsg top_msg = (func::__DoTopMsg)top_addr;
  INT64 index = 0;
  INT64 local_id = DB::GetInstance().GetLocalIdByMsgId(msg_id, index);
  if (local_id <= 0 || index >> 32 == 0) {
    success = -2;
    return success;
  }
  LARGE_INTEGER l;
  l.HighPart = index >> 32;
  l.LowPart = (DWORD)local_id;
  UINT64 ptr = reinterpret_cast<UINT64>(&l);
  success = top_msg(ptr, 1);

  return success;
}

INT64 Manager::RemoveTopMsg(const std::wstring &room_id, ULONG64 msg_id) {
  INT64 success = -1;
  UINT64 remove_addr = base_addr_ + offset::kRemoveTopMsg;
  func::__RemoveTopMsg remove_top_msg = (func::__RemoveTopMsg)remove_addr;
  prototype::WeChatString *chat_room_id = BuildWechatString(room_id);
  const wchar_t *w_room = room_id.c_str();
  success = remove_top_msg(reinterpret_cast<UINT64>(w_room), msg_id,
                           reinterpret_cast<UINT64>(chat_room_id));
  return success;
}

INT64 Manager::InviteMemberToChatRoom(const std::wstring &room_id,
                                      const std::vector<std::wstring> &wxids) {
  INT64 success = -1;
  UINT64 invite_addr = base_addr_ + offset::kInviteMember;
  func::__InviteMemberToChatRoom invite =
      (func::__InviteMemberToChatRoom)invite_addr;
  const wchar_t *w_room = room_id.c_str();
  prototype::WeChatString *chat_room_id = BuildWechatString(room_id);
  std::vector<prototype::WeChatString> wxid_list;
  common::VectorInner *list = (common::VectorInner *)&wxid_list;
  INT64 head = (INT64)&list->start;
  for (int i = 0; i < wxids.size(); i++) {
    prototype::WeChatString id(wxids[i]);
    wxid_list.push_back(id);
  }
  UINT64 temp[2] = {0};
  success = invite(reinterpret_cast<UINT64>(w_room), head,
                   reinterpret_cast<UINT64>(chat_room_id),
                   reinterpret_cast<UINT64>(&temp));
  return success;
}

INT64 Manager::CreateChatRoom(const std::vector<std::wstring>& wxids){
  INT64 success = -1;
  UINT64 get_chat_room_mgr_addr = base_addr_ + offset::kChatRoomMgr;
  UINT64 create_chat_room_addr = base_addr_ + offset::kCreateChatRoom;
  func::__GetChatRoomMgr get_chat_room_mgr =
      (func::__GetChatRoomMgr)get_chat_room_mgr_addr;
  func::__CreateChatRoom create_chat_room =
      (func::__CreateChatRoom)create_chat_room_addr;
  std::vector<prototype::WeChatString> wxid_list;
  common::VectorInner *list = (common::VectorInner *)&wxid_list;
  INT64 head = (INT64)&list->start;
  for (int i = 0; i < wxids.size(); i++) {
    prototype::WeChatString id(wxids[i]);
    wxid_list.push_back(id);
  }
  INT64 end = list->end;
  UINT64 mgr = get_chat_room_mgr();
  success = create_chat_room(mgr, head, end);
  return success;
}
INT64 Manager::QuitChatRoom(const std::wstring &room_id) {
  INT64 success = -1;
  UINT64 get_chat_room_mgr_addr = base_addr_ + offset::kChatRoomMgr;
  UINT64 quit_chat_room_addr = base_addr_ + offset::kQuitChatRoom;
  func::__GetChatRoomMgr get_chat_room_mgr =
      (func::__GetChatRoomMgr)get_chat_room_mgr_addr;
  func::__QuitChatRoom quit_chat_room =
      (func::__QuitChatRoom)quit_chat_room_addr;
  UINT64 mgr = get_chat_room_mgr();
  prototype::WeChatString chat_room_id(room_id);
  success = quit_chat_room(mgr, reinterpret_cast<UINT64>(&chat_room_id), 0);
  return success;
}
INT64 Manager::ForwardMsg(UINT64 msg_id, const std::wstring &wxid) {
  INT64 success = -1;
  UINT64 forward_addr = base_addr_ + offset::kForwardMsg;
  func::__ForwardMsg forward_msg = (func::__ForwardMsg)forward_addr;
  INT64 index = 0;
  INT64 local_id = DB::GetInstance().GetLocalIdByMsgId(msg_id, index);
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

INT64 Manager::GetSNSFirstPage() {
  INT64 success = -1;
  UINT64 sns_data_mgr_addr = base_addr_ + offset::kSNSDataMgr;
  UINT64 sns_first_page_addr = base_addr_ + offset::kSNSGetFirstPage;
  func::__GetSNSDataMgr sns_data_mgr = (func::__GetSNSDataMgr)sns_data_mgr_addr;
  func::__GetSNSFirstPage sns_first_page =
      (func::__GetSNSFirstPage)sns_first_page_addr;
  UINT64 mgr = sns_data_mgr();
  INT64 buff[16] = {0};
  success = sns_first_page(mgr, reinterpret_cast<UINT64>(&buff), 1);
  return success;
}

INT64 Manager::GetSNSNextPage(UINT64 sns_id) {
  INT64 success = -1;
  UINT64 time_line_mgr_addr = base_addr_ + offset::kSNSTimeLineMgr;
  UINT64 sns_next_page_addr = base_addr_ + offset::kSNSGetNextPageScene;
  func::__GetSnsTimeLineMgr time_line_mgr =
      (func::__GetSnsTimeLineMgr)time_line_mgr_addr;
  func::__GetSNSNextPageScene sns_next_page =
      (func::__GetSNSNextPageScene)sns_next_page_addr;
  UINT64 mgr = time_line_mgr();
  success = sns_next_page(mgr, sns_id);
  return success;
}

INT64 Manager::AddFavFromMsg(UINT64 msg_id) {
  INT64 success = -1;
  UINT64 get_chat_mgr_addr = base_addr_ + offset::kGetChatMgr;
  UINT64 get_by_local_id_addr = base_addr_ + offset::kGetMgrByPrefixLocalId;
  UINT64 add_fav_addr = base_addr_ + offset::kAddFavFromMsg;
  UINT64 get_favorite_mgr_addr = base_addr_ + offset::kGetFavoriteMgr;
  UINT64 free_chat_msg_addr = base_addr_ + offset::kFreeChatMsg;
  func::__GetMgrByPrefixLocalId get_by_local_id = (func::__GetMgrByPrefixLocalId)get_by_local_id_addr;
  UINT64 new_chat_msg_addr = base_addr_ + offset::kChatMsgInstanceCounter;

  func::__AddFavFromMsg add_fav = (func::__AddFavFromMsg)add_fav_addr;
  func::__GetChatMgr get_chat_mgr = (func::__GetChatMgr)get_chat_mgr_addr;
  func::__GetFavoriteMgr get_favorite_mgr = (func::__GetFavoriteMgr)get_favorite_mgr_addr;
  func::__FreeChatMsg free_chat_msg = (func::__FreeChatMsg)free_chat_msg_addr;
  func::__NewChatMsg new_chat_msg = (func::__NewChatMsg)new_chat_msg_addr;

  INT64 index = 0;
  INT64 local_id = DB::GetInstance().GetLocalIdByMsgId(msg_id, index);
  if (local_id <= 0 || index >> 32 == 0) {
    success = -2;
    return success;
  }
  char chat_msg[0x460]= {0};
  LARGE_INTEGER l;
  l.HighPart = index >> 32;
  l.LowPart = (DWORD)local_id;
  UINT64 p_chat_msg = new_chat_msg(reinterpret_cast<UINT64>(&chat_msg));

  get_chat_mgr();
  get_by_local_id(l.QuadPart,p_chat_msg);
  UINT64 mgr = get_favorite_mgr();
  success = add_fav(mgr,p_chat_msg);
  free_chat_msg(p_chat_msg);
  return success;
}

INT64 Manager::AddFavFromImage(const std::wstring &wxid,
                               const std::wstring &image_path) {
  INT64 success = -1;
  UINT64 get_favorite_mgr_addr = base_addr_ + offset::kGetFavoriteMgr;
  UINT64 add_fav_from_image_addr = base_addr_ + offset::kAddFavFromImage;
  prototype::WeChatString *send_id = BuildWechatString(wxid);
  prototype::WeChatString *path = BuildWechatString(image_path);
  func::__GetFavoriteMgr get_favorite_mgr =
      (func::__GetFavoriteMgr)get_favorite_mgr_addr;
  func::__AddFavFromImage add_fav_from_image =
      (func::__AddFavFromImage)add_fav_from_image_addr;
  UINT64 mgr = get_favorite_mgr();
  success = add_fav_from_image(mgr, reinterpret_cast<UINT64>(path),
                               reinterpret_cast<UINT64>(send_id));
  return success;
}

INT64 Manager::SendAtText(const std::wstring &room_id,
                          const std::vector<std::wstring> &wxids,
                          const std::wstring &msg) {
  INT64 success = -1;
  std::vector<prototype::WeChatString> wxid_list;
  common::VectorInner *list = (common::VectorInner *)&wxid_list;
  std::wstring at_msg = L"";
  int number = 0;
  for (unsigned int i = 0; i < wxids.size(); i++) {
    std::wstring nickname;
    std::wstring at_all = L"notify@all";
    if (at_all.compare(wxids[i]) == 0 ) {
      nickname = L"所有人";
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
  func::__GetSendMessageMgr mgr;
  mgr = (func::__GetSendMessageMgr)send_message_mgr_addr;
  func::__SendTextMsg send;
  send = (func::__SendTextMsg)send_text_msg_addr;
  func::__FreeChatMsg free;
  free = (func::__FreeChatMsg)free_chat_msg_addr;
  mgr();
  success = send(reinterpret_cast<UINT64>(&chat_msg),
                 reinterpret_cast<UINT64>(&to_user),
                 reinterpret_cast<UINT64>(&text_msg), head, 1, 1, 0, 0);
  free(reinterpret_cast<UINT64>(&chat_msg));
  return success;
}

std::wstring Manager::GetContactOrChatRoomNickname(const std::wstring &wxid) {
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
    std::wstring nickname = Utils::ReadWstring(contact + 0xA0);
    free_contact(contact);
    return nickname;
  } else {
    free_contact(contact);
    return L"";
  }
}

INT64 Manager::GetContactByWxid(const std::wstring &wxid,
                                common::ContactProfileInner &profile) {
  INT64 success = -1;
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
  success = get_contact(mgr, reinterpret_cast<UINT64>(&to_user), contact);
   profile.wxid =  Utils::ReadWstringThenConvert(contact + 0x10);
  profile.account = Utils::ReadWstringThenConvert(contact + 0x30);
  profile.v3 = Utils::ReadWstringThenConvert(contact + 0x50);
  profile.nickname =  Utils::ReadWstringThenConvert(contact +  0xA0);
  profile.head_image = Utils::ReadWstringThenConvert(contact +  0x188);
  free_contact(contact);
  return success;
}

INT64 Manager::DoDownloadTask(UINT64 msg_id) {
  INT64 success = -1;
  UINT64 get_by_local_id_addr = base_addr_ + offset::kGetMgrByPrefixLocalId;
  func::__GetMgrByPrefixLocalId get_by_local_id =
      (func::__GetMgrByPrefixLocalId)get_by_local_id_addr;

  UINT64 get_chat_mgr_addr = base_addr_ + offset::kGetChatMgr;
  func::__GetChatMgr get_chat_mgr = (func::__GetChatMgr)get_chat_mgr_addr;

  UINT64 free_chat_msg_addr = base_addr_ + offset::kFreeChatMsg;
  func::__FreeChatMsg free_chat_msg = (func::__FreeChatMsg)free_chat_msg_addr;

  UINT64 new_chat_msg_addr = base_addr_ + offset::kChatMsgInstanceCounter;
  func::__NewChatMsg new_chat_msg = (func::__NewChatMsg)new_chat_msg_addr;

  UINT64 get_current_data_path_addr = base_addr_ + offset::kGetCurrentDataPath;
  func::__GetCurrentDataPath GetCurrentDataPath =
      (func::__GetCurrentDataPath)get_current_data_path_addr;

  UINT64 new_app_msg_info_addr = base_addr_ + offset::kNewAppMsgInfo;
  func::__NewAppMsgInfo new_app_msg_info =
      (func::__NewAppMsgInfo)new_app_msg_info_addr;

  UINT64 free_app_msg_info_addr = base_addr_ + offset::kFreeAppMsgInfo;
  func::__FreeAppMsgInfo free_app_msg_info =
      (func::__NewAppMsgInfo)free_app_msg_info_addr;

  UINT64 xml_to_app_info_addr = base_addr_ + offset::kParseAppMsgXml;
  func::__ParseAppMsgXml xml_to_app_info =
      (func::__ParseAppMsgXml)xml_to_app_info_addr;

  UINT64 get_pre_download_mgr_addr = base_addr_ + offset::kGetPreDownLoadMgr;
  func::__GetPreDownLoadMgr get_pre_download_mgr =
      (func::__GetPreDownLoadMgr)get_pre_download_mgr_addr;

  UINT64 push_attach_task_addr = base_addr_ + offset::kPushAttachTask;
  func::__PushAttachTask push_attach_task =
      (func::__PushAttachTask)push_attach_task_addr;

  INT64 index = 0;
  INT64 local_id = DB::GetInstance().GetLocalIdByMsgId(msg_id, index);
  if (local_id <= 0 || index >> 32 == 0) {
    success = -2;
    return success;
  }
  char *chat_msg = Utils::WxHeapAlloc<char>(0x460);
  LARGE_INTEGER l;
  l.HighPart = index >> 32;
  l.LowPart = (DWORD)local_id;
  UINT64 p_chat_msg = new_chat_msg(reinterpret_cast<UINT64>(chat_msg));

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

  if (!Utils::FindOrCreateDirectoryW(save_path.c_str())) {
    return -3;
  }
  INT64 type = *(INT64 *)(chat_msg + 0x38);
  wchar_t *content = *(wchar_t **)(chat_msg + 0x88);
  DWORD len = *(DWORD *)(chat_msg + 0x94);
  std::wstring tmp_content(content, len);
  prototype::WeChatString *we_content = BuildWechatString(tmp_content);

  switch (type) {
    case 0x3: {
      save_path += L"\\image";
      if (!Utils::FindOrCreateDirectoryW(save_path.c_str())) {
        return -3;
      }
      thumb_path = save_path + L"\\" + std::to_wstring(msg_id) + L"_t.dat";
      save_path = save_path + L"\\" + std::to_wstring(msg_id) + L".dat";
      break;
    }
    case 0x3E:
    case 0x2B: {
      save_path += L"\\video";
      if (!Utils::FindOrCreateDirectoryW(save_path.c_str())) {
        return -3;
      }
      thumb_path = save_path + L"\\" + std::to_wstring(msg_id) + L".jpg";
      save_path = save_path + L"\\" + std::to_wstring(msg_id) + L".mp4";

      break;
    }
    case 0x31: {
      save_path += L"\\file";
      if (!Utils::FindOrCreateDirectoryW(save_path.c_str())) {
        return -3;
      }
      char *p_xml_app_msg = Utils::WxHeapAlloc<char>(0x3000);
      UINT64 xml_msg =
          new_app_msg_info(reinterpret_cast<UINT64>(p_xml_app_msg));
      UINT64 result =
          xml_to_app_info(xml_msg, reinterpret_cast<UINT64>(we_content), 1);
      if (result != 1) {
        return -4;
      }
      std::wstring file_name = Utils::ReadWstring(xml_msg + 0x70);
      save_path =
          save_path + L"\\" + std::to_wstring(msg_id) + L"_" + file_name;
      free_app_msg_info(xml_msg);
      break;
    }
    default:
      break;
  }
  prototype::WeChatString *we_save_path = BuildWechatString(save_path);
  prototype::WeChatString *we_thumb_path = BuildWechatString(thumb_path);
  int temp = 1;
  memcpy(chat_msg + 0x280, we_thumb_path, sizeof(prototype::WeChatString));
  memcpy(chat_msg + 0x2A0, we_save_path, sizeof(prototype::WeChatString));
  memcpy(chat_msg + 0x40C, &temp, sizeof(temp));
  UINT64 mgr = get_pre_download_mgr();
  success = push_attach_task(mgr, p_chat_msg, 0, 1);
  free_chat_msg(p_chat_msg);

  return success;
}

INT64 Manager::ForwardPublicMsg(const std::wstring &wxid,
                                const std::wstring &title,
                                const std::wstring &url,
                                const std::wstring &thumb_url,
                                const std::wstring &sender_id,
                                const std::wstring &sender_name,
                                const std::wstring &digest) {
  INT64 success = -1;
  UINT64 new_item_addr = base_addr_ + offset::kNewMMReaderItem;
  func::__NewMMReaderItem new_item = (func::__NewMMReaderItem)new_item_addr;

  UINT64 free_item_addr = base_addr_ + offset::kFreeMMReaderItem;
  func::__FreeMMReaderItem free_item = (func::__FreeMMReaderItem)free_item_addr;

  UINT64 get_app_msg_mgr_addr = base_addr_ + offset::kGetAppMsgMgr;
  func::__GetAppMsgMgr get_app_mgr = (func::__GetAppMsgMgr)get_app_msg_mgr_addr;

  UINT64 forward_public_msg_addr = base_addr_ + offset::kForwordPublicMsg;
  func::__ForwordPublicMsg forward_public_msg =
      (func::__ForwordPublicMsg)forward_public_msg_addr;

  char *p_item = Utils::WxHeapAlloc<char>(0x3E4);
  new_item(reinterpret_cast<UINT64>(p_item));
  prototype::WeChatString *to_user = BuildWechatString(wxid);
  prototype::WeChatString *p_title = BuildWechatString(title);
  prototype::WeChatString *p_url = BuildWechatString(url);
  prototype::WeChatString *p_thumburl = BuildWechatString(thumb_url);
  prototype::WeChatString *p_sender_id = BuildWechatString(sender_id);
  prototype::WeChatString *p_name = BuildWechatString(sender_name);
  prototype::WeChatString *p_digest = BuildWechatString(digest);

  memcpy(p_item + 0x8, p_title, sizeof(prototype::WeChatString));
  memcpy(p_item + 0x48, p_url, sizeof(prototype::WeChatString));
  memcpy(p_item + 0xB0, p_thumburl, sizeof(prototype::WeChatString));
  memcpy(p_item + 0xF0, p_digest, sizeof(prototype::WeChatString));
  memcpy(p_item + 0x2A0, p_sender_id, sizeof(prototype::WeChatString));
  memcpy(p_item + 0x2C0, p_name, sizeof(prototype::WeChatString));
  memcpy(p_item + 0x2C0, p_name, sizeof(prototype::WeChatString));

  UINT64 mgr = get_app_mgr();
  success = forward_public_msg(mgr, reinterpret_cast<UINT64>(to_user),
                               reinterpret_cast<UINT64>(p_item));
  free_item(reinterpret_cast<UINT64>(p_item));
  return success;
}

INT64 Manager::ForwardPublicMsgByMsgId(const std::wstring &wxid,
                                       UINT64 msg_id) {
  INT64 success = -1;
  std::string compress_content =
      DB::GetInstance().GetPublicMsgCompressContentByMsgId(msg_id);
  if (compress_content.empty()) {
    SPDLOG_INFO("Get compressContent is null from PublicMsg.db");
    return -3;
  }

  std::string decode = base64_decode(compress_content);
  size_t len = decode.size();
  const char *src = decode.c_str();
  size_t dst_len = (len << 8);
  char *dst = new char[dst_len];

  int decompress_len = LZ4_decompress_safe_partial(
      src, dst, static_cast<int>(len), static_cast<int>(dst_len), static_cast<int>(dst_len));
  if (decompress_len < 0) {
    SPDLOG_INFO("decompress content size :{}", decompress_len);
    return -1;
  }
  tinyxml2::XMLDocument doc;
  if (doc.Parse(dst, decompress_len - 1) != 0) {
    SPDLOG_INFO("tinyxml2 parse error");
    return -2;
  }
  const char *title = doc.FirstChildElement("msg")
                          ->FirstChildElement("appmsg")
                          ->FirstChildElement("title")
                          ->GetText();
  const char *digest = doc.FirstChildElement("msg")
                           ->FirstChildElement("appmsg")
                           ->FirstChildElement("des")
                           ->GetText();

  const char *url = doc.FirstChildElement("msg")
                        ->FirstChildElement("appmsg")
                        ->FirstChildElement("mmreader")
                        ->FirstChildElement("category")
                        ->FirstChildElement("item")
                        ->FirstChildElement("url")
                        ->GetText();
  const char *thumb_url = doc.FirstChildElement("msg")
                              ->FirstChildElement("appmsg")
                              ->FirstChildElement("thumburl")
                              ->GetText();
  const char *user_name = doc.FirstChildElement("msg")
                              ->FirstChildElement("appmsg")
                              ->FirstChildElement("mmreader")
                              ->FirstChildElement("publisher")
                              ->FirstChildElement("username")
                              ->GetText();

  const char *nickname = doc.FirstChildElement("msg")
                             ->FirstChildElement("appmsg")
                             ->FirstChildElement("mmreader")
                             ->FirstChildElement("publisher")
                             ->FirstChildElement("nickname")
                             ->GetText();

  std::string s_title(title);
  std::string s_digest(digest);
  std::string s_url(url);
  std::string s_thumburl(thumb_url);
  std::string s_user_name(user_name);
  std::string s_nickname(nickname);

  std::wstring ws_title = Utils::UTF8ToWstring(s_title);
  std::wstring ws_digest = Utils::UTF8ToWstring(s_digest);
  std::wstring ws_url = Utils::UTF8ToWstring(s_url);
  std::wstring ws_thumb_url = Utils::UTF8ToWstring(s_thumburl);
  std::wstring ws_user_name = Utils::UTF8ToWstring(s_user_name);
  std::wstring ws_nickname = Utils::UTF8ToWstring(s_nickname);
  success = ForwardPublicMsg(wxid, ws_title, ws_url, ws_thumb_url, ws_user_name,
                             ws_nickname, ws_digest);
  return success;
}

INT64 Manager::DecodeImage(const std::wstring &file_path, const std::wstring &save_dir) {
  return Utils::DecodeImage(file_path.c_str(), save_dir.c_str());
}

INT64 Manager::GetVoiceByDB(ULONG64 msg_id, const std::wstring& dir) {
  INT64 success = -1;
  std::string buff = DB::GetInstance().GetVoiceBuffByMsgId(msg_id);
  if (buff.size() == 0) {
    success = 0;
    return success;
  }
  std::wstring save_path = dir;
  if (!Utils::FindOrCreateDirectoryW(save_path.c_str())) {
    success = -2;
    return success;
  }
  save_path = save_path + L"\\" + std::to_wstring(msg_id) + L".amr";
  HANDLE file_handle = CreateFileW(save_path.c_str(), GENERIC_ALL, 0, NULL,
                                   CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (file_handle == INVALID_HANDLE_VALUE) {
    SPDLOG_ERROR("GetVoiceByDB save path invalid");
    return success;
  }
  DWORD bytes_write = 0;
  std::string decode = base64_decode(buff);
  WriteFile(file_handle, (LPCVOID)decode.c_str(), static_cast<DWORD>(decode.size()) , &bytes_write, 0);
  CloseHandle(file_handle);
  success = 1;
  return success;
}

INT64 Manager::SendCustomEmotion(const std::wstring &file_path,
                                 const std::wstring &wxid) {
  INT64 success = -1;
  UINT64 get_custom_smiley_mgr_addr = base_addr_ + offset::kGetCustomSmileyMgr;
  func::__GetCustomSmileyMgr get_custom_smiley_mgr =
      (func::__GetCustomSmileyMgr)get_custom_smiley_mgr_addr;
  UINT64 send_custom_emotion_addr = base_addr_ + offset::kSendCustomEmotion;
  func::__SendCustomEmotion send_custom_emotion =
      (func::__SendCustomEmotion)send_custom_emotion_addr;
  prototype::WeChatString *path = BuildWechatString(file_path);
  prototype::WeChatString *recv = BuildWechatString(wxid);
  INT64 *temp = Utils::WxHeapAlloc<INT64>(0x20);
  memset(temp, 0, 0x20);
  UINT64 mgr = get_custom_smiley_mgr();
  success = send_custom_emotion(
      mgr, reinterpret_cast<UINT64>(path), reinterpret_cast<UINT64>(temp),
      reinterpret_cast<UINT64>(recv), 2, reinterpret_cast<UINT64>(temp), 0,
      reinterpret_cast<UINT64>(temp));
  return success;
}

INT64 Manager::SendApplet(const std::wstring &recv_wxid,
                          const std::wstring &waid_suff,
                          const std::wstring &waid_w, const std::string &waid_s,
                          const std::string &wa_wxid,
                          const std::string &json_param,
                          const std::string &head_image,
                          const std::string &big_image,
                          const std::string &index_page) {
  INT64 success = -1;
  if (js_api_addr_ == 0) {
    auto vec2 = Utils::QWordScan(base_addr_ + 0x32D1318, 0x1000, 0x8);
    for (int i = 0; i < vec2.size(); i++) {
      INT64 ptr = vec2.at(i);
      if (*(INT64 *)ptr == base_addr_ + 0x32D1318) {
        js_api_addr_ = ptr;
        break;
      }
    }
  }
  if (js_api_addr_ == 0) {
    success = -2;
    return success;
  }

  UINT64 share_app_msg_addr = base_addr_ + offset::kNewJsApiShareAppMessage;
  func::__JsApiShareAppMessage share_app_msg =
      (func::__JsApiShareAppMessage)share_app_msg_addr;

  UINT64 init_addr = base_addr_ + offset::kInitJsConfig;
  func::__InitJsConfig init = (func::__InitJsConfig)init_addr;

  UINT64 send_applet_addr = base_addr_ + offset::kSendApplet;
  func::__SendApplet send_applet = (func::__SendApplet)send_applet_addr;

  UINT64 get_by_waid_addr = base_addr_ + offset::kGetAppInfoByWaid;
  func::__GetAppInfoByWaid get_app_info =
      (func::__GetAppInfoByWaid)get_by_waid_addr;

  UINT64 copy_app_req_addr = base_addr_ + offset::kCopyShareAppMessageRequest;
  func::__CopyShareAppMessageRequest copy_app_req =
      (func::__CopyShareAppMessageRequest)copy_app_req_addr;

  UINT64 new_wa_msg_addr = base_addr_ + offset::kNewWAUpdatableMsgInfo;
  func::__NewWAUpdatableMsgInfo new_wa_msg =
      (func::__NewWAUpdatableMsgInfo)new_wa_msg_addr;

  UINT64 free_wa_msg_addr = base_addr_ + offset::kFreeWAUpdatableMsgInfo;
  func::__FreeWAUpdatableMsgInfo free_wa_msg =
      (func::__FreeWAUpdatableMsgInfo)free_wa_msg_addr;

  std::vector<prototype::WeChatString> *temp =
      Utils::WxHeapAlloc<std::vector<prototype::WeChatString>>(0x20);
  // std::vector<prototype::WeChatString>*  temp = new
  // std::vector<prototype::WeChatString>();
  common::VectorInner *list = (common::VectorInner *)temp;

  prototype::WeChatString *member = BuildWechatString(recv_wxid);

  list->head = reinterpret_cast<UINT64>(member);
  list->start = reinterpret_cast<UINT64>(member);
  list->finsh = reinterpret_cast<UINT64>(member) + 0x20;
  list->end = reinterpret_cast<UINT64>(member) + 0x20;

  INT64 head = reinterpret_cast<UINT64>(&(list->start));

  prototype::WeChatString *waid_cat = BuildWechatString(waid_suff);
  prototype::WeChatString *waid = BuildWechatString(waid_w);

  prototype::WeChatString *waid_2 = BuildWechatString(waid_suff);

  prototype::WeChatStr *waid_str = BuildWechatStr(waid_s);
  prototype::WeChatStr *app_wxid = BuildWechatStr(wa_wxid);
  prototype::WeChatStr *json_str = BuildWechatStr(json_param);
  prototype::WeChatStr *head_image_url = BuildWechatStr(head_image);
  prototype::WeChatStr *image = BuildWechatStr(big_image);
  prototype::WeChatStr *index = BuildWechatStr(index_page);

  UINT64 app_msg = js_api_addr_;

  UINT64 data = *(UINT64 *)(app_msg + 0x8);
  char *share_req = Utils::WxHeapAlloc<char>(0x2000);

  char *mid_ptr = Utils::WxHeapAlloc<char>(0x18);
  memcpy(mid_ptr, &share_req, sizeof(INT64));
  memcpy(mid_ptr + 0x8, &share_req, sizeof(INT64));
  memcpy(mid_ptr + 0x10, &share_req, sizeof(INT64));

  memcpy((void *)data, mid_ptr, 0x18);

  memcpy(share_req, (void *)(app_msg + 0x8), sizeof(UINT64));
  memcpy(share_req + 0x8, (void *)(app_msg + 0x8), sizeof(UINT64));
  memcpy(share_req + 0x10, (void *)(app_msg + 0x8), sizeof(UINT64));
  memcpy(share_req + 0x20, waid_2, sizeof(prototype::WeChatString));
  memcpy(share_req + 0x48, waid_str, sizeof(prototype::WeChatStr));
  memcpy(share_req + 0x98, app_wxid, sizeof(prototype::WeChatStr));
  memcpy(share_req + 0xF8, json_str, sizeof(prototype::WeChatStr));
  memcpy(share_req + 0x178, head_image_url, sizeof(prototype::WeChatStr));
  memcpy(share_req + 0x198, image, sizeof(prototype::WeChatStr));
  memcpy(share_req + 0x1c0, index, sizeof(prototype::WeChatStr));

  success = send_applet(app_msg, reinterpret_cast<UINT64>(waid_cat), head, 0);

  return success;
}

INT64 Manager::SendPatMsg(const std::wstring &room_id,
                          const std::wstring &wxid) {
  INT64 success = -1;
  UINT64 send_pat_msg_addr = base_addr_ + offset::kSendPatMsg;
  func::__SendPatMsg send_pat_msg =(func::__SendPatMsg)send_pat_msg_addr;
  prototype::WeChatString chat_room(room_id);
  prototype::WeChatString target(wxid);
  success = send_pat_msg(reinterpret_cast<UINT64>(&chat_room),reinterpret_cast<UINT64>(&target));
  return success;
}

INT64 Manager::DoOCRTask(const std::wstring &img_path, std::string &result) {
  INT64 success = -1;
  UINT64 ocr_manager_addr = base_addr_ + offset::kGetOCRManager;
  func::__GetOCRManager ocr_manager = (func::__GetOCRManager)ocr_manager_addr;

  UINT64 do_ocr_task_addr = base_addr_ + offset::kDoOCRTask;
  func::__DoOCRTask do_ocr_task = (func::__DoOCRTask)do_ocr_task_addr;

  prototype::WeChatString img(img_path);
  std::vector<INT64> *temp = Utils::WxHeapAlloc<std::vector<INT64>>(0x20);
  INT64 unkonwn = 0;
  common::VectorInner *list = (common::VectorInner *)temp;
  list->start = reinterpret_cast<INT64>(&list->start);
  list->finsh = list->start;
  char buff[0x28] = {0};
  memcpy(buff, &list->start, sizeof(INT64));
  UINT64 mgr = ocr_manager();
  success = do_ocr_task(mgr, reinterpret_cast<UINT64>(&img),1,
                        reinterpret_cast<UINT64>(buff),reinterpret_cast<UINT64>(&unkonwn));
  INT64 number = *(INT64 *)(buff + 0x8);
  if (number > 0) {
    INT64 header = list->start;
    for (unsigned int i = 0; i < number - 1; i++) {
      INT64 content = *(INT64 *)header;
      result += Utils::ReadWstringThenConvert(content + 0x28);
      result += "\r\n";
      header = content;
    }
  }
  return success;
}

INT64 Manager::Test() {
  auto vec = Utils::QWordScan(base_addr_ + 0x32D1318, 0x1, L"WeChatWin.dll");
  for (int i = 0; i < vec.size(); i++) {
    INT64 re = vec.at(i);
    SPDLOG_INFO("scan result :{},{}", i, re);
  }

  auto vec2 = Utils::QWordScan(base_addr_ + 0x32D1318, 0x1000, 0x8);
  for (int i = 0; i < vec2.size(); i++) {
    INT64 re = vec2.at(i);
    SPDLOG_INFO("scan2 result :{},{}", i, re);
  }
  return 1;
}
} // namespace wxhelper
