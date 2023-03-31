#include "pch.h"
#include "sns_mgr.h"


#include "wechat_function.h"


namespace wxhelper {
SNSMgr::SNSMgr(DWORD base):BaseMgr(base) {}
SNSMgr::~SNSMgr() {}
int SNSMgr::GetFirstPage() {
  int success = -1;
  DWORD sns_data_mgr_addr = base_addr_ + WX_SNS_DATA_MGR_OFFSET;
  DWORD get_first_page_addr = base_addr_ + WX_SNS_GET_FIRST_PAGE_OFFSET;

  char buff[0xB44] = {};
  __asm {
    PUSHAD
    CALL       sns_data_mgr_addr                                 
    PUSH       0x1
    LEA        ECX,buff
    PUSH       ECX
    MOV        ECX,EAX
    CALL       get_first_page_addr   
    MOV        success,EAX
    POPAD
  }

  return success;
}
int SNSMgr::GetNextPage(ULONG64 sns_id) {
  int success = -1;
  DWORD sns_data_mgr_addr = base_addr_ + WX_SNS_DATA_MGR_OFFSET;
  DWORD get_next_page_addr = base_addr_ + WX_SNS_GET_NEXT_PAGE_OFFSET;
  VectorInner temp = {};
  __asm {
    PUSHAD
    CALL       sns_data_mgr_addr                                    
    LEA        ECX,temp
    PUSH       ECX
    MOV        EBX,dword ptr [sns_id + 0x4]
    PUSH       EBX
    MOV        EDI,dword ptr [sns_id ]
    PUSH       EDI
    MOV        ECX,EAX
    CALL       get_next_page_addr
    MOV        success,EAX
    POPAD
  }
  return success;
}
}  // namespace wxhelper