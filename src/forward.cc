#include "pch.h"
#include "forward.h"

#include "common.h"
#include "get_db_handle.h"
#include "wechat_data.h"
#define WX_FORWARD_MSG_OFFSET 0xce6730
#define WX_INIT_CHAT_MSG_OFFSET 0xf59e40

int ForwardMsg(wchar_t *wxid, unsigned long long msgid) {
  int success = 0;

  int db_index = 0;
  int localid = GetLocalIdByMsgId(msgid, db_index);

  if (localid == 0) return 0;
  WeChatString to_user(wxid);
  DWORD base = GetWeChatWinBase();
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
    CALL       forward_msg_addr
    MOVZX      EAX,AL
    MOV        success,EAX
    ADD        ESP,0x1c
    POPFD
    POPAD
  }
  return success;
}