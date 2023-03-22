#include "pch.h"
#include "send_text.h"


#include "common.h"
#include "wechat_data.h"
#include "contact.h"

#define WX_SEND_TEXT_OFFSET 0xce6c80

#define WX_SEND_MESSAGE_MGR_OFFSET 0x768140

#define WX_FREE_CHAT_MSG_OFFSET 0x756960
using namespace std;
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
  char chat_msg[0x2D8] ={0};
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



int SendAtText(wchar_t* chat_room_id,wchar_t** wxids,int len,wchar_t* msg){
  int success = -1;
  WeChatString  * at_users = new WeChatString[len+1];
  wstring at_msg = L"";
  int number =0;
  for (int i = 0; i < len; i++) {
    wstring nickname;
    if (!lstrcmpiW((wchar_t *)wxids[i], (wchar_t *)L"notify@all")) {
      nickname = L"������";
    } else {
      nickname = GetContactOrChatRoomNickname(wxids[i]);
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
  wstring origin(msg);
  at_msg += origin;
  AtInner at_list = {0};
  at_list.start = (DWORD)at_users;
  at_list.finsh = (DWORD)&at_users[number];
  at_list.end = (DWORD)&at_users[number];
  WeChatString to_user(chat_room_id);
  WeChatString text_msg((wchar_t *)at_msg.c_str());
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
      POPAD
  }
  return success;
}