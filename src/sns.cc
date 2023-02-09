#include "pch.h"
#include "sns.h"

#include "common.h"
#include "wechat_data.h"
using namespace std;
#define WX_SNS_DATA_MGR_OFFSET 0xbc4100
#define WX_SNS_GET_FIRST_PAGE_OFFSET 0x1427be0
#define WX_SNS_GET_NEXT_PAGE_OFFSET  0x1427c80

int GetFirstPage() {
  int success = -1;
  DWORD base = GetWeChatWinBase();
  DWORD sns_data_mgr_addr = base + WX_SNS_DATA_MGR_OFFSET;
  DWORD get_first_page_addr = base + WX_SNS_GET_FIRST_PAGE_OFFSET;

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


int GetNextPage(ULONG64 sns_id) {
  int success = -1;
  DWORD base = GetWeChatWinBase();
  DWORD sns_data_mgr_addr = base + WX_SNS_DATA_MGR_OFFSET;
  DWORD get_next_page_addr = base + WX_SNS_GET_NEXT_PAGE_OFFSET;
  VectorInner temp = {};
  __asm{
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
