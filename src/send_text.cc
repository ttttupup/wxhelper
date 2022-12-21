#include "pch.h"
#include "send_text.h"


#include "common.h"
#include "wechat_data.h"

#define WX_SEND_TEXT_OFFSET 0xb690a0

#define WX_SEND_MESSAGE_MGR_OFFSET 0x663320

#define WX_FREE_CHAT_MSG_OFFSET 0x651c40
/// @brief 发生文本消息
/// @param wxid wxid
/// @param msg  文本消息
/// @return  成功返回1
int SendText(wchar_t* wxid, wchar_t* msg) {
  int success = 0;
  WeChatString to_user(wxid);
  WeChatString text_msg(msg);
  wchar_t **msg_pptr = &text_msg.ptr;
  char chat_msg[0x2A8] ={0};
  DWORD base = GetWeChatWinBase();
  DWORD send_message_mgr_addr = base + WX_SEND_MESSAGE_MGR_OFFSET;
  DWORD send_text_msg_addr = base + WX_SEND_TEXT_OFFSET;
  DWORD free_msg_addr = base + WX_FREE_CHAT_MSG_OFFSET;
  __asm{
      PUSHAD
      PUSH       0x0
      PUSH       0x0
      PUSH       0x1
      PUSH       0x0
      MOV        EDI,msg_pptr
      PUSH       EDI
      LEA        EDX,to_user
      LEA        ECX,chat_msg
      CALL       send_text_msg_addr               
      ADD        ESP,0x14
      MOV        success,EAX
      LEA        ECX,chat_msg
      CALL       free_msg_addr
      POPAD
  }
  return success;
}