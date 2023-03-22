#include "pch.h"
#include "send_image.h"
#include "common.h"
#include "wechat_data.h"

#define WX_SEND_IMAGE_OFFSET  0xce6640
#define WX_SEND_MESSAGE_MGR_OFFSET 0x768140
#define WX_INIT_CHAT_MSG_OFFSET  0xf59e40
#define WX_FREE_CHAT_MSG_OFFSET 0x756960
int SendImage(wchar_t *wxid, wchar_t *image_path){

  int success = 0;
  WeChatString to_user(wxid);
  WeChatString path(image_path);
  char chat_msg[0x2D8] ={0};
  DWORD base = GetWeChatWinBase();
  DWORD send_message_mgr_addr = base + WX_SEND_MESSAGE_MGR_OFFSET;
  DWORD init_chat_msg_addr = base + WX_INIT_CHAT_MSG_OFFSET;
  DWORD send_image_msg_addr = base + WX_SEND_IMAGE_OFFSET;
  DWORD free_msg_addr = base + WX_FREE_CHAT_MSG_OFFSET;
  DWORD temp = 0;
  WeChatString null_obj = {0};
  __asm{
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
  return success;
}