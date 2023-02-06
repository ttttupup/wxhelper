#include "pch.h"
#include "ocr.h"

#include "common.h"
#include "wechat_data.h"

#define WX_INIT_OBJ_OFFSET 0x6cbab0
#define WX_OCR_MANAGER_OFFSET 0x6cff00
#define WX_DO_OCR_TASK_OFFSET 0x11e3210
using namespace std;
int DoOCRTask(wchar_t *img_path, std::string &result) {
  int success = -1;
  WeChatString path(img_path);
  WeChatString null_obj = {0};
  WeChatString ocr_result = {0};
  DWORD base = GetWeChatWinBase();
  DWORD ocr_manager_addr = base + WX_OCR_MANAGER_OFFSET;
  DWORD do_ocr_task_addr = base + WX_DO_OCR_TASK_OFFSET;
  DWORD init_addr = base + WX_INIT_OBJ_OFFSET;
  __asm {
        PUSHAD
        PUSHFD
        LEA        ECX,ocr_result
        CALL       init_addr
        CALL       ocr_manager_addr                                  
        LEA        ECX,null_obj
        PUSH       ECX
        LEA        ECX,ocr_result
        PUSH       ECX
        PUSH       ECX
        LEA        ECX,path
        PUSH       ECX
        MOV        ECX,EAX
        CALL       do_ocr_task_addr    
        MOV        success,EAX  
        POPFD
        POPAD
  }

  if (success == 0) {
    DWORD addr = (DWORD)&ocr_result;
    DWORD ptr = *(DWORD *)addr;
    DWORD num = *(DWORD *)(addr + 0x4);
    if (num <= 0) {
      return success;
    }

    DWORD header = *(DWORD *)ptr;
    for (unsigned int i = 0; i < num -1; i++) {
      DWORD content = *(DWORD *)header;
      result += unicode_to_utf8((wchar_t *)READ_WSTRING(content, 0x14).c_str());
     
      header = content;
    }
#ifdef _DEBUG
    cout << "num:" << num << endl;
    cout << "all:" << result << endl;
#endif
  }
  return success;
}