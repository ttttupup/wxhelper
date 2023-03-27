#include "pch.h"
#include "self_info.h"

#include "common.h"

#include "wechat_data.h"
using namespace std;



#define WX_LOGOUT_OFFSET 0xe58870
#define WX_ACCOUNT_SERVICE_OFFSET 0x768c80
#define WX_GET_APP_DATA_SAVE_PATH_OFFSET 0xf3a610
#define WX_GET_CURRENT_DATA_PATH_OFFSET 0xc872c0
int GetSelfInfo(SelfInfoInner &out) {
  DWORD base = GetWeChatWinBase();
  DWORD accout_service_addr = base + WX_ACCOUNT_SERVICE_OFFSET;
  DWORD get_app_save_addr = base + WX_GET_APP_DATA_SAVE_PATH_OFFSET;
  DWORD get_current_data_path_addr = base + WX_GET_CURRENT_DATA_PATH_OFFSET;
  DWORD service_addr = NULL;
  __asm {
      PUSHAD 
      CALL       accout_service_addr                          
      MOV        service_addr,EAX
      POPAD
  }
  if (service_addr) {
    if (*(DWORD *)(service_addr + 0x44) == 0 ||
        *(DWORD *)(service_addr + 0x44 + 0x10) == 0) {
      out.wxid = string();
    } else {
      if (*(DWORD *)(service_addr + 0x44 + 0x14) == 0xF) {
        out.wxid = string((char *)(service_addr + 0x44),
                          *(DWORD *)(service_addr + 0x44 + 0x10));
      } else {
        out.wxid = string(*(char **)(service_addr + 0x44),
                          *(DWORD *)(service_addr + 0x44 + 0x10));
      }
    }

    if (*(DWORD *)(service_addr + 0xA8) == 0 ||
        *(DWORD *)(service_addr + 0xA8 + 0x10) == 0) {
      out.account = string();
    } else {
      if (*(DWORD *)(service_addr + 0xA8 + 0x14) == 0xF) {
        out.account = string((char *)(service_addr + 0xA8),
                             *(DWORD *)(service_addr + 0xA8 + 0x10));
      } else {
        out.account = string(*(char **)(service_addr + 0xA8),
                             *(DWORD *)(service_addr + 0xA8 + 0x10));
      }
    }

    if (*(DWORD *)(service_addr + 0xC0) == 0 ||
        *(DWORD *)(service_addr + 0xC0 + 0x10) == 0) {
      out.mobile = string();
    } else {
      if (*(DWORD *)(service_addr + 0xC0 + 0x14) == 0xF) {
        out.mobile = string((char *)(service_addr + 0xC0),
                            *(DWORD *)(service_addr + 0xC0 + 0x10));
      } else {
        out.mobile = string(*(char **)(service_addr + 0xC0),
                            *(DWORD *)(service_addr + 0xC0 + 0x10));
      }
    }

    if (*(DWORD *)(service_addr + 0xD8) == 0 ||
        *(DWORD *)(service_addr + 0xD8 + 0x10) == 0) {
      out.signature = string();
    } else {
      if (*(DWORD *)(service_addr + 0xD8 + 0x14) == 0xF) {
        out.signature = string((char *)(service_addr + 0xD8),
                               *(DWORD *)(service_addr + 0xD8 + 0x10));
      } else {
        out.signature = string(*(char **)(service_addr + 0xD8),
                               *(DWORD *)(service_addr + 0xD8 + 0x10));
      }
    }

    if (*(DWORD *)(service_addr + 0xF0) == 0 ||
        *(DWORD *)(service_addr + 0xF0 + 0x10) == 0) {
      out.country = string();
    } else {
      if (*(DWORD *)(service_addr + 0xF0 + 0x14) == 0xF) {
        out.country = string((char *)(service_addr + 0xF0),
                             *(DWORD *)(service_addr + 0xF0 + 0x10));
      } else {
        out.country = string(*(char **)(service_addr + 0xF0),
                             *(DWORD *)(service_addr + 0xF0 + 0x10));
      }
    }

    if (*(DWORD *)(service_addr + 0x108) == 0 ||
        *(DWORD *)(service_addr + 0x108 + 0x10) == 0) {
      out.province = string();
    } else {
      if (*(DWORD *)(service_addr + 0x108 + 0x14) == 0xF) {
        out.province = string((char *)(service_addr + 0x108),
                              *(DWORD *)(service_addr + 0x108 + 0x10));
      } else {
        out.province = string(*(char **)(service_addr + 0x108),
                              *(DWORD *)(service_addr + 0x108 + 0x10));
      }
    }

    if (*(DWORD *)(service_addr + 0x120) == 0 ||
        *(DWORD *)(service_addr + 0x120 + 0x10) == 0) {
      out.city = string();
    } else {
      if (*(DWORD *)(service_addr + 0x120 + 0x14) == 0xF) {
        out.city = string((char *)(service_addr + 0x120),
                          *(DWORD *)(service_addr + 0x120 + 0x10));
      } else {
        out.city = string(*(char **)(service_addr + 0x120),
                          *(DWORD *)(service_addr + 0x120 + 0x10));
      }
    }

    if (*(DWORD *)(service_addr + 0x150) == 0 ||
        *(DWORD *)(service_addr + 0x150 + 0x10) == 0) {
      out.name = string();
    } else {
      if (*(DWORD *)(service_addr + 0x150 + 0x14) == 0xF) {
        out.name = string((char *)(service_addr + 0x150),
                          *(DWORD *)(service_addr + 0x150 + 0x10));
      } else {
        out.name = string(*(char **)(service_addr + 0x150),
                          *(DWORD *)(service_addr + 0x150 + 0x10));
      }
    }

    if (*(DWORD *)(service_addr + 0x304) == 0 ||
        *(DWORD *)(service_addr + 0x304 + 0x10) == 0) {
      out.head_img = string();
    } else {
      if (*(DWORD *)(service_addr + 0x304 + 0x14) == 0xF) {
        out.head_img = string((char *)(service_addr + 0x304),
                              *(DWORD *)(service_addr + 0x304 + 0x10));
      } else {
        out.head_img = string(*(char **)(service_addr + 0x304),
                              *(DWORD *)(service_addr + 0x304 + 0x10));
      }
    }

    if (*(DWORD *)(service_addr + 0x4CC) == 0 ||
        *(DWORD *)(service_addr +0x4D0) == 0) {
      out.db_key = string();
    } else {
      DWORD  byte_addr = *(DWORD *)(service_addr + 0x4CC);
      DWORD len = *(DWORD *)(service_addr +0x4D0);
      out.db_key = Bytes2Hex((BYTE *)byte_addr,len);
    }
  }

  WeChatString data_save_path;
  WeChatString current_data_path;

  __asm {
      PUSHAD
      LEA     ECX,data_save_path
      CALL    get_app_save_addr
      LEA     ECX,current_data_path
      CALL    get_current_data_path_addr
      POPAD
  }

  if (data_save_path.ptr) {
    out.data_save_path =
        Wstring2String(wstring(data_save_path.ptr, data_save_path.length));
  }
  else {
    out.data_save_path = string();
  }

  if (current_data_path.ptr) {
    out.current_data_path = Wstring2String(
        wstring(current_data_path.ptr, current_data_path.length));
  } else {
    out.current_data_path = string();
  }
  return 1;
}

int CheckLogin() {
  DWORD base = GetWeChatWinBase();
  DWORD accout_service_addr = base + WX_ACCOUNT_SERVICE_OFFSET;
  DWORD service_addr = NULL;
  __asm {
      PUSHAD 
      CALL       accout_service_addr                          
      MOV        service_addr,EAX
      POPAD
  }
  if (service_addr) {
    return *(DWORD *)(service_addr + 0x4C8);
  }
  else {
    return 0;
  }
}

int Logout() {
  int success = 0;
  if (!CheckLogin()) {
    return success;
  }
  DWORD base = GetWeChatWinBase();
  DWORD account_service_addr = base + WX_ACCOUNT_SERVICE_OFFSET;
  DWORD logout_addr = base + WX_LOGOUT_OFFSET;
  __asm {
    PUSHAD
    CALL       account_service_addr                              
    PUSH       0x0
    MOV        ECX,EAX
    CALL       logout_addr 
    MOV        success,EAX                         
    POPAD
  }
  return success;
}