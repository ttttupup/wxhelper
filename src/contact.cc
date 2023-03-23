#include "pch.h"
#include "contact.h"

#include "common.h"
#include "wechat_data.h"
using namespace std;
#define WX_CONTACT_MGR_INSTANCE_OFFSET 0x75a4a0
#define WX_CONTACT_GET_LIST_OFFSET 0xc089f0
#define WX_CONTACT_DEL_OFFSET 0xb9b3b0
#define WX_INIT_CHAT_MSG_OFFSET 0xed3be0
#define WX_SYNC_MGR_OFFSET 0xa87fd0
#define WX_SET_VALUE_OFFSET 0x1f80900
#define WX_DO_DEL_CONTACT_OFFSET 0xca6480
#define WX_FREE_CONTACT_OFFSET  0xe23690
#define WX_GET_CONTACT_OFFSET  0xb93b20
#define WX_DO_VERIFY_USER_OFFSET  0xB91160

int GetAllContact(vector<Contact> &vec) {
  DWORD base = GetWeChatWinBase();
  DWORD get_instance = base + WX_CONTACT_MGR_INSTANCE_OFFSET;
  DWORD contact_get_list = base + WX_CONTACT_GET_LIST_OFFSET;
  // char contact[0xc] = {0};
  DWORD* contact[3] = {0, 0, 0};
  int success = 0;
  __asm {
        PUSHAD                     
        CALL       get_instance   
        LEA        ECX,contact
        PUSH       ECX
        MOV        ECX,EAX
        CALL       contact_get_list
        MOVZX      EAX,AL
        MOV        success,EAX
        POPAD
  }
 DWORD start =   (DWORD)contact[0];
 DWORD end =   (DWORD)contact[2];
#ifdef _DEBUG
 cout << "address = " << &contact << endl;
 cout << "refresh contact = " << success << endl;
 cout << "start  = " << start << endl;
 cout << "end    = " << end << endl;
#endif
 while (start < end) {
   Contact temp{0};

   temp.wxid.ptr = *(wchar_t **)(start + 0x10);
   temp.wxid.length = *(DWORD *)(start + 0x14);
   temp.wxid.max_length = *(DWORD *)(start + 0x18);

   temp.custom_account.ptr = *(wchar_t **)(start + 0x24);
   temp.custom_account.length = *(DWORD *)(start + 0x28);
   temp.custom_account.max_length = *(DWORD *)(start + 0x2C);

   temp.encrypt_name.ptr = *(wchar_t **)(start + 0x6c);
   temp.encrypt_name.length = *(DWORD *)(start + 0x70);
   temp.encrypt_name.max_length = *(DWORD *)(start + 0x74);

   temp.pinyin.ptr = *(wchar_t **)(start + 0xAC);
   temp.pinyin.length = *(DWORD *)(start + 0xB0);
   temp.pinyin.max_length = *(DWORD *)(start + 0xB4);

   temp.pinyin_all.ptr = *(wchar_t **)(start + 0xC0);
   temp.pinyin_all.length = *(DWORD *)(start + 0xC4);
   temp.pinyin_all.max_length = *(DWORD *)(start + 0xC8);

   temp.del_flag = *(DWORD *)(start + 0x4c);
   temp.type = *(DWORD *)(start + 0x50);
   temp.verify_flag = *(DWORD *)(start + 0x54);
   vec.push_back(temp);
   start += 0x438;
    }
    return success;
}
// note maybe  not delete 
int DelContact(wchar_t *wxid) {
    int success = -1;
    WeChatString user_id(wxid);
    DWORD id_ptr = (DWORD) &user_id;
    DWORD base = GetWeChatWinBase();
    DWORD sync_mgr_addr = base + WX_SYNC_MGR_OFFSET;
    DWORD set_id_addr = base + WX_SET_VALUE_OFFSET;
    DWORD del_contact_addr = base + WX_DO_DEL_CONTACT_OFFSET;
    int len = user_id.length;
    
    string id_cstr = unicode_to_utf8(wxid);
    char id_[0x20]={0};
    memcpy(id_,id_cstr.c_str(),id_cstr.size()+1);
    char buff[0x10]={0}; 
    __asm{
        PUSHAD
        PUSHFD
        CALL       sync_mgr_addr
        MOV        ECX,EAX
        LEA        EAX,buff
        MOV        [ECX + 4],EAX
        LEA        EAX,id_
        Mov        dword ptr[buff +0x4],EAX
        CALL       del_contact_addr 
        MOV        success,EAX            
        POPFD                 
        POPAD      
    }
    return success;
}

std::wstring GetContactOrChatRoomNickname(wchar_t *id) {
    int success = -1;
    char buff[0x440] = {0};
    WeChatString pri(id);
    DWORD base = GetWeChatWinBase();
    DWORD contact_mgr_addr = base + WX_CONTACT_MGR_INSTANCE_OFFSET;
    DWORD get_contact_addr = base + WX_GET_CONTACT_OFFSET;
    DWORD free_contact_addr = base + WX_FREE_CONTACT_OFFSET;
    wstring name = L"";
    __asm {
      PUSHAD
      PUSHFD
      CALL       contact_mgr_addr                                   
      LEA        ECX,buff
      PUSH       ECX
      LEA        ECX,pri
      PUSH       ECX
      MOV        ECX,EAX
      CALL       get_contact_addr                            
      POPFD
      POPAD
    }
    name += READ_WSTRING(buff, 0x6C);
    __asm {
      PUSHAD
      PUSHFD
      LEA        ECX,buff
      CALL       free_contact_addr    
      POPFD
      POPAD
    }
    return name;
}


int AddFriendByWxid(wchar_t *wxid){
  int success = -1;
  DWORD base = GetWeChatWinBase();
  DWORD contact_mgr_addr = base + WX_CONTACT_MGR_INSTANCE_OFFSET;
  DWORD set_group_addr = base + 0x746E20;
  DWORD fn2_addr = base + 0x285D968;
  DWORD fn3_addr = base + 0x6F6050;
  DWORD fn4_addr = base + 0xED3BE0;
  DWORD do_verify_user_addr = base + WX_DO_VERIFY_USER_OFFSET;

  DWORD instance =0;
  WeChatString chat_room(NULL);
  WeChatString user_id(wxid);
  __asm{
    PUSHAD
    PUSHFD
    CALL        contact_mgr_addr    
    SUB         ESP,0x18                                     
    MOV         dword ptr [instance],EAX                  
    MOV         ECX,ESP                                    
    PUSH        ECX                                       
    LEA         ECX,chat_room                  
    CALL        set_group_addr 
    MOV         EAX,fn2_addr                     
    SUB         ESP,0x18                                     
    MOV         ECX,ESP                                    
    PUSH        EAX                                       
    CALL        fn3_addr                       
    PUSH        0x0                                         
    PUSH        0XE                    
    SUB         ESP,0x14                                     
    MOV         ESI,ESP                                    
    MOV         dword ptr [ESI],0x0                       
    MOV         dword ptr [ESI+0x4],0x0                     
    MOV         dword ptr [ESI+0x8],0x0                     
    MOV         dword ptr [ESI+0xC],0x0                     
    MOV         dword ptr [ESI+0x10],0x0                                      
    PUSH        0x1                                         
    SUB         ESP,0x14                                     
    MOV         ECX,ESP      
    LEA         EAX,user_id                              
    PUSH        EAX                   
    CALL        fn4_addr                        
    MOV         ECX,dword ptr [instance]                  
    CALL        do_verify_user_addr            
    MOV         success,EAX         
    POPFD
    POPAD
  }
  return success;
}