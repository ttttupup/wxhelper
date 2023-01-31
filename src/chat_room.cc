#include "pch.h"
#include "chat_room.h"

#include "common.h"

#include "wechat_data.h"
#define WX_CHAT_ROOM_MGR_OFFSET 0x67ee70
#define WX_GET_CHAT_ROOM_DETAIL_INFO_OFFSET 0xa73a80
#define WX_NEW_CHAT_ROOM_INFO_OFFSET 0xd07010
#define WX_FREE_CHAT_ROOM_INFO_OFFSET 0xd072f0
#define WX_DEL_CHAT_ROOM_MEMBER_OFFSET 0xa69a50
#define WX_INIT_CHAT_MSG_OFFSET 0xdbf380
#define WX_FREE_CHAT_MSG_OFFSET 0x649ac0
#define WX_ADD_MEMBER_TO_CHAT_ROOM_OFFSET 0xa69560
#define WX_GET_MEMBER_FROM_CHAT_ROOM_OFFSET 0xa749b0
#define WX_INIT_CHAT_ROOM_OFFSET 0xd04d80
#define WX_FREE_CHAT_ROOM_OFFSET 0xa7c620
#define WX_MOD_CHAT_ROOM_MEMBER_NICK_NAME_OFFSET 0xa6f8f0

int GetChatRoomDetailInfo(wchar_t* chat_room_id, ChatRoomInfoInner& room_info) {
  int success = 0;
  WeChatString chat_room(chat_room_id);
  DWORD base = GetWeChatWinBase();
  DWORD get_chat_room_mgr_addr = base + WX_CHAT_ROOM_MGR_OFFSET;
  DWORD get_chat_room_detail_addr = base + WX_GET_CHAT_ROOM_DETAIL_INFO_OFFSET;
  DWORD create_chat_room_info_addr = base + WX_NEW_CHAT_ROOM_INFO_OFFSET;
  DWORD free_chat_room_info_addr = base + WX_FREE_CHAT_ROOM_INFO_OFFSET;
  char chat_room_info[0xDC] = {0};
  __asm {
         PUSHAD
         LEA        ECX,chat_room_info
         CALL       create_chat_room_info_addr
         CALL       get_chat_room_mgr_addr                                  
         PUSH       0x0
         LEA        ECX,chat_room_info
         PUSH       ECX
         LEA        ECX,chat_room
         PUSH       ECX
         MOV        ECX,EAX
         CALL       get_chat_room_detail_addr
         MOV        success,EAX  
         POPAD
  }
  room_info.chat_room_id.ptr = *(wchar_t**)(chat_room_info + 0x4);
  room_info.chat_room_id.length = *(DWORD*)(chat_room_info + 0x8);
  room_info.chat_room_id.max_length = *(DWORD*)(chat_room_info + 0xC);

  room_info.notice.ptr = *(wchar_t**)(chat_room_info + 0x18);
  room_info.notice.length = *(DWORD*)(chat_room_info + 0x1C);
  room_info.notice.max_length = *(DWORD*)(chat_room_info + 0x20);

  room_info.admin.ptr = *(wchar_t**)(chat_room_info + 0x2C);
  room_info.admin.length = *(DWORD*)(chat_room_info + 0x30);
  room_info.admin.max_length = *(DWORD*)(chat_room_info + 0x34);

  room_info.xml.ptr = *(wchar_t**)(chat_room_info + 0x50);
  room_info.xml.length = *(DWORD*)(chat_room_info + 0x54);
  room_info.xml.max_length = *(DWORD*)(chat_room_info + 0x58);
  __asm {
        PUSHAD
        LEA         ECX,chat_room_info
        CALL        free_chat_room_info_addr
        POPAD
  }
  return success;
}

int DelMemberFromChatRoom(wchar_t* chat_room_id, wchar_t** wxids,int len) {
  int success = 0;
  WeChatString chat_room(chat_room_id);
  vector<WeChatString> members;
  VectorInner *list = (VectorInner *)&members;
  DWORD members_ptr = (DWORD)&list->start;
  for (int i = 0; i < len; i++) {
    WeChatString pwxid(wxids[i]);
    members.push_back(pwxid);
  }
  DWORD base = GetWeChatWinBase();
  DWORD get_chat_room_mgr_addr = base + WX_CHAT_ROOM_MGR_OFFSET;
  DWORD del_member_addr = base + WX_DEL_CHAT_ROOM_MEMBER_OFFSET;
  DWORD init_chat_msg_addr = base + WX_INIT_CHAT_MSG_OFFSET;
  __asm {
         PUSHAD
         CALL       get_chat_room_mgr_addr                              
         SUB        ESP,0x14
         MOV        ESI,EAX
         MOV        ECX,ESP
         LEA        EDI,chat_room
         PUSH       EDI
         CALL       init_chat_msg_addr    
         MOV        ECX,ESI
         MOV        EAX,dword ptr[members_ptr]
         PUSH       EAX
         CALL       del_member_addr   
         MOV        success,EAX        
         POPAD
  }
  return success;
}


