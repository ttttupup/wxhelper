#include "pch.h"
#include "send_file.h"
#include "common.h"
#include "wechat_data.h"

#define WX_APP_MSG_MGR_OFFSET   0x665f60
#define WX_SEND_FILE_OFFSET     0xa0ce20
#define WX_INIT_CHAT_MSG_OFFSET 0xdbcc40
#define WX_FREE_CHAT_MSG_OFFSET 0x651c40

int  SendFile(wchar_t *wxid, wchar_t *file_path){
  int success = 0;
  WeChatString to_user(wxid);
  WeChatString path(file_path);
  char chat_msg[0x2A8] = {0};
  DWORD base = GetWeChatWinBase();
  DWORD app_msg_mgr_addr = base + WX_APP_MSG_MGR_OFFSET;
  DWORD init_chat_msg_addr = base + WX_INIT_CHAT_MSG_OFFSET;
  DWORD send_file_addr = base + WX_SEND_FILE_OFFSET;
  DWORD free_msg_addr = base + WX_FREE_CHAT_MSG_OFFSET;
  DWORD temp = 0;
  WeChatString null_obj = {0};
  __asm{
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
  if (success == 0x31){
   return 1;
  }
  return 0;
}