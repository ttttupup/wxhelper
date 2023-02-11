#include "pch.h"
#include "confirm_receipt.h"

#include "common.h"
#include "wechat_data.h"

#define WX_NEW_WCPAYINFO_OFFSET 0x756340
#define WX_FREE_WCPAYINFO_OFFSET 0x73c170
#define WX_CONFIRM_RECEIPT_OFFSET 0x15287a0

int DoConfirmReceipt(wchar_t *wxid, wchar_t *transcationid,
                     wchar_t *transferid) {
  int success = -1;
  WeChatString recv_id(wxid);
  WeChatString transcation_id(transcationid);
  WeChatString transfer_id(transferid);
  char pay_info[0x134] = {0};
  DWORD base = GetWeChatWinBase();
  DWORD new_pay_info_addr = base + WX_NEW_WCPAYINFO_OFFSET;
  DWORD free_pay_info_addr = base + WX_FREE_WCPAYINFO_OFFSET;
  DWORD do_confirm_addr = base + WX_CONFIRM_RECEIPT_OFFSET;
  __asm {
    PUSHAD
    LEA        ECX,pay_info
    CALL       new_pay_info_addr
    MOV        dword ptr [pay_info + 0x4], 0x1
    MOV        dword ptr [pay_info + 0x4C], 0x1        
    POPAD
  }
  memcpy(&pay_info[0x1c], &transcation_id, sizeof(transcation_id));
  memcpy(&pay_info[0x38], &transfer_id, sizeof(transfer_id));

  __asm {
    PUSHAD
    PUSH       0x1
    SUB        ESP,0x8
    LEA        EDX,recv_id
    LEA        ECX,pay_info
    CALL       do_confirm_addr  
    MOV        success,EAX  
    ADD        ESP,0xC
    PUSH       0x0
    LEA        ECX,pay_info
    CALL       free_pay_info_addr 
    POPAD
  }

  return success;
}