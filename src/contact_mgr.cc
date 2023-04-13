#include "pch.h"
#include "contact_mgr.h"


#include "wechat_function.h"

using namespace std;
namespace wxhelper {
ContactMgr::ContactMgr(DWORD base) : BaseMgr(base) {}
ContactMgr::~ContactMgr() {}
int ContactMgr::GetAllContact(vector<Contact> &vec) {
  DWORD get_instance = base_addr_ + WX_CONTACT_MGR_OFFSET;
  DWORD contact_get_list = base_addr_ + WX_CONTACT_GET_LIST_OFFSET;
  DWORD *contact[3] = {0, 0, 0};
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
  DWORD start = (DWORD)contact[0];
  DWORD end = (DWORD)contact[2];
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
int ContactMgr::DelContact(wchar_t *wxid) {
  int success = -1;
  WeChatString user_id(wxid);
  DWORD id_ptr = (DWORD)&user_id;
  DWORD sync_mgr_addr = base_addr_ + WX_SYNC_MGR_OFFSET;
  DWORD set_id_addr = base_addr_ + WX_SET_VALUE_OFFSET;
  DWORD del_contact_addr = base_addr_ + WX_DO_DEL_CONTACT_OFFSET;
  int len = user_id.length;
  wstring ws_wxid(wxid);

  string id_cstr = Utils::WstringToUTF8(ws_wxid);
  char id_[0x20] = {0};
  memcpy(id_, id_cstr.c_str(), id_cstr.size() + 1);
  char buff[0x10] = {0};
  __asm {
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
wstring ContactMgr::GetContactOrChatRoomNickname(wchar_t *id) {
  int success = -1;
  char buff[0x440] = {0};
  WeChatString pri(id);
  DWORD contact_mgr_addr = base_addr_ + WX_CONTACT_MGR_OFFSET;
  DWORD get_contact_addr = base_addr_ + WX_GET_CONTACT_OFFSET;
  DWORD free_contact_addr = base_addr_ + WX_FREE_CONTACT_OFFSET;
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

int ContactMgr::AddFriendByWxid(wchar_t *wxid,wchar_t* msg) {
  int success = -1;
  DWORD contact_mgr_addr = base_addr_ + WX_CONTACT_MGR_OFFSET;
  DWORD verify_msg_addr = base_addr_ + WX_VERIFY_MSG_OFFSET;
  DWORD set_value_addr = base_addr_ + WX_INIT_CHAT_MSG_OFFSET;
  DWORD do_verify_user_addr = base_addr_ + WX_DO_VERIFY_USER_OFFSET;
  DWORD fn1_addr = base_addr_ + 0x758720;
  WeChatString user_id(wxid);
  WeChatString w_msg(msg);
  DWORD instance =0;
  Unkown null_obj={0,0,0,0,0,0xF};
 __asm{
        PUSHAD
        PUSHFD
        CALL       contact_mgr_addr     
        MOV        dword ptr [instance],EAX     
        MOV        EDI,0x6
        MOV        ESI,0
        MOV        EAX,0x2  
        SUB        ESP,0x18                                         
        MOV        EAX,ESP
        MOV        dword ptr ds:[EAX],0   
        MOV        dword ptr ds:[EAX+0x14],0xF
        MOV        dword ptr ds:[EAX+0x10],0 
        MOV        byte ptr ds:[EAX],0  
        SUB        ESP,0x18
        LEA        EAX,null_obj                              
        MOV        ECX,ESP
        PUSH       EAX                                               
        CALL       fn1_addr                                     
        PUSH       0x0
        PUSH       0x6
        MOV        EAX,w_msg       

        SUB        ESP,0x14
        MOV        ECX,ESP
        PUSH       -0x1
        PUSH       EAX
        CALL       verify_msg_addr                                     
        PUSH       0x2
        LEA        EAX,user_id

        SUB        ESP,0x14
        MOV        ECX,ESP
        PUSH       EAX
        CALL       set_value_addr                                 
        MOV        ECX,dword ptr [instance] 
        CALL       do_verify_user_addr    
        MOV        success,EAX          
        POPFD         
        POPAD
 }
  return success;
}

 int ContactMgr::VerifyApply(wchar_t *v3, wchar_t *v4){
  int success = -1;
  DWORD set_value_addr = base_addr_ + WX_INIT_CHAT_MSG_OFFSET;
  DWORD verify_addr = base_addr_ + WX_VERIFY_OK_OFFSET;
  DWORD new_helper_addr = base_addr_ + WX_NEW_ADD_FRIEND_HELPER_OFFSET;
  DWORD free_helper_addr = base_addr_ + WX_FREE_ADD_FRIEND_HELPER_OFFSET;
  
  WeChatString v4_str(v4);
  WeChatString v3_str(v3);
  char helper_obj[0x40] = {0};
  char nullbuffer[0x3CC] = {0};
  __asm {
      PUSHAD  
      PUSHFD         
      LEA        ECX,helper_obj
      CALL       new_helper_addr
      MOV        ESI,0x0
      MOV        EDI,0x6                   
      PUSH       ESI
      PUSH       EDI
      SUB        ESP,0x14
      MOV        ECX,ESP
      LEA        EAX,v4_str
      PUSH       EAX
      CALL       set_value_addr                                  
      SUB        ESP,0x8
      PUSH       0x0
      LEA        EAX, nullbuffer
      PUSH       EAX
      LEA        EAX,v3_str
      PUSH       EAX
      LEA        ECX,helper_obj        
      CALL       verify_addr                       
      MOV        success,EAX     
      LEA        ECX,helper_obj
      CALL       free_helper_addr
      POPFD         
      POPAD
  }
  return success;
 }
}  // namespace wxhelper