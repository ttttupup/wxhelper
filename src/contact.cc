#include "pch.h"
#include "contact.h"

#include "common.h"
#include "wechat_data.h"
using namespace std;
#define WX_CONTACT_MGR_INSTANCE_OFFSET 0x6f8990
#define WX_CONTACT_GET_LIST_OFFSET 0xb97550
#define WX_CONTACT_DEL_OFFSET 0xb9b3b0
#define WX_INIT_CHAT_MSG_OFFSET 0xed3be0
#define WX_SYNC_MGR_OFFSET 0xa87fd0
#define WX_SET_VALUE_OFFSET 0x1f80900
#define WX_DO_DEL_CONTACT_OFFSET 0xca6480
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