int AddMemberToChatRoom(wchar_t* chat_room_id, wchar_t** wxids,int len){
 int success = 0;
  WeChatString chat_room(chat_room_id);
  vector<WeChatString> members;
  VectorInner *list = (VectorInner *)&members;
  DWORD members_ptr = (DWORD)&list->start;
  for (int i = 0; i < len; i++) {
    WeChatString pwxid(wxids[i]);
    members.push_back(pwxid);
  }
  DWORD base = GetWeChatWinBase();
  DWORD get_chat_room_mgr_addr = base + WX_CHAT_ROOM_MGR_OFFSET;
  DWORD add_member_addr = base + WX_ADD_MEMBER_TO_CHAT_ROOM_OFFSET;
  DWORD init_chat_msg_addr = base + WX_INIT_CHAT_MSG_OFFSET;
  DWORD temp=0;
  __asm {
         PUSHAD
         PUSHFD
         CALL       get_chat_room_mgr_addr 
         SUB        ESP,0x8
         MOV        temp,EAX
         MOV        ECX,ESP
         MOV        dword ptr [ECX],0x0
         MOV        dword ptr [ECX + 4],0x0
         TEST       ESI,ESI
         SUB        ESP,0x14
         MOV        ECX,ESP
         LEA        EAX,chat_room
         PUSH       EAX
         CALL       init_chat_msg_addr                                    
         MOV        ECX,temp
         MOV        EAX,dword ptr[members_ptr]
         PUSH       EAX
         CALL       add_member_addr
         MOV        success,EAX        
         POPFD
         POPAD
  }
  return success;
}


int GetMemberFromChatRoom(wchar_t* chat_room_id,ChatRoomInner & out){
  int success = 0;
   WeChatString chat_room(chat_room_id);
   DWORD chat_room_ptr = (DWORD) &chat_room;
  char buffer[0x1D4] = {0};
  DWORD base = GetWeChatWinBase();
  DWORD get_member_addr =  base + WX_GET_MEMBER_FROM_CHAT_ROOM_OFFSET;
  DWORD get_chat_room_mgr_addr = base + WX_CHAT_ROOM_MGR_OFFSET;
  DWORD create_chat_room_addr = base + WX_INIT_CHAT_ROOM_OFFSET;
  DWORD free_chat_room_addr = base + WX_FREE_CHAT_ROOM_OFFSET;
  __asm {
    PUSHAD
    LEA       ECX,buffer
    CALL      create_chat_room_addr
    CALL      get_chat_room_mgr_addr
    LEA       EAX, buffer
    PUSH      EAX
    PUSH      chat_room_ptr
    CALL      get_member_addr
    MOVZX     EAX,AL
    MOV       success,EAX
    POPAD
  }
  char* members  = *(char **)(buffer +0x1c);
  wchar_t* room  = *(wchar_t **)(buffer +0x8);
  wchar_t* admin  = *(wchar_t **)(buffer +0x4c);
 
  out.members = new char[strlen(members) + 1];
  memcpy(out.members, members, strlen(members) + 1);

  out.chat_room = new wchar_t[wcslen(room)+1];
  wmemcpy(out.chat_room ,room,wcslen(room)+1);

  out.admin = new wchar_t[wcslen(admin)+1];
  wmemcpy(out.admin ,admin,wcslen(admin)+1);

  __asm{
    LEA       ECX,buffer
    CALL      free_chat_room_addr
  }  
  return success;
}

int ModChatRoomMemberNickName(wchar_t* chat_room_id,wchar_t* wxid,wchar_t * nick){
  int success = 0;
  WeChatString chat_room(chat_room_id);
  WeChatString self_wxid(wxid);
  WeChatString new_nick(nick);
  DWORD base = GetWeChatWinBase();
  DWORD get_chat_room_mgr_addr = base + WX_CHAT_ROOM_MGR_OFFSET;
  DWORD mod_member_nick_name_addr = base + WX_MOD_CHAT_ROOM_MEMBER_NICK_NAME_OFFSET;
  DWORD init_chat_msg_addr = base + WX_INIT_CHAT_MSG_OFFSET;
  __asm{
    PUSHAD
    CALL       get_chat_room_mgr_addr                               
    SUB        ESP,0x14
    MOV        ECX,ESP
    LEA        EDI,new_nick
    PUSH       EDI
    CALL       init_chat_msg_addr                                      
    SUB        ESP,0x14
    LEA        EAX,self_wxid
    MOV        ECX,ESP
    PUSH       EAX
    CALL       init_chat_msg_addr                                      
    SUB        ESP,0x14
    LEA        EAX,chat_room
    MOV        ECX,ESP
    PUSH       EAX
    CALL       init_chat_msg_addr                                      
    CALL       mod_member_nick_name_addr   
    MOVZX     EAX,AL
    MOV       success,EAX      
    POPAD
  }
  return success;
}