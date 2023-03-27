#include "pch.h"
#include "pat.h"

#include "common.h"
#include "wechat_data.h"

#define  WX_PAT_MGR_OFFSET  0x931730
#define  WX_SEND_PAT_MSG_OFFSET  0x1421940
#define  WX_RET_OFFSET   0x1D58751

int SendPatMsg(wchar_t* chat_room_id, wchar_t* wxid) {
  int success = -1;
  WeChatString chat_room(chat_room_id);
  WeChatString self_wxid(wxid);
  DWORD base = GetWeChatWinBase();
  DWORD get_pat_mgr_addr = base + WX_PAT_MGR_OFFSET;
  DWORD send_pat_msg_addr = base + WX_SEND_PAT_MSG_OFFSET;
  DWORD ret_addr = base + WX_RET_OFFSET;
  __asm {
    PUSHAD
    CALL       get_pat_mgr_addr
    PUSH       ret_addr
    PUSH       0x0
    PUSH       EAX
    LEA        ECX,chat_room
    LEA        EDX,self_wxid
    CALL       send_pat_msg_addr                           
    ADD        ESP,0xc
    MOVZX      EAX,AL
    MOV        success,EAX      
    POPAD
  }
  return success;
}