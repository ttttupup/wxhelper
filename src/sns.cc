#include "pch.h"
#include "sns.h"

#include "common.h"
#include "wechat_data.h"
using namespace std;
#define WX_SNS_DATA_MGR_OFFSET 0xac66a0
#define WX_SNS_GET_FIRST_PAGE_OFFSET 0x12e46c0
#define WX_SNS_TIME_LINE_MGR_OFFSET 0x128e6a0
#define WX_SNS_TRY_GET_FIRST_PAGE_SCENE_OFFSET 0x12ff300
#define WX_SNS_GET_NEXT_PAGE_OFFSET  0x12e4760

int GetFirstPage() {
  int success = -1;
  DWORD base = GetWeChatWinBase();
  DWORD sns_data_mgr_addr = base + WX_SNS_DATA_MGR_OFFSET;
  DWORD get_first_page_addr = base + WX_SNS_GET_FIRST_PAGE_OFFSET;

  DWORD time_line_mgr_addr = base + WX_SNS_TIME_LINE_MGR_OFFSET;
  DWORD get_first_page_scene_addr = base + WX_SNS_TRY_GET_FIRST_PAGE_SCENE_OFFSET;
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

//   __asm {
//     PUSHAD
//     CALL time_line_mgr_addr 
//     PUSH 0x1 
//     MOV ECX,EAX 
//     CALL get_first_page_scene_addr 
//     MOV success, EAX 
//     POPAD
//  }
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
