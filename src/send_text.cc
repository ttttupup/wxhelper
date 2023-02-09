#include "pch.h"
#include "send_text.h"


#include "common.h"
#include "wechat_data.h"

#define WX_SEND_TEXT_OFFSET 0xc71a60

#define WX_SEND_MESSAGE_MGR_OFFSET 0x706d30

#define WX_FREE_CHAT_MSG_OFFSET 0x6f4ea0
/// @brief 发生文本消息
/// @param wxid wxid
/// @param msg  文本消息
/// @return  成功返回1
int  SendText(wchar_t* wxid, wchar_t* msg) {
  int success = 0;
  WeChatString to_user(wxid);
  WeChatString text_msg(msg);
  wchar_t **msg_pptr = &text_msg.ptr;
  
  DWORD base = GetWeChatWinBase();
  DWORD send_message_mgr_addr = base + WX_SEND_MESSAGE_MGR_OFFSET;
  DWORD send_text_msg_addr = base + WX_SEND_TEXT_OFFSET;
  DWORD free_chat_msg_addr = base + WX_FREE_CHAT_MSG_OFFSET;
  char chat_msg[0x2C4] ={0};
  __asm{
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
  return success;
}