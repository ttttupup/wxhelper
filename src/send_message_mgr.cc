#include "pch.h"
#include "send_message_mgr.h"

#include "easylogging++.h"

#include "wechat_function.h"
#include "db.h"

namespace wxhelper {
SendMessageMgr::SendMessageMgr(DWORD base):BaseMgr(base) {}
SendMessageMgr::~SendMessageMgr() {}
int SendMessageMgr::SendText(wchar_t* wxid, wchar_t* msg) {
  int success = -1;
  WeChatString to_user(wxid);
  WeChatString text_msg(msg);
  wchar_t** msg_pptr = &text_msg.ptr;
  DWORD base = Utils::GetWeChatWinBase();
  DWORD send_message_mgr_addr = base + WX_SEND_MESSAGE_MGR_OFFSET;
  DWORD send_text_msg_addr = base + WX_SEND_TEXT_OFFSET;
  DWORD free_chat_msg_addr = base + WX_FREE_CHAT_MSG_OFFSET;
  char chat_msg[0x2D8] = {0};
  __asm {
      PUSHAD
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
      POPAD
  }
  LOG_IF((success == -1), ERROR) << "SendText fail";
  return success;
}
int SendMessageMgr::SendAtText(wchar_t* chat_room_id, wchar_t** wxids, int len,
                               wchar_t* msg) {
  int success = -1;
  return success;
}
int SendMessageMgr::SendImage(wchar_t* wxid, wchar_t* image_path) {
  int success = -1;
  WeChatString to_user(wxid);
  WeChatString path(image_path);
  char chat_msg[0x2D8] = {0};
  DWORD base = Utils::GetWeChatWinBase();
  DWORD send_message_mgr_addr = base + WX_SEND_MESSAGE_MGR_OFFSET;
  DWORD init_chat_msg_addr = base + WX_INIT_CHAT_MSG_OFFSET;
  DWORD send_image_msg_addr = base + WX_SEND_IMAGE_OFFSET;
  DWORD free_msg_addr = base + WX_FREE_CHAT_MSG_OFFSET;
  DWORD temp = 0;
  WeChatString null_obj = {0};
  __asm {
      PUSHAD
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
      POPAD
  }
  LOG_IF((success == -1), ERROR) << "SendImage fail";
  return success;
}
int SendMessageMgr::SendFile(wchar_t* wxid, wchar_t* file_path) {
  int success = -1;
  WeChatString to_user(wxid);
  WeChatString path(file_path);
  char chat_msg[0x2D8] = {0};
  DWORD base = Utils::GetWeChatWinBase();
  DWORD app_msg_mgr_addr = base + WX_APP_MSG_MGR_OFFSET;
  DWORD init_chat_msg_addr = base + WX_INIT_CHAT_MSG_OFFSET;
  DWORD send_file_addr = base + WX_SEND_FILE_OFFSET;
  DWORD free_msg_addr = base + WX_FREE_CHAT_MSG_OFFSET;
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
  if (success == 0x31) {
    return 1;
  }
  LOG_IF((success == -1), ERROR) << "SendFile fail";
  return success;
}

int SendMessageMgr::ForwardMsg(wchar_t* wxid, unsigned long long msgid) {
  int success = 0;

  int db_index = 0;
  int localid = DB::GetInstance().GetLocalIdByMsgId(msgid, db_index);

  if (localid == 0) return 0;
  WeChatString to_user(wxid);
  DWORD base = Utils::GetWeChatWinBase();
  DWORD forward_msg_addr = base + WX_FORWARD_MSG_OFFSET;
  DWORD init_chat_msg_addr = base + WX_INIT_CHAT_MSG_OFFSET;
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
}  // namespace wxhelper