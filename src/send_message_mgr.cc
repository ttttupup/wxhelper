#include "pch.h"
#include "send_message_mgr.h"
#include "wechat_function.h"
#include "db.h"
#include "contact_mgr.h"
#include "spdlog/spdlog.h"
namespace wxhelper {
SendMessageMgr::SendMessageMgr(DWORD base):BaseMgr(base) {}
SendMessageMgr::~SendMessageMgr() {}
int SendMessageMgr::SendText(wchar_t* wxid, wchar_t* msg) {
  int success = -1;
  WeChatString to_user(wxid);
  WeChatString text_msg(msg);
  wchar_t** msg_pptr = &text_msg.ptr;
  DWORD send_message_mgr_addr = base_addr_ + WX_SEND_MESSAGE_MGR_OFFSET;
  DWORD send_text_msg_addr = base_addr_ + WX_SEND_TEXT_OFFSET;
  DWORD free_chat_msg_addr = base_addr_ + WX_FREE_CHAT_MSG_OFFSET;
  char chat_msg[0x2D8] = {0};
  __asm {
      PUSHAD
      PUSHFD
      CALL       send_message_mgr_addr
      PUSH       0x0
      PUSH       0x0
      PUSH       0x0
      PUSH       0x1
      PUSH       0x0
      MOV        EAX,msg_pptr
      PUSH       EAX
      LEA        EDX,to_user
      LEA        ECX,chat_msg
      CALL       send_text_msg_addr 
      MOV        success,EAX
      ADD        ESP,0x18
      LEA        ECX,chat_msg        
      CALL       free_chat_msg_addr
      POPFD
      POPAD
  }
  return success;
}
int SendMessageMgr::SendAtText(wchar_t* chat_room_id, wchar_t** wxids, int len,
                               wchar_t* msg) {
  int success = -1;
  WeChatString  * at_users = new WeChatString[len+1];
  std::wstring at_msg = L"";
  int number =0;
  for (int i = 0; i < len; i++) {
    std::wstring nickname;
    if (!lstrcmpiW((wchar_t *)wxids[i], (wchar_t *)L"notify@all")) {
      nickname = L"所有人";
    } else {
      ContactMgr contact{base_addr_};
      nickname = contact.GetContactOrChatRoomNickname(wxids[i]);
    }
    if (nickname.length() == 0) {
      continue;
    }

    WeChatString temp = {0};
    temp.ptr = (wchar_t *)wxids[i];
    temp.length = wcslen((wchar_t *)wxids[i]);
    temp.max_length = wcslen((wchar_t *)wxids[i]) * 2;
    memcpy(&at_users[number], &temp, sizeof(WeChatString));
    at_msg = at_msg + L"@" + nickname + L" ";
    number++;
  }
  if (number < 1){
    return success;
  }
  std::wstring origin(msg);
  at_msg += origin;
  AtInner at_list = {0};
  at_list.start = (DWORD)at_users;
  at_list.finsh = (DWORD)&at_users[number];
  at_list.end = (DWORD)&at_users[number];
  WeChatString to_user(chat_room_id);
  WeChatString text_msg((wchar_t *)at_msg.c_str());
  wchar_t **msg_pptr = &text_msg.ptr;
  
  DWORD send_message_mgr_addr = base_addr_ + WX_SEND_MESSAGE_MGR_OFFSET;
  DWORD send_text_msg_addr = base_addr_ + WX_SEND_TEXT_OFFSET;
  DWORD free_chat_msg_addr = base_addr_ + WX_FREE_CHAT_MSG_OFFSET;
  char chat_msg[0x2D8] = {0};
  __asm{
      PUSHAD
      PUSHFD
      CALL       send_message_mgr_addr
      PUSH       0x0
      PUSH       0x0
      PUSH       0x0
      PUSH       0x1
      LEA        EAX,at_list
      PUSH       EAX
      MOV        EAX,msg_pptr
      PUSH       EAX
      LEA        EDX,to_user
      LEA        ECX,chat_msg
      CALL       send_text_msg_addr 
      MOV        success,EAX
      ADD        ESP,0x18
      LEA        ECX,chat_msg        
      CALL       free_chat_msg_addr
      POPFD
      POPAD
  }
  SPDLOG_INFO("SendText  code = {}",success);
  return success;
}
int SendMessageMgr::SendImage(wchar_t* wxid, wchar_t* image_path) {
  int success = -1;
  WeChatString to_user(wxid);
  WeChatString path(image_path);
  char chat_msg[0x2D8] = {0};
  DWORD send_message_mgr_addr = base_addr_ + WX_SEND_MESSAGE_MGR_OFFSET;
  DWORD init_chat_msg_addr = base_addr_ + WX_INIT_CHAT_MSG_OFFSET;
  DWORD send_image_msg_addr = base_addr_ + WX_SEND_IMAGE_OFFSET;
  DWORD free_msg_addr = base_addr_ + WX_FREE_CHAT_MSG_OFFSET;
  DWORD temp = 0;
  WeChatString null_obj = {0};
  __asm {
      PUSHAD
      PUSHFD
      CALL       send_message_mgr_addr                        
      SUB        ESP,0x14
      MOV        temp,EAX
      LEA        EAX,null_obj
      MOV        ECX,ESP
      LEA        EDI,path
      PUSH       EAX
      CALL       init_chat_msg_addr                                   
      MOV        ECX,dword ptr [temp]
      LEA        EAX,to_user
      PUSH       EDI
      PUSH       EAX
      LEA        EAX,chat_msg
      PUSH       EAX
      CALL       send_image_msg_addr
      MOV        success,EAX
      LEA        ECX,chat_msg
      CALL       free_msg_addr
      POPFD
      POPAD
  }
  SPDLOG_INFO("SendImage  code = {}",success);
  return success;
}
int SendMessageMgr::SendFile(wchar_t* wxid, wchar_t* file_path) {
  int success = -1;
  WeChatString to_user(wxid);
  WeChatString path(file_path);
  char chat_msg[0x2D8] = {0};
  DWORD app_msg_mgr_addr = base_addr_ + WX_APP_MSG_MGR_OFFSET;
  DWORD init_chat_msg_addr = base_addr_ + WX_INIT_CHAT_MSG_OFFSET;
  DWORD send_file_addr = base_addr_ + WX_SEND_FILE_OFFSET;
  DWORD free_msg_addr = base_addr_ + WX_FREE_CHAT_MSG_OFFSET;
  DWORD temp = 0;
  WeChatString null_obj = {0};
  __asm {
      PUSHAD
      PUSHFD
      CALL       app_msg_mgr_addr  
      SUB        ESP,0x14
      MOV        temp,EAX
      LEA        EAX,null_obj
      MOV        ECX,ESP
      PUSH       EAX
      CALL       init_chat_msg_addr 
      PUSH       0x0 
      SUB        ESP,0x14
      MOV        EDI,ESP
      MOV        dword ptr [EDI],0  
      MOV        dword ptr [EDI + 0x4],0  
      MOV        dword ptr [EDI + 0x8],0  
      MOV        dword ptr [EDI + 0xc],0  
      MOV        dword ptr [EDI + 0x10],0  
      SUB        ESP,0x14
      LEA        EAX,path
      MOV        ECX,ESP
      PUSH       EAX
      CALL       init_chat_msg_addr                                   
      SUB        ESP,0x14
      LEA        EAX,to_user
      MOV        ECX,ESP
      PUSH       EAX
      CALL       init_chat_msg_addr                                    
      MOV        ECX,dword ptr [temp]
      LEA        EAX,chat_msg
      PUSH       EAX
      CALL       send_file_addr
      MOV        AL,byte ptr [eax + 0x38]
      MOVZX      EAX,AL                         
      MOV        success,EAX
      LEA        ECX,chat_msg
      CALL       free_msg_addr
      POPFD
      POPAD
  }
  SPDLOG_INFO("SendFile  code = {}",success);
  if (success == 0x31) {
    return 1;
  }
  return success;
}

int SendMessageMgr::ForwardMsg(wchar_t* wxid, unsigned long long msgid) {
  int success = 0;

  int db_index = 0;
  int localid = DB::GetInstance().GetLocalIdByMsgId(msgid, db_index);

  if (localid == 0) return 0;
  WeChatString to_user(wxid);
  DWORD forward_msg_addr = base_addr_ + WX_FORWARD_MSG_OFFSET;
  DWORD init_chat_msg_addr = base_addr_ + WX_INIT_CHAT_MSG_OFFSET;
  __asm {
    PUSHAD
    PUSHFD
    MOV        EDX, DWORD PTR [db_index]
    PUSH       EDX
    MOV        EAX, DWORD PTR [localid]
    PUSH       EAX
    SUB        ESP,0x14
    MOV        ECX,ESP
    LEA        ESI,to_user
    PUSH       ESI
    CALL       init_chat_msg_addr      
    XOR        ECX,ECX                          
    CALL       forward_msg_addr
    MOVZX      EAX,AL
    MOV        success,EAX
    ADD        ESP,0x1c
    POPFD
    POPAD
  }
  return success;
}

int SendMessageMgr::ForwardPublicMsg(wchar_t* wxid, wchar_t* title, wchar_t* url, wchar_t* thumburl,wchar_t* senderId,wchar_t* senderName,wchar_t* digest) {
  int success = -1;
  char buff[0x238] = {0};
  DWORD init_chat_msg_addr = base_addr_ + WX_INIT_CHAT_MSG_OFFSET;
  DWORD app_msg_mgr_addr = base_addr_ + WX_APP_MSG_MGR_OFFSET;
  DWORD new_item_addr = base_addr_ + NEW_MM_READ_ITEM_OFFSET;
  DWORD free_item_2_addr = base_addr_ + FREE_MM_READ_ITEM_2_OFFSET;
  DWORD forward_public_msg_addr = base_addr_ + FORWARD_PUBLIC_MSG_OFFSET;
  __asm {
    PUSHAD
    PUSHFD
    LEA        ECX,buff
    CALL       new_item_addr
    POPFD
    POPAD
  }
  WeChatString to_user(wxid);
  WeChatString wtitle(title);
  WeChatString wurl(url);
  WeChatString wthumburl(thumburl);
  WeChatString wsender(senderId);
  WeChatString wname(senderName);
  WeChatString wdigest(digest);
  memcpy(&buff[0x4], &wtitle, sizeof(wtitle));
  memcpy(&buff[0x2c], &wurl, sizeof(wurl));
  memcpy(&buff[0x6c], &wthumburl, sizeof(wthumburl));
  memcpy(&buff[0x94], &wdigest, sizeof(wdigest));
  memcpy(&buff[0x1A0], &wsender, sizeof(wsender));
  memcpy(&buff[0x1B4], &wname, sizeof(wname));
  __asm {
    PUSHAD
    PUSHFD
    CALL       app_msg_mgr_addr
    LEA        ECX,buff
    PUSH       ECX
    SUB        ESP,0x14
    MOV        EDI,EAX
    MOV        ECX,ESP
    LEA        EBX,to_user
    PUSH       EBX
    CALL       init_chat_msg_addr                                  
    MOV        ECX,EDI
    CALL       forward_public_msg_addr
    MOV        success,EAX
    ADD        EBX,0x14
    LEA        ECX,buff
    PUSH       0x0
    CALL       free_item_2_addr
    POPFD
    POPAD
  }
  return success;
}
}  // namespace wxhelper